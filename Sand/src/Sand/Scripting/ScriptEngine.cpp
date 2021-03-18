#include "sandpch.h"
#include "ScriptEngine.h"
#include "ScriptData.h"

#include "Sand/Core/Input.h"
#include "Sand/Debug/Debug.h"
#include "Sand/Scene/Components.h"

#include <filesystem>
#include <vector>
#include <string>

#include <mono/jit/jit.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/assembly.h>

namespace Sand
{
	// Mono shit
	static MonoDomain* s_MonoDomain = nullptr;
	static MonoImage* s_MonoImage = nullptr;
	static MonoAssembly* s_MonoAssembly = nullptr;

	static MonoAssembly* s_ClientMonoAssembly = nullptr;
	static MonoImage* s_ClientMonoImage = nullptr;

	// Script shit
	static Scene* s_ActiveScene = nullptr;

	static std::unordered_map<uint32_t, ScriptData> s_ScriptDatas = {};

	static MonoClass* s_ScriptableActorClass = nullptr;

	typedef bool(*HasComponentFunc)(Actor& actor);
	static std::unordered_map<MonoType*, HasComponentFunc> s_HasComponentFuncs = {};
	typedef void(*AddComponentFunc)(Actor& actor);
	static std::unordered_map<MonoType*, AddComponentFunc> s_AddComponentFuncs = {};

