#pragma once

#include "Sand/Core/Time.h"
#include "Sand/Events/ApplicationEvent.h"
#include "Sand/Core/Application.h"
#include "Sand/Renderer/EditorCamera.h"
#include "Sand/Renderer/Framebuffer.h"

#include "entt.hpp"

namespace Sand
{
	class Actor;

	class Scene
	{
	public:
		Scene() {}
		~Scene();
		
		Actor CreateActor(const std::string& name = std::string());
		void DestroyActor(Actor& entity);

		void OnViewportResize(float x, float y);

		void BeginPlay();
		void EndPlay();
		bool IsPlaying() const { return m_Playmode; }

		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts);

		Actor GetPrimaryCameraActor();
		Actor FindActor(const std::string& name);
		const uint32_t GetNumberOfActors() const { return (uint32_t)m_Registry.alive(); }
	private:
		template<typename T>
		void OnComponentAdded(Actor entity, T& component);
	private:
		entt::registry m_Registry;

		bool m_Playmode = false;
		float m_ViewportWidth = 5, m_ViewportHeight = 5;

		friend class Actor;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}