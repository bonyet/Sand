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
		RegisterComponent(PhysicsComponent);
		RegisterComponent(TransformComponent);
		RegisterComponent(BoxColliderComponent);
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

				break;
			}
			default:
			{
				return ScriptDataType::Unknown;
			}
		}
	}

	bool ScriptEngine::MonoFieldIsPublic(void* field)
	{
		uint32_t flags = mono_field_get_flags((MonoClassField*)field);
		return (flags & MONO_FIELD_ATTR_PUBLIC) != 0;
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
	static inline Actor ActorFromID(uint32_t id)
	{
		return { (entt::entity)id, Scene::GetActiveScene() };
	}

	// ===== TAG =====
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

	// ===== TRANSFORM =====
	static void TransformComponent_SetPosition(uint32_t id, glm::vec2* position)
	{
		ActorFromID(id).GetComponent<TransformComponent>().SetPosition(*position);
	}
	static void TransformComponent_SetRotation(uint32_t id, float rotation)
	{
		ActorFromID(id).GetComponent<TransformComponent>().SetRotation(rotation);
	}
	static void TransformComponent_SetScale(uint32_t id, glm::vec2* scale)
	{
		ActorFromID(id).GetComponent<TransformComponent>().SetScale(*scale);
	}

	static void TransformComponent_GetPosition(uint32_t id, glm::vec2* out)
	{
		*out = ActorFromID(id).GetComponent<TransformComponent>().GetPosition();
	}
	static void TransformComponent_GetRotation(uint32_t id, float* out)
	{
		*out = ActorFromID(id).GetComponent<TransformComponent>().GetRotation();
	}
	static void TransformComponent_GetScale(uint32_t id, glm::vec2* out)
	{
		*out = ActorFromID(id).GetComponent<TransformComponent>().GetScale();
	}

	// ===== CAMERA =====
	static void CameraComponent_SetSize(uint32_t id, float size)
	{
		ActorFromID(id).GetComponent<CameraComponent>().Camera.SetSize(size);
	}
	static float CameraComponent_GetSize(uint32_t id)
	{
		return ActorFromID(id).GetComponent<CameraComponent>().Camera.GetSize();
	}

	// ==== SPRITE RENDERER =====
	static void SpriteRendererComponent_SetColor(glm::vec4* color, uint32_t id)
	{
		ActorFromID(id).GetComponent<SpriteRendererComponent>().Color = *color;
	}
	static void SpriteRendererComponent_GetColor(glm::vec4* color, uint32_t id)
	{
		color = &ActorFromID(id).GetComponent<SpriteRendererComponent>().Color;
	}

	// ===== PHYSICS =====
	static void PhysicsComponent_SetType(uint32_t id, int type)
	{
		ActorFromID(id).GetComponent<PhysicsComponent>().Body.SetType((PhysicsBodyType)type);
	}
	static int PhysicsComponent_GetType(uint32_t id)
	{
		return (int)ActorFromID(id).GetComponent<PhysicsComponent>().Body.GetType();
	}
	static void PhysicsComponent_SetMass(uint32_t id, float mass)
	{
		ActorFromID(id).GetComponent<PhysicsComponent>().Body.SetMass(mass);
	}
	static float PhysicsComponent_GetMass(uint32_t id)
	{
		return ActorFromID(id).GetComponent<PhysicsComponent>().Body.GetMass();
	}
	static void PhysicsComponent_SetGravityScale(uint32_t id, float gravity)
	{
		ActorFromID(id).GetComponent<PhysicsComponent>().Body.SetGravityScale(gravity);
	}
	static float PhysicsComponent_GetGravityScale(uint32_t id)
	{
		return ActorFromID(id).GetComponent<PhysicsComponent>().Body.GetGravityScale();
	}
	static void PhysicsComponent_SetFixedRotation(uint32_t id, bool fixed)
	{
		ActorFromID(id).GetComponent<PhysicsComponent>().Body.SetFixedRotation(fixed);
	}
	static bool PhysicsComponent_GetFixedRotation(uint32_t id)
	{
		return ActorFromID(id).GetComponent<PhysicsComponent>().Body.GetFixedRotation();
	}
	static void PhysicsComponent_ApplyLinearImpulse(uint32_t id, glm::vec2* impulse, bool wake)
	{
		ActorFromID(id).GetComponent<PhysicsComponent>().Body.ApplyLinearImpulse(*impulse, wake);
	}
	static void PhysicsComponent_ApplyForce(uint32_t id, glm::vec2* force, bool wake)
	{
		ActorFromID(id).GetComponent<PhysicsComponent>().Body.ApplyForce(*force, wake);
	}

	// ===== BOX COLLIDER =====
	static void BoxColliderComponent_SetScale(uint32_t id, glm::vec2* scale)
	{
		ActorFromID(id).GetComponent<BoxColliderComponent>().SetScale(*scale);
	}
	static void BoxColliderComponent_GetScale(uint32_t id, glm::vec2* outScale)
	{
		*outScale = ActorFromID(id).GetComponent<BoxColliderComponent>().GetScale();
	}
	static void BoxColliderComponent_SetRestitution(uint32_t id, float restitution)
	{
		ActorFromID(id).GetComponent<BoxColliderComponent>().SetRestitution(restitution);
	}
	static void BoxColliderComponent_SetFriction(uint32_t id, float friction)
	{
		ActorFromID(id).GetComponent<BoxColliderComponent>().SetFriction(friction);
	}
	static float BoxColliderComponent_GetFriction(uint32_t id)
	{
		return ActorFromID(id).GetComponent<BoxColliderComponent>().GetFriction();
	}
	static float BoxColliderComponent_GetRestitution(uint32_t id)
	{
		return ActorFromID(id).GetComponent<BoxColliderComponent>().GetRestitution();
	}
	static void BoxColliderComponent_SetObserver(uint32_t id, bool flag)
	{
		ActorFromID(id).GetComponent<BoxColliderComponent>().SetObserver(flag);
	}
	static bool BoxColliderComponent_IsObserver(uint32_t id)
	{
		return ActorFromID(id).GetComponent<BoxColliderComponent>().IsObserver();
	}

	// ===== ACTOR =====
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

	// ===== SCENE =====
	static uint32_t Scene_GetNumberOfActors()
	{
		return Scene::GetActiveScene()->GetNumberOfActors();
	}

	static int Scene_FindActorByName(MonoString* name)
	{
		char* cstring = mono_string_to_utf8(name);

		int id = Scene::GetActiveScene()->FindActorID(std::string(cstring));
		
		mono_free(cstring);

		return id;
	}
#pragma endregion

	void ScriptEngine::SetupInternalCalls()
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