#pragma once

#include "Sand/Debug/Profiler.h"
#include "Sand/Renderer/Texture.h"
#include "Sand/Physics/PhysicsBody.h"

#include "SceneCamera.h"
#include "ScriptableActor.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/type_ptr.hpp>

#include "Sand/Audio/Audio.h"
#include <vector>

namespace Sand
{

	struct TagComponent
	{
		Actor owner{};
	public:
		std::string Name;

		TagComponent() = default;
		TagComponent(const std::string& tag)
			: Name(tag) {}
	};

	struct TransformComponent
	{
		Actor owner{};
	private:
		glm::vec2 m_Position{ 0.0f, 0.0f };
		float m_Rotation = 0.0f;
		glm::vec2 m_Scale{ 1.0f, 1.0f };

		Actor m_Parent = {};
		bool m_Dirty = true;
		std::vector<Actor> m_Children = {};
	public:
		TransformComponent() = default;
		TransformComponent(const glm::vec2& pos, const glm::vec2& size, float rot)
			: m_Position(pos), m_Scale(size), m_Rotation(rot) {}

		void SetPosition(const glm::vec2& position) 
		{
			m_Position = position;
			m_Dirty = true;
		}
		void SetScale(const glm::vec2& scale) 
		{ 
			m_Scale = scale;
			m_Dirty = true;
		}
		void SetRotation(float rotation) 
		{
			m_Rotation = rotation; 
			m_Dirty = true; 
		}

		// Children
		std::vector<Actor>& GetChildren() { return m_Children; }
		bool HasChildren() const { return m_Children.size() != 0; }
		void RemoveChild(Actor child) { m_Children.erase(std::find(m_Children.begin(), m_Children.end(), child)); }
		bool HasChild(Actor child) { return std::find(m_Children.begin(), m_Children.end(), child) != m_Children.end(); }
		void AddChild(Actor child) 
		{ 
			if (HasChild(child))
				return; 

			m_Children.push_back(child); 
		}

		// Transformation
		const glm::vec2& GetPosition() const { return m_Position; }
		const glm::vec2& GetScale() const { return m_Scale; }
		float GetRotation() const { return m_Rotation; }

		// Parenting
		bool HasParent() const { return m_Parent; }
		Actor GetParent() const { return m_Parent; }
		TransformComponent& GetParentTransform() { return m_Parent.GetComponent<TransformComponent>(); }
		void SetParent(Actor parent) 
		{ 
			// Remove us from the existing parents children if we reset the parent
			if (!parent && m_Parent)
				m_Parent.GetComponent<TransformComponent>().RemoveChild(owner);

			m_Parent = parent; 

			// Add us to the parent's children
			if (parent)
				parent.GetComponent<TransformComponent>().AddChild(owner); 
		}

		glm::mat4 GetGlobalTransform()
		{
			SAND_PROFILE_FUNCTION();

			return glm::translate(glm::mat4(1.0f), glm::vec3(m_Position, 0.0f))
				* glm::rotate(glm::mat4(1.0f), m_Rotation, { 0, 0, 1 })
				* glm::scale(glm::mat4(1.0f), glm::vec3(m_Scale, 1.0f));
		}

		glm::mat4 GetTransform() 
		{
			SAND_PROFILE_FUNCTION();
			
			// Return global transform if we are an orphan
			if (!m_Parent)
				return GetGlobalTransform();

			return m_Parent.GetComponent<TransformComponent>().GetGlobalTransform() * glm::translate(glm::mat4(1.0f), glm::vec3(m_Position, 0.0f))
				* glm::rotate(glm::mat4(1.0f), m_Rotation, { 0, 0, 1 })
				* glm::scale(glm::mat4(1.0f), glm::vec3(m_Scale, 1.0f));
		}

		operator const glm::mat4 () { return GetTransform(); }
	public:
		void Reset()
		{
			m_Position = { 0.0f, 0.0f };
			m_Scale = { 1.0f, 1.0f };
			m_Rotation = 0.0f;
		}
	};

	struct TextureComponent
	{
		Actor owner{};
	public:
		Ref<Texture2D> Texture = nullptr;
		float TilingFactor = 1;

		bool IsTextured() const { return Texture != nullptr; }

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

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
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
		CameraComponent(float orthoSize) { Camera.SetSize(orthoSize); }
	public:
		void Reset()
		{
			Camera.SetFarClip(1.0f);
			Camera.SetNearClip(-1.0f);			
			Camera.SetSize(10.0f);
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

	struct BoxColliderComponent
	{
		Actor owner{};
	private:
		glm::vec2 m_Scale = { 1.0f, 1.0f };
		bool m_Observer = false;
		float m_Restitution = 0.0f, m_Friction = 0.3f;
	public:
		b2Body* Body = nullptr;
		void* Fixture = nullptr;

		void Create(const glm::vec2& scale, b2Body* const body);
		void Destroy();

		void SetScale(const glm::vec2& scale);
		glm::vec2 GetScale() const;

		void SetObserver(bool flag);
		bool IsObserver() const;

		void SetRestitution(float restitution);
		float GetRestitution() const;

		void SetFriction(float friction);
		float GetFriction() const;

		void Reset()
		{
		}
	};

	struct PhysicsComponent
	{
		Actor owner{};
	public:
		PhysicsBody Body;
		PhysicsBodyDescription Description =
		{
			{ 0.0f, 0.0f },
			{ 0.0f, 0.0f },
			0.0f
		};

		void Reset() {}
	};

	struct AnimatorComponent
	{
		Actor owner;
	public:
		void Reset() {}
	};

	struct AudioSourceComponent
	{
		Actor owner;
	public:
		AudioSource Source;
		AudioClip Clip;

		void Init()
		{
			Source = Audio::LoadAudioSource(Clip);
		}

		void Reset()
		{
			Source.SetGain(1.0f);
			Source.SetPitch(1.0f);
			Source.SetSpatial(false);
		};
	};

	struct ScriptComponent
	{
		Actor owner{};
	public:
		std::string ModuleName;

		ScriptComponent() = default;
		ScriptComponent(const std::string& moduleName)
			: ModuleName(moduleName) { }

		void Init();
		void Uninit();

		void Reset()
		{
			ModuleName.clear();
		}
	};

}