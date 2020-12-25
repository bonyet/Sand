#pragma once

#include "Sand/Core/Log.h"

#include "Sand/Scene/Scene.h"
#include "Sand/Scene/Entity.h"

#include "EditorPanel.h"

namespace Sand
{

	class SceneHierarchyPanel : public EditorPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);
		Entity GetSelectedEntity() const { return m_SelectionContext; }

		virtual void OnGuiRender() override;

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
		void DrawEntityCreationMenu();
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};

}