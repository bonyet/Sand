#pragma once

#include "Sand/Core/Time.h"
#include "Sand/Events/ApplicationEvent.h"
#include "Sand/Core/Application.h"
#include "Sand/Renderer/EditorCamera.h"
#include "Sand/Renderer/Framebuffer.h"

#include "entt.hpp"

namespace Sand
{
	class Entity;

	class Scene
	{
	public:
		Scene() {}
		~Scene();
		
		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity& entity);

		void OnViewportResize(float x, float y);

		void BeginPlay();
		void EndPlay();
		bool IsPlaying() const { return m_Playmode; }

		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts);
		void DrawIDBuffer(Ref<Framebuffer> idFramebuffer, EditorCamera& camera);

		Entity GetPrimaryCameraEntity();
		Entity FindEntity(const std::string& name);
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
		template<typename T>
		void OnComponentRemoved(Entity entity, T& component);
	private:
		entt::registry m_Registry;

		bool m_Playmode = false;
		float m_ViewportWidth = 5, m_ViewportHeight = 5;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}