	static std::vector<MonoClass*> s_CachedClientScripts = {};

#define RegisterComponent(Type) \
	{\
		MonoType* monoType = mono_reflection_type_from_name("Sand." #Type, s_MonoImage);\
		if (!monoType) {\
			SAND_CORE_ERROR("No C# component class found for " #Type ".");\
		}\
		else if (monoType) {\
			s_HasComponentFuncs.emplace(monoType, [](Actor& actor) { return actor.HasComponent<Type>(); });\
			s_AddComponentFuncs.emplace(monoType, [](Actor& actor) { actor.AddComponent<Type>(); });\
		}\
	}
	static void InitCoreComponents()
	{
		SAND_PROFILE_FUNCTION();

		s_ScriptableActorClass = mono_class_from_name(s_MonoImage, "Sand", "ScriptableActor");
		SAND_CORE_ASSERT(s_ScriptableActorClass, "Failed to find ScriptableActor class.");

		RegisterComponent(TagComponent);
		RegisterComponent(CameraComponent);
		RegisterComponent(TransformComponent);
		RegisterComponent(SpriteRendererComponent);
	}
#undef RegisterComponent

	void ScriptEngine::Init()
	{
		SAND_PROFILE_FUNCTION();

		mono_set_dirs("C:\\Program Files\\Mono\\lib", "C:\\Program Files\\Mono\\etc");
		mono_set_assemblies_path("lib");
		
		MonoDomain* domain = mono_jit_init("Sand");

		s_MonoDomain = mono_domain_create_appdomain((char*)"Sand-Runtime", nullptr);

		// Core
		const char* path = "D:\\dev\\Sand\\bin\\Debug-windows-x86_64\\Sand-CSCore\\Sand-CSCore.dll";
		s_MonoAssembly = mono_domain_assembly_open(s_MonoDomain, path);
		SAND_CORE_ASSERT(s_MonoAssembly, "Failed to load core MonoAssembly");
		mono_assembly_set_main(s_MonoAssembly);

		s_MonoImage = mono_assembly_get_image(s_MonoAssembly);

		InitCoreComponents();

		LoadClientAssembly();

		SetupInternalCalls();
	}
	
	void ScriptEngine::Shutdown()
	{
		mono_jit_cleanup(s_MonoDomain);
	}

	void ScriptEngine::SetActiveScene(Scene* const scene)
	{
		s_ActiveScene = scene;
	}

	void ScriptEngine::ReloadClientAssembly()
	{
		SAND_PROFILE_FUNCTION();

		// Cleanup
		s_ScriptDatas.clear();
		s_CachedClientScripts.clear();

		mono_image_close(s_ClientMonoImage);
		mono_assembly_close(s_ClientMonoAssembly);

		// Load
		LoadClientAssembly();
	}
	
	// Forward decl cause im into that shieet
	static std::vector<MonoClass*> FindAllClasses(MonoImage* image);

	void ScriptEngine::LoadClientAssembly()
	{
		SAND_PROFILE_FUNCTION();

		MonoDomain* domain = nullptr;
		if (s_MonoDomain)
		{
			domain = mono_domain_create_appdomain("Sand Client", nullptr);
			mono_domain_set(domain, true);
		}

		// Client
		// TODO: get correct client dll path (set on project create)
		const char* path = "D:\\dev\\SandApp\\SandApp\\bin\\x64\\Debug\\SandApp.dll";
		s_ClientMonoAssembly = mono_domain_assembly_open(s_MonoDomain, path);
		SAND_CORE_ASSERT(s_ClientMonoAssembly, "Failed to load client MonoAssembly");
		s_ClientMonoImage = mono_assembly_get_image(s_ClientMonoAssembly);

		s_CachedClientScripts = FindAllClasses(s_ClientMonoImage);
	}

	static std::vector<MonoClass*> FindAllClasses(MonoImage* image)
	{
		SAND_PROFILE_FUNCTION();

		std::vector<MonoClass*> classList;

		const MonoTableInfo* tableInfo = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);

		int rows = mono_table_info_get_rows(tableInfo);

		/* For each row, get some of its values */
		for (int i = 0; i < rows; i++)
		{
			MonoClass* klass = nullptr;
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(tableInfo, i, cols, MONO_TYPEDEF_SIZE);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			klass = mono_class_from_name(image, nameSpace, name);

			if (strcmp(name, "<Module>") == 0)
				continue; // for some reason <Module> always shows up as a class... idk bro, it just works.
			
			classList.push_back(klass);
		}

		return classList;
	}

	static std::vector<ScriptField> FindAllFields(MonoClass* klass)
	{
		SAND_PROFILE_FUNCTION();

		std::vector<ScriptField> fields;

		void* iterator = NULL;
		MonoClassField* field;
		while ((field = mono_class_get_fields(klass, &iterator)) != NULL)
		{
			MonoType* fieldType = mono_field_get_type(field);
			const char* name = mono_field_get_name(field);
			ScriptDataType scriptFieldType = ScriptEngine::MonoTypeToScriptDataType(fieldType);
			bool isPublic = ScriptEngine::MonoFieldIsPublic(field);

			fields.emplace_back(field, name, isPublic, scriptFieldType);
		}

		return fields;
	}

	bool ScriptEngine::ModuleExists(const std::string& moduleName)
	{
		MonoClass* klass = mono_class_from_name(s_ClientMonoImage, "Client", moduleName.c_str());

		return klass != nullptr;
	}

	bool ScriptEngine::ModuleActive(Actor actor)
	{
		auto sdata = s_ScriptDatas.find(actor);
		return sdata != s_ScriptDatas.end() && sdata->second.Object != nullptr;
	}

	void ScriptEngine::AddModule(uint32_t actorID, const std::string& moduleName)
	{
		SAND_PROFILE_FUNCTION();

		MonoClass* klass = mono_class_from_name(s_ClientMonoImage, "Client", moduleName.c_str());

		if (!klass)
			return;

		SAND_CORE_TRACE("Activating module {0}::{1}", "Client", moduleName);
		
		ScriptData sdata = {};
		sdata.Class = klass;
		
		MonoMethod* initializeMethod = NULL;
		{
			SAND_PROFILE_SCOPE("ScriptEngine::AddModule - Getting methods from Script");;
			
			// Get all the methods we need
			sdata.OnLoadMethod    = mono_class_get_method_from_name(klass, "OnLoad", 0);
			sdata.OnCreateMethod  = mono_class_get_method_from_name(klass, "OnCreate", 0);
			sdata.OnUpdateMethod  = mono_class_get_method_from_name(klass, "OnUpdate", 1);
			sdata.OnLateUpdateMethod = mono_class_get_method_from_name(klass, "OnLateUpdate", 1);
			sdata.OnDestroyMethod = mono_class_get_method_from_name(klass, "OnDestroy", 0);

			// Get initialization method
			initializeMethod = mono_class_get_method_from_name(s_ScriptableActorClass, "Initialize", 1);

			sdata.Object = mono_object_new(s_MonoDomain, klass);
		}
		{
			SAND_PROFILE_SCOPE("ScriptEngine::AddModule - FindAllFields()");
			sdata.Fields = FindAllFields(klass);
		}
		
		// Call scriptable actor constructor
		void* params[] = { &actorID };
		mono_runtime_invoke(initializeMethod, sdata.Object, params, NULL);

		s_ScriptDatas.emplace(actorID, sdata);
	}

	static inline void ScriptDataOnDestroy(const ScriptData& scriptData);

	void ScriptEngine::DeactivateModule(Actor actor)
	{
		SAND_CORE_TRACE("Deactivating module on '{0}'", actor.GetComponent<TagComponent>().Name);

		// Call OnDestroy if the game is running
		if (s_ActiveScene->IsPlaying())
			ScriptDataOnDestroy(s_ScriptDatas[actor]);

		s_ScriptDatas.erase(actor);
	}

	ScriptData* ScriptEngine::GetScriptData(uint32_t actor, const std::string& moduleName)
	{
		SAND_PROFILE_FUNCTION();

		MonoClass* klass = mono_class_from_name(s_ClientMonoImage, "Client", moduleName.c_str());
		if (!klass || !s_ScriptDatas.size()) {
			// Nah fam
			return nullptr;
		}

		if (klass && s_ScriptDatas.find(actor) == s_ScriptDatas.end())
		{
			// We haven't initialized this module yet, but we should
			AddModule(actor, moduleName);
		}

		for (auto& sdata : s_ScriptDatas)
		{
			if (sdata.first == actor)
			{
				return &sdata.second;
			}
		}

		return nullptr;
	}

	static inline void ScriptDataOnLoad(const ScriptData& scriptData)
	{
		if (!scriptData.OnLoadMethod)
			return;

		MonoObject* exception = NULL;
		mono_runtime_invoke(scriptData.OnLoadMethod, scriptData.Object, NULL, &exception);
		if (exception)
		{
			char* toString = mono_string_to_utf8(mono_object_to_string(exception, NULL));
			SAND_CORE_ERROR(toString);
			mono_free(toString);
		}
	}

	static inline void ScriptDataOnCreate(const ScriptData& scriptData)
	{
		if (!scriptData.OnCreateMethod)
			return;

		MonoObject* exception = NULL;
		{
			SAND_PROFILE_SCOPE("(ScriptEngine.cpp) ScriptDataOnCreate() - mono_runtime_invoke()");
			mono_runtime_invoke(scriptData.OnCreateMethod, scriptData.Object, NULL, &exception);
		}
		if (exception)
		{
			char* toString = mono_string_to_utf8(mono_object_to_string(exception, NULL));
			SAND_CORE_ERROR(toString);
			mono_free(toString);
		}
	}

	static inline void ScriptDataOnUpdate(const ScriptData& scriptData, float ts)
	{
		SAND_PROFILE_FUNCTION();

		void* params[] = { &ts };

		if (!scriptData.OnUpdateMethod)
			return;

		MonoObject* exception = NULL;
		{
			SAND_PROFILE_SCOPE("(ScriptEngine.cpp) ScriptDataOnUpdate() - mono_runtime_invoke()");
			mono_runtime_invoke(scriptData.OnUpdateMethod, scriptData.Object, params, &exception);
		}
		if (exception)
		{
			char* toString = mono_string_to_utf8(mono_object_to_string(exception, NULL));
			SAND_CORE_ERROR(toString);
			mono_free(toString);
		}
	}

	static inline void ScriptDataOnLateUpdate(const ScriptData& scriptData, float ts)
	{
		SAND_PROFILE_FUNCTION();

		void* params[] = { &ts };

		if (!scriptData.OnLateUpdateMethod)
			return;

		MonoObject* exception = NULL;
		{
			SAND_PROFILE_SCOPE("(ScriptEnigne.cpp) ScriptDataOnLateUpdate() - mono_runtime_invoke()");
			mono_runtime_invoke(scriptData.OnLateUpdateMethod, scriptData.Object, params, &exception);
		}
		if (exception)
		{
			char* toString = mono_string_to_utf8(mono_object_to_string(exception, NULL));
			SAND_CORE_ERROR(toString);
			mono_free(toString);
		}
	}

	static inline void ScriptDataOnDestroy(const ScriptData& scriptData)
	{
		if (!scriptData.OnDestroyMethod)
			return;

		MonoObject* exception = NULL;
		{
			SAND_PROFILE_SCOPE("(ScriptEngine.cpp) ScriptDataOnDestroy() - mono_runtime_invoke()");
			mono_runtime_invoke(scriptData.OnDestroyMethod, scriptData.Object, NULL, &exception);
		}
		if (exception)
		{
			char* toString = mono_string_to_utf8(mono_object_to_string(exception, NULL));
			SAND_CORE_ERROR(toString);
			mono_free(toString);
		}
	}

	void ScriptEngine::CreateAll()
	{
		SAND_PROFILE_FUNCTION();

		// OnLoad
		for (auto& sdata : s_ScriptDatas)
		{
			ScriptDataOnLoad(sdata.second);
		}

		// OnCreate
		for (auto& sdata : s_ScriptDatas)
		{
			ScriptDataOnCreate(sdata.second);
		}
	}

	void ScriptEngine::UpdateAll(Timestep ts)
	{
		SAND_PROFILE_FUNCTION();

		// OnUpdate
		for (auto& sdata : s_ScriptDatas)
		{
			ScriptDataOnUpdate(sdata.second, ts);
		}

		// OnLateUpdate
		for (auto& sdata : s_ScriptDatas)
		{
			ScriptDataOnLateUpdate(sdata.second, ts);
		}
	}

	void ScriptEngine::DestroyAll()
	{
		SAND_PROFILE_FUNCTION();

		for (auto& sdata : s_ScriptDatas)
		{
			ScriptDataOnDestroy(sdata.second);
		}
	}

	ScriptDataType ScriptEngine::MonoTypeToScriptDataType(void* monoType)
	{
		int type = mono_type_get_type((MonoType*)monoType);
		switch (type)
		{
			case MONO_TYPE_I2:
			case MONO_TYPE_I4:
			case MONO_TYPE_I8:
				return ScriptDataType::Int;
			case MONO_TYPE_U2:
			case MONO_TYPE_U4:
			case MONO_TYPE_U8:
				return ScriptDataType::UInt;
			case MONO_TYPE_R4:
				return ScriptDataType::Float;
			case MONO_TYPE_R8:
				return ScriptDataType::Double;
			case MONO_TYPE_VALUETYPE:
			{
				const char* name = mono_type_get_name((MonoType*)monoType);

				if (strcmp(name, "Sand.Math.Vector2") == 0)
					return ScriptDataType::Vector2;
				if (strcmp(name, "Sand.Math.Vector3") == 0)
					return ScriptDataType::Vector3;
				if (strcmp(name, "Sand.Math.Vector4") == 0)
					return ScriptDataType::Vector4;
				if (strcmp(name, "Sand.Color") == 0)
					return ScriptDataType::Color;
			}
			default:
				return ScriptDataType::Unknown;
		}
	}

	bool ScriptEngine::MonoFieldIsPublic(void* field)
	{
		uint32_t flags = mono_field_get_flags((MonoClassField*)field);
		return (flags & MONO_FIELD_ATTR_PUBLIC) != 0;
	}

	std::vector<void*> ScriptEngine::GetCachedClientScripts()
	{
		// yes I know this line is cursed - it just works.
		return *reinterpret_cast<std::vector<void*>*>(&s_CachedClientScripts);
	}

#pragma region InternalFunctions
	// ===== LOGGING =====
	static void Log_PrintInfo(MonoString* string)
	{
		char* str = mono_string_to_utf8(string);
		if (str) // Don't want to log a null string
			SAND_CORE_INFO(str); 
		mono_free(str);
	}
	static void Log_PrintWarn(MonoString* string)
	{
		char* str = mono_string_to_utf8(string);
		if (str) // Don't want to log a null string
			SAND_CORE_WARN(str);
		mono_free(str);
	}
	static void Log_PrintError(MonoString* string)
	{
		char* str = mono_string_to_utf8(string);
		if (str) // Don't want to log a null string
			SAND_CORE_ERROR(str);
		mono_free(str);
	}
	//====== INPUT ========
	static inline bool Input_IsKeyPressed(Keycode button)
	{
		return Input::IsKeyPressed(button);
	}
	static inline bool Input_WasKeyPressed(Keycode button)
	{
		return Input::WasKeyPressed(button);
	}
	static inline bool Input_IsMouseButtonPressed(Mousecode button)
	{
		return Input::IsKeyPressed(button);
	}
	static inline bool Input_WasMouseButtonPressed(Mousecode button)
	{
		return Input::WasKeyPressed(button);
	}
	// ===== COMPONENTS =====
	static inline Actor ActorFromID(uint32_t id)
	{
		return { (entt::entity)id, s_ActiveScene };
	}

	// TODO: don't GetComponent<> every single function call?
	static void TagComponent_SetTag(uint32_t id, MonoString* tag)
	{
		char* str = mono_string_to_utf8(tag);
		ActorFromID(id).GetComponent<TagComponent>().Name = str;
		mono_free(str);
	}
	static MonoString* TagComponent_GetTag(uint32_t id)
	{
		return mono_string_new(s_MonoDomain, ActorFromID(id).GetComponent<TagComponent>().Name.c_str());
	}

	static void TransformComponent_SetPosition(uint32_t id, glm::vec2* position)
	{
		ActorFromID(id).GetComponent<TransformComponent>().Position = *position;
	}
	static void TransformComponent_SetRotation(uint32_t id, float rotation)
	{
		ActorFromID(id).GetComponent<TransformComponent>().Rotation = rotation;
	}
	static void TransformComponent_SetScale(uint32_t id, glm::vec2* scale)
	{
		ActorFromID(id).GetComponent<TransformComponent>().Scale = *scale;
	}

	static void TransformComponent_GetPosition(uint32_t id, glm::vec2* out)
	{
		*out = ActorFromID(id).GetComponent<TransformComponent>().Position;
	}
	static void TransformComponent_GetRotation(uint32_t id, float* out)
	{
		*out = ActorFromID(id).GetComponent<TransformComponent>().Rotation;
	}
	static void TransformComponent_GetScale(uint32_t id, glm::vec2* out)
	{
		*out = ActorFromID(id).GetComponent<TransformComponent>().Scale;
	}

	static void CameraComponent_SetSize(uint32_t id, float size)
	{
		ActorFromID(id).GetComponent<CameraComponent>().Camera.SetOrthographicSize(size);
	}
	static float CameraComponent_GetSize(uint32_t id)
	{
		return ActorFromID(id).GetComponent<CameraComponent>().Camera.GetOrthographicSize();
	}

	static void SpriteRendererComponent_SetColor(glm::vec4* color, uint32_t id)
	{
		ActorFromID(id).GetComponent<SpriteRendererComponent>().Color = *color;
	}
	static void SpriteRendererComponent_GetColor(glm::vec4* color, uint32_t id)
	{
		color = &ActorFromID(id).GetComponent<SpriteRendererComponent>().Color;
	}

	static void Actor_AddComponent(uint32_t id, MonoString* typeName)
	{
		SAND_PROFILE_FUNCTION();

		char* string = mono_string_to_utf8(typeName);

		Actor actor = ActorFromID(id);
		MonoType* type = mono_class_get_type(mono_class_from_name(s_MonoImage, "Sand", string));

		if (s_AddComponentFuncs.count(type) == 1)
			s_AddComponentFuncs[type](actor); // Add the component
		
		mono_free(string);
	}

	static bool Actor_HasComponent(uint32_t id, MonoString* typeName)
	{
		SAND_PROFILE_FUNCTION();

		char* string = mono_string_to_utf8(typeName);

		Actor actor = ActorFromID(id);
		MonoType* type = mono_class_get_type(mono_class_from_name(s_MonoImage, "Sand", string));

		bool result = false;
		if (s_HasComponentFuncs.count(type) == 1)
			result = s_HasComponentFuncs[type](actor);

		mono_free(string);

		return result;
	}

	static uint32_t Scene_GetNumberOfActors()
	{
		return s_ActiveScene->GetNumberOfActors();
	}

	static int Scene_FindActorByName(MonoString* name)
	{
		char* cstring = mono_string_to_utf8(name);

		int id = s_ActiveScene->FindActorID(std::string(cstring));
		
		mono_free(cstring);

		return id;
	}
#pragma endregion

	void ScriptEngine::SetupInternalCalls()
	{
		SAND_PROFILE_FUNCTION();

		mono_add_internal_call("Sand.Log::Info", &Log_PrintInfo);
		mono_add_internal_call("Sand.Log::Warn", &Log_PrintWarn);
		mono_add_internal_call("Sand.Log::Error", &Log_PrintError);

		mono_add_internal_call("Sand.Input::IsKeyPressed_Native", &Input_IsKeyPressed);
		mono_add_internal_call("Sand.Input::WasKeyPressed_Native", &Input_WasKeyPressed);

		mono_add_internal_call("Sand.Input::IsMousePressed_Native", &Input_IsMouseButtonPressed);
		mono_add_internal_call("Sand.Input::WasMousePressed_Native", &Input_WasMouseButtonPressed);

		mono_add_internal_call("Sand.TagComponent::SetTag_Native", &TagComponent_SetTag);
		mono_add_internal_call("Sand.TagComponent::GetTag_Native", &TagComponent_GetTag);

		mono_add_internal_call("Sand.SpriteRendererComponent::SetColor_Native", &SpriteRendererComponent_SetColor);
		mono_add_internal_call("Sand.SpriteRendererComponent::GetColor_Native", &SpriteRendererComponent_GetColor);

		mono_add_internal_call("Sand.TransformComponent::SetPosition_Native", &TransformComponent_SetPosition);
		mono_add_internal_call("Sand.TransformComponent::SetRotation_Native", &TransformComponent_SetRotation);
		mono_add_internal_call("Sand.TransformComponent::SetScale_Native", &TransformComponent_SetScale);

		mono_add_internal_call("Sand.CameraComponent::SetSize_Native", &CameraComponent_SetSize);
		mono_add_internal_call("Sand.CameraComponent::GetSize_Native", &CameraComponent_GetSize);

		mono_add_internal_call("Sand.TransformComponent::GetPosition_Native", &TransformComponent_GetPosition);
		mono_add_internal_call("Sand.TransformComponent::GetRotation_Native", &TransformComponent_GetRotation);
		mono_add_internal_call("Sand.TransformComponent::GetScale_Native", &TransformComponent_GetScale);

		mono_add_internal_call("Sand.Actor::HasComponent_Native", &Actor_HasComponent);
		mono_add_internal_call("Sand.Actor::AddComponent_Native", &Actor_AddComponent);

		mono_add_internal_call("Sand.Scene.Scene::GetNumberOfActors_Native", &Scene_GetNumberOfActors);
		mono_add_internal_call("Sand.Scene.Scene::FindActorByName_Native", &Scene_FindActorByName);
	}
	
}
