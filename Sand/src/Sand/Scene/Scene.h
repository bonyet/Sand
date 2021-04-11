#pragma once

#include "Sand/Core/Time.h"
#include "Sand/Events/ApplicationEvent.h"
#include "Sand/Core/Application.h"
#include "Sand/Renderer/EditorCamera.h"
#include "Sand/Renderer/Framebuffer.h"

#include "entt.hpp"

#include "Sand/Physics/PhysicsWorld.h"

namespace Sand
{
	class Actor;

	class Scene
	{
	public:
		Scene();
		~Scene();
		
		Actor CreateActor(const std::string& name = std::string());
		void DestroyActor(Actor& entity);

		void OnViewportResize(float x, float y);

		void BeginPlay();
		void EndPlay();
		bool IsPlaying() const { return m_Playmode; }

		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts);

		Actor FindActor(const std::string& name);
		int FindActorID(const std::string& name);

		Actor DuplicateActor(Actor original);

		Actor GetPrimaryCameraActor();
		PhysicsWorld& GetPhysicsWorld() { return m_PhysicsWorld; }
		const uint32_t GetNumberOfActors() const { return (uint32_t)m_Registry.alive(); }

		bool ContainsActor(Actor actor);
		bool IsActorIDValid(uint32_t id);

		static Scene* const GetActiveScene();
	private:
		template<typename T>
		void OnComponentAdded(Actor actor, T& component);
		template<typename T>
		void OnComponentRemoved(Actor actor, T& component);

		void SwitchToRuntimeRegistry();
		void SwitchToEditorRegistry();

		void OnActorCreate(Actor actor);
		void OnActorDestroy(Actor actor);
	private:
		PhysicsWorld m_PhysicsWorld;
		entt::registry m_Registry;
		entt::registry m_RuntimeRegistry;
		entt::registry* m_CurrentRegistry = nullptr;
		
		bool m_Playmode = false;
		float m_ViewportWidth = 5, m_ViewportHeight = 5;

		friend class Actor;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
		friend class PhysicsWorld;
	};

}