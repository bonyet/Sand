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
		bool IsPlaying() const { return mPlaymode; }

		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts);

		Actor GetPrimaryCameraActor();
		Actor FindActor(const std::string& name);
		const uint32_t GetNumberOfActors() const { return (uint32_t)mRegistry.alive(); }

		PhysicsWorld& GetPhysicsWorld() { return mPhysicsWorld; }
	private:
		template<typename T>
		void OnComponentAdded(Actor entity, T& component);
	private:
		PhysicsWorld mPhysicsWorld;
		entt::registry mRegistry;

		bool mPlaymode = false;
		float mViewportWidth = 5, mViewportHeight = 5;

		friend class Actor;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
		friend class PhysicsWorld;
	};

}