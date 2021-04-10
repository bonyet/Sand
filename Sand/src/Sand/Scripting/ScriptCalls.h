#pragma once

#include "ScriptEngine.h"

#include "Sand/Core/Log.h"
#include "Sand/Core/Input.h"
#include "Sand/Scene/Components.h"

#include <mono/jit/jit.h>

namespace Sand { namespace ScriptCalls {

	using HasComponentFunc = bool(*)(Actor& actor);
	static std::unordered_map<MonoType*, HasComponentFunc> s_HasComponentFuncs = {};
	using AddComponentFunc = void(*)(Actor& actor);
	static std::unordered_map<MonoType*, AddComponentFunc> s_AddComponentFuncs = {};

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
		return mono_string_new(mono_domain_get(), ActorFromID(id).GetComponent<TagComponent>().Name.c_str());
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
		MonoType* type = mono_class_get_type(mono_class_from_name(ScriptEngine::GetMonoImage(), "Sand", string));

		if (s_AddComponentFuncs.count(type) == 1)
			s_AddComponentFuncs[type](actor); // Add the component

		mono_free(string);
	}

	static bool Actor_HasComponent(uint32_t id, MonoString* typeName)
	{
		SAND_PROFILE_FUNCTION();

		char* string = mono_string_to_utf8(typeName);

		Actor actor = ActorFromID(id);
		MonoType* type = mono_class_get_type(mono_class_from_name(ScriptEngine::GetMonoImage(), "Sand", string));

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

}}