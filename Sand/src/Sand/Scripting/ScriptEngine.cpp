#include "sandpch.h"
#include "ScriptCalls.h" // Includes ScriptEngine.h

#include "Sand/Debug/Debug.h"

#include <vector>

#include <mono/metadata/attrdefs.h>
#include <mono/metadata/assembly.h>

namespace Sand
{
	using namespace ScriptCalls;

	static MonoDomain* s_MonoDomain = nullptr;
	static MonoImage* s_MonoImage = nullptr;
	static MonoAssembly* s_MonoAssembly = nullptr;

	static MonoAssembly* s_ClientMonoAssembly = nullptr;
	static MonoImage* s_ClientMonoImage = nullptr;

	static const std::string CLIENT_NAMESPACE = "Client";

	static std::unordered_map<uint32_t, ScriptData> s_ScriptDataMap;
	static std::vector<std::string> s_CachedClientScriptNames;

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

		// Register all the core components
		{
			RegisterComponent(TagComponent);
			RegisterComponent(CameraComponent);
			RegisterComponent(PhysicsComponent);
			RegisterComponent(TransformComponent);
			RegisterComponent(BoxColliderComponent);
			RegisterComponent(SpriteRendererComponent);
		}

		LoadClientAssembly();
		RegisterInternalCalls();
	}
