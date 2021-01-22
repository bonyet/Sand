#pragma once

#include "SceneCamera.h"
#include "ScriptableEntity.h"

#include "Sand/Debug/Profiler.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "../vendor/imgui/imgui.h"
#include <glm\gtc\type_ptr.hpp>

#include "Sand/Renderer/Renderer2D.h"

#include "Sand/Scene/Physics/PhysicsWorld.h"

namespace Sand
{

	struct ComponentBase
	{
	public:
		Entity owner{};
	};

	struct TagComponent : ComponentBase
	{
	public:
		std::string Name;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Name(tag) {}
	};

	struct TransformComponent : ComponentBase
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

	struct SpriteRendererComponent : ComponentBase
	{
	public:
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		
		SpriteRendererComponent() {}
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
	};

	struct CameraComponent : ComponentBase
	{
	public:
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(float orthoSize) { Camera.SetOrthographicSize(orthoSize); }
	};

	struct NativeScriptComponent : ComponentBase
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

	// ======= PHYSICS STUFF ========

	enum class RigidbodyType
	{
		Static,
		Kinematic,
		Dynamic,
	};

	struct Rigidbody2DComponent : ComponentBase
	{
	public:
		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const glm::vec3& scale)
		{
			m_Scale = scale;
		}
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
		
		// Getters and setters
		void SetType(RigidbodyType type) { m_Type = type; }
		RigidbodyType GetType() { return m_Type; }

		void SetScale(const glm::vec2& scale)
		{
			m_Scale = scale;

			if (m_Body)
				((b2PolygonShape*)m_Body->GetFixtureList()->GetShape())->SetAsBox(scale.x, scale.y);
		}

		glm::vec2 GetScale() const
		{
			return m_Scale;
		}
		glm::vec3 GetPosition() const
		{
			auto pos = m_Body->GetPosition();
			return { pos.x, pos.y, 0.0f };
		}
		float GetRotationRadians() const
		{
			return m_Body->GetAngle();
		}

		void SetGravityScale(float gravScale)
		{
			m_GravityScale = gravScale;

			if (m_Body)
				m_Body->SetGravityScale(gravScale);
		}
		float GetGravityScale() const { return m_GravityScale; }

		void SetFriction(float friction)
		{
			m_Friction = friction;

			if (m_Body)
				m_Body->GetFixtureList()->SetFriction(friction);
		}
		float GetFriction() const { return m_Friction;  }

		void SetRestitution(float restitution)
		{
			m_Restitution = restitution;

			if (m_Body)
				m_Body->GetFixtureList()->SetRestitution(restitution);
		}
		float GetRestitution() const { return m_Restitution; }

		void ApplyForce(const glm::vec2& force, bool wake)
		{
			m_Body->ApplyForceToCenter({ force.x, force.y }, wake);
		}
		void ApplyImpulse(const glm::vec2& impulse, bool wake)
		{
			m_Body->ApplyLinearImpulseToCenter({ impulse.x, impulse.y }, wake);
		}
		void ApplyAngularImpulse(float impulse, bool wake)
		{
			m_Body->ApplyAngularImpulse(impulse, wake);
		}
	private:
		void Create()
		{
			auto& tc = owner.GetComponent<TransformComponent>();
			auto& position = tc.Position;
			float rotation = tc.Rotation.z;
			m_Scale = tc.Scale;

			b2BodyDef bodyDef;
			bodyDef.type = (b2BodyType)m_Type;
			bodyDef.position.Set(position.x, position.y);
			bodyDef.enabled = true;
			bodyDef.gravityScale = m_GravityScale;
			bodyDef.angle = rotation;

			m_Body = PhysicsWorld::CreateBody(&bodyDef);

			b2PolygonShape boxShape;
			boxShape.SetAsBox(m_Scale.x / 2.0f, m_Scale.y / 2.0f);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxShape;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.0f;
			fixtureDef.restitution = 0.0f;

			m_Body->CreateFixture(&fixtureDef);
		}
		void Destroy()
		{
			PhysicsWorld::DestroyBody(m_Body);
			m_Body = nullptr;
		}

		void UpdateTransform()
		{
			auto& transform = owner.GetComponent<TransformComponent>();
			transform.Position = GetPosition();
			transform.Rotation.z = GetRotationRadians();
		}
	private:
		b2Body* m_Body = nullptr;
		RigidbodyType m_Type = RigidbodyType::Static;

		float m_Friction = 0.0f, m_GravityScale = 1.0f, m_Restitution = 0.0f;
		glm::vec2 m_Scale{ 0.0f };

		friend class Scene;
	};

}