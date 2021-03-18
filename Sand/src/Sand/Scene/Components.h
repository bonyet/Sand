#pragma once

#include "Sand/Debug/Profiler.h"
#include "Sand/Renderer/Texture.h"
#include "Sand/Physics/PhysicsBody.h"
#include "Sand/Animation/Animator.h"

#include "SceneCamera.h"
#include "ScriptableActor.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Sand
{

	struct TagComponent
	{
		Actor owner{};
	public:
		std::string Name;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Name(tag) {}
	};

	struct TransformComponent
	{
		Actor owner{};
	public:
		glm::vec2 Position{ 0.0f, 0.0f };
		float Rotation = 0.0f;
		glm::vec2 Scale{ 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec2& pos, const glm::vec2& size, float rot)
			: Position(pos), Scale(size), Rotation(rot) {}

		glm::mat4 GetTransform() 
		{
			SAND_PROFILE_FUNCTION();

			return glm::translate(glm::mat4(1.0f), glm::vec3(Position, 0.0f))
				* glm::rotate(glm::mat4(1.0f), Rotation, { 0, 0, 1 })
				* glm::scale(glm::mat4(1.0f), glm::vec3(Scale, 1.0f));
		}

		operator const glm::mat4 () { return GetTransform(); }
	public:
		void Reset()
		{
			Position = { 0.0f, 0.0f };
			Scale = { 1.0f, 1.0f };
			Rotation = 0.0f;
		}
	};

	struct TextureComponent
	{
		Actor owner{};
	public:
		Ref<Texture2D> Texture = nullptr;
		float TilingFactor = 1;

		inline bool IsTextured() const { return Texture != nullptr; }

		TextureComponent() {}
		TextureComponent(const TextureComponent&) = default;

		void Reset()
		{
			Texture.reset();
			TilingFactor = 1;
		}
	};

	struct SpriteRendererComponent
	{
		Actor owner{};
	public:
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() {}
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
	public:
		void Reset()
		{
			Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		}
	};

	struct CameraComponent
	{
		Actor owner{};
	public:
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(float orthoSize) { Camera.SetOrthographicSize(orthoSize); }
	public:
		void Reset()
		{
			Camera.SetOrthographicFarClip(1.0f);
			Camera.SetOrthographicNearClip(-1.0f);			
			Camera.SetOrthographicSize(10.0f);
		}
	};

	struct NativeScriptComponent
	{
		Actor owner{};
	public:
		ScriptableActor* Instance = nullptr;
	
		ScriptableActor*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableActor*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

	struct ScriptComponent
	{
		Actor owner{};
	public:
		std::string ModuleName;

		ScriptComponent() = default;
		ScriptComponent(const std::string& moduleName) : ModuleName(moduleName) {}
		ScriptComponent(const ScriptComponent&) = default;

		void Activate();
		void Deactivate();

		void Reset()
		{
			ModuleName = "";
		}
	};

	struct PhysicsComponent
	{
		Actor owner{};
	public:
		PhysicsBody Body;

		PhysicsComponent() = default;
		PhysicsComponent(const PhysicsComponent&) = default;

		void Reset() {}
	};

	struct AnimatorComponent
	{
		Actor owner;
	public:
		Animator Animator;

		AnimatorComponent() = default;
		AnimatorComponent(const AnimatorComponent&) = default;

		void Reset() {}
	};

}