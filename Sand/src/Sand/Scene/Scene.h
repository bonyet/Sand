#pragma once

#include "entt.hpp"
#include "Sand/Core/Time.h"

#include "Sand/Events/ApplicationEvent.h"

#include "Sand/Core/Application.h"
#include "Sand/Renderer/EditorCamera.h"

namespace Sand
{
	class Entity;

	class Scene
	{
	public:
		Scene(const std::string& name = std::string())
			: m_Name(name) { }
		~Scene();
		
		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity& entity);

		void OnViewportResize(float x, float y);

		const std::string& GetName() const { return m_Name; }
		void SetName(const std::string& name) { m_Name = name; }

		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts);

		Entity GetPrimaryCameraEntity();

		static Ref<Scene>& GetActiveScene() { return Application::Get().GetActiveScene(); }
		static void SetActiveScene(const Ref<Scene>& scene) { return Application::Get().SetActiveScene(scene); }
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;

		std::string m_Name;
		float m_ViewportWidth = 5, m_ViewportHeight = 5;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}