#pragma once

#include "SceneCamera.h"
#include "ScriptableEntity.h"

#include "Sand/Debug/Profiler.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "../vendor/imgui/imgui.h"
#include <glm\gtc\type_ptr.hpp>

#include "Sand/Renderer/Renderer2D.h"

namespace Sand
{

	struct TagComponent
	{
	public:
		std::string Name;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Name(tag) {}
	};

	struct TransformComponent
	{
	private:
		glm::mat4 Transform{ glm::mat4(1.0f) };
	public:
		glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation{ 0.0f };
		glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& pos, const glm::vec3& size, const glm::vec3& rot)
			: Position(pos), Scale(size), Rotation(rot) {}
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {}

		glm::mat4& GetTransform() 
		{
			SAND_PROFILE_FUNCTION();

			Transform = glm::translate(glm::mat4(1.0f), Position)
				* glm::toMat4(glm::quat(Rotation))
				* glm::scale(glm::mat4(1.0f), Scale);

			return Transform;
		}

		operator const glm::mat4& () { return GetTransform(); }
	};

	struct SpriteRendererComponent
	{
	public:
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		
		SpriteRendererComponent() {}
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
	};

	struct CameraComponent
	{
	public:
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(float size) { Camera.SetOrthographicSize(size); }
	};

	struct NativeScriptComponent
	{
	public:
		ScriptableEntity* Instance = nullptr;
	
		ScriptableEntity*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

}