#undef RegisterComponent
	
	void ScriptEngine::Shutdown()
	{
		mono_jit_cleanup(s_MonoDomain);
	}

	MonoImage* ScriptEngine::GetMonoImage()
	{
		return s_MonoImage;
	}

	// Forward decl for LoadClientAssembly
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

		// Get the names of all the classes and cache them in a vector
		{
			std::vector<MonoClass*> clientClasses = FindAllClasses(s_ClientMonoImage);
			s_ScriptDataMap.reserve(clientClasses.size());
			for (auto klass : clientClasses)
			{
				s_CachedClientScriptNames.emplace_back(mono_class_get_name(klass));
			}
		}

	}
	bool ScriptEngine::IsModuleRegistered(uint32_t actorID)
	{
		return s_ScriptDataMap.count(actorID);
	}

	// == Module things ==
	void ScriptEngine::RegisterModule(uint32_t actorID, const std::string& moduleName)
	{
		SAND_PROFILE_FUNCTION();

		MonoClass* klass = mono_class_from_name(s_ClientMonoImage, CLIENT_NAMESPACE.c_str(), moduleName.c_str());
		if (!klass)
			return; // Class doesn't exist

		// Get all the engine-specific methods we need
		// mono_class_get_method_from_name() will just return NULL if it couldn't find it (instead of crashing), so it works out fine.
		MonoMethod* onLoadMethod = mono_class_get_method_from_name(klass, "OnLoad", 0);
		MonoMethod* onCreateMethod = mono_class_get_method_from_name(klass, "OnCreate", 0);
		MonoMethod* onUpdateMethod = mono_class_get_method_from_name(klass, "OnUpdate", 1);
		MonoMethod* onLateUpdateMethod = mono_class_get_method_from_name(klass, "OnLateUpdate", 1);
		MonoMethod* onDestroyMethod = mono_class_get_method_from_name(klass, "OnDestroy", 0);

		// Create an instance of the script object
		MonoObject* object = mono_object_new(s_MonoDomain, klass);

		ScriptData data = {
			object, klass, onLoadMethod, onCreateMethod, onUpdateMethod, onLateUpdateMethod, onDestroyMethod
		};

		// Get all the fields in the MonoClass
		{
			SAND_PROFILE_SCOPE("Getting script fields - ScriptEngine::RegisterModule(actorID, moduleName)");

			MonoClassField* field;
			void* iterator = NULL;
			while (field = mono_class_get_fields(klass, &iterator))
			{
				data.Fields.emplace_back(ScriptField{ MonoTypeToScriptDataType(mono_field_get_type(field)), field });
			}
		}

		s_ScriptDataMap.emplace(actorID, data);
	}
	void ScriptEngine::UnregisterModule(uint32_t actorID)
	{
		if (!s_ScriptDataMap.count(actorID))
			return; // Early exit if the actor doesn't have a script associated with it

		const ScriptData& scriptData = s_ScriptDataMap[actorID];
		
		// Call appropriate destruction function(s)
		if (scriptData.OnDestroyMethod)
			mono_runtime_invoke(scriptData.OnDestroyMethod, scriptData.Object, nullptr, nullptr);

		s_ScriptDataMap.erase(actorID);
	}
	// == End module things ==

	// Returns all the MonoClasses in a MonoImage
	static std::vector<MonoClass*> FindAllClasses(MonoImage* image)
	{
		SAND_PROFILE_FUNCTION();

		const MonoTableInfo* tableInfo = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);

		int rows = mono_table_info_get_rows(tableInfo);

		std::vector<MonoClass*> classes;
		classes.reserve(rows);

		// Get values from each row
		for (int i = 0; i < rows; i++)
		{
			MonoClass* klass = nullptr;
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(tableInfo, i, cols, MONO_TYPEDEF_SIZE);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			klass = mono_class_from_name(image, nameSpace, name);

			if (strcmp(name, "<Module>") == 0)
				continue; // for some reason <Module> always shows up as a class... idk, it just works.
			
			classes.push_back(klass);
		}

		return classes;
	}

	std::vector<std::string>& ScriptEngine::GetClientScriptNames()
	{
		return s_CachedClientScriptNames;
	}

	ScriptData& ScriptEngine::GetScriptDataFromActor(uint32_t actorID)
	{
		return s_ScriptDataMap[actorID];
	}

	ScriptFieldType ScriptEngine::MonoTypeToScriptDataType(MonoType* monoType)
	{
		int type = mono_type_get_type(monoType);
		switch (type)
		{
			case MONO_TYPE_I2:
			case MONO_TYPE_I4:
			case MONO_TYPE_I8:
				return ScriptFieldType::Int;
			case MONO_TYPE_U2:
			case MONO_TYPE_U4:
			case MONO_TYPE_U8:
				return ScriptFieldType::UInt;
			case MONO_TYPE_R4:
				return ScriptFieldType::Float;
			case MONO_TYPE_R8:
				return ScriptFieldType::Double;
			case MONO_TYPE_VALUETYPE:
			{
				const char* name = mono_type_get_name((MonoType*)monoType);

				if (strcmp(name, "Sand.Math.Vector2") == 0)
					return ScriptFieldType::Vector2;
				if (strcmp(name, "Sand.Math.Vector3") == 0)
					return ScriptFieldType::Vector3;
				if (strcmp(name, "Sand.Math.Vector4") == 0)
					return ScriptFieldType::Vector4;
				if (strcmp(name, "Sand.Color") == 0)
					return ScriptFieldType::Color;

				break;
			}
		}
		
		return ScriptFieldType::Unknown;
	}

	bool ScriptEngine::MonoFieldIsPublic(MonoClassField* field)
	{
		uint32_t flags = mono_field_get_flags(field);
		return (flags & MONO_FIELD_ATTR_PUBLIC) != 0;
	}

	void ScriptEngine::RegisterInternalCalls()
	{
		SAND_PROFILE_FUNCTION();

		// Logging
		mono_add_internal_call("Sand.Log::Info", &Log_PrintInfo);
		mono_add_internal_call("Sand.Log::Warn", &Log_PrintWarn);
		mono_add_internal_call("Sand.Log::Error", &Log_PrintError);

		// Input
		mono_add_internal_call("Sand.Input::IsKeyPressed_Native", &Input_IsKeyPressed);
		mono_add_internal_call("Sand.Input::WasKeyPressed_Native", &Input_WasKeyPressed);

		mono_add_internal_call("Sand.Input::IsMousePressed_Native", &Input_IsMouseButtonPressed);
		mono_add_internal_call("Sand.Input::WasMousePressed_Native", &Input_WasMouseButtonPressed);

		// Tag component
		mono_add_internal_call("Sand.TagComponent::SetTag_Native", &TagComponent_SetTag);
		mono_add_internal_call("Sand.TagComponent::GetTag_Native", &TagComponent_GetTag);

		// Transform component
		mono_add_internal_call("Sand.TransformComponent::SetPosition_Native", &TransformComponent_SetPosition);
		mono_add_internal_call("Sand.TransformComponent::SetRotation_Native", &TransformComponent_SetRotation);
		mono_add_internal_call("Sand.TransformComponent::SetScale_Native", &TransformComponent_SetScale);

		mono_add_internal_call("Sand.TransformComponent::GetPosition_Native", &TransformComponent_GetPosition);
		mono_add_internal_call("Sand.TransformComponent::GetRotation_Native", &TransformComponent_GetRotation);
		mono_add_internal_call("Sand.TransformComponent::GetScale_Native", &TransformComponent_GetScale);

		// Sprite renderer component
		mono_add_internal_call("Sand.SpriteRendererComponent::SetColor_Native", &SpriteRendererComponent_SetColor);
		mono_add_internal_call("Sand.SpriteRendererComponent::GetColor_Native", &SpriteRendererComponent_GetColor);

		// Camera component
		mono_add_internal_call("Sand.CameraComponent::SetSize_Native", &CameraComponent_SetSize);
		mono_add_internal_call("Sand.CameraComponent::GetSize_Native", &CameraComponent_GetSize);

		// Physics component
		mono_add_internal_call("Sand.PhysicsComponent::SetType_Native", &PhysicsComponent_SetType);
		mono_add_internal_call("Sand.PhysicsComponent::GetType_Native", &PhysicsComponent_GetType);
		mono_add_internal_call("Sand.PhysicsComponent::SetMass_Native", &PhysicsComponent_SetMass);
		mono_add_internal_call("Sand.PhysicsComponent::GetMass_Native", &PhysicsComponent_GetMass);
		mono_add_internal_call("Sand.PhysicsComponent::SetGravityScale_Native", &PhysicsComponent_SetGravityScale);
		mono_add_internal_call("Sand.PhysicsComponent::GetGravityScale_Native", &PhysicsComponent_GetGravityScale);
		mono_add_internal_call("Sand.PhysicsComponent::SetFixedRotation_Native", &PhysicsComponent_SetFixedRotation);
		mono_add_internal_call("Sand.PhysicsComponent::GetFixedRotation_Native", &PhysicsComponent_GetFixedRotation);
		mono_add_internal_call("Sand.PhysicsComponent::ApplyLinearImpulse_Native", &PhysicsComponent_ApplyLinearImpulse);
		mono_add_internal_call("Sand.PhysicsComponent::ApplyForce_Native", &PhysicsComponent_ApplyForce);

		// Box collider component
		mono_add_internal_call("Sand.BoxColliderComponent::SetScale_Native", &BoxColliderComponent_SetScale);
		mono_add_internal_call("Sand.BoxColliderComponent::GetScale_Native", &BoxColliderComponent_GetScale);
		mono_add_internal_call("Sand.BoxColliderComponent::SetRestitution_Native", &BoxColliderComponent_SetRestitution);
		mono_add_internal_call("Sand.BoxColliderComponent::GetRestitution_Native", &BoxColliderComponent_GetRestitution);
		mono_add_internal_call("Sand.BoxColliderComponent::SetObserver_Native", &BoxColliderComponent_SetObserver);
		mono_add_internal_call("Sand.BoxColliderComponent::IsObserver_Native", &BoxColliderComponent_IsObserver);
		mono_add_internal_call("Sand.BoxColliderComponent::SetFriction_Native", &BoxColliderComponent_SetFriction);
		mono_add_internal_call("Sand.BoxColliderComponent::GetFriction_Native", &BoxColliderComponent_GetFriction);

		// Actor
		mono_add_internal_call("Sand.Actor::HasComponent_Native", &Actor_HasComponent);
		mono_add_internal_call("Sand.Actor::AddComponent_Native", &Actor_AddComponent);

		// Scene
		mono_add_internal_call("Sand.Scene.Scene::GetNumberOfActors_Native", &Scene_GetNumberOfActors);
		mono_add_internal_call("Sand.Scene.Scene::FindActorByName_Native", &Scene_FindActorByName);
	}
	
}