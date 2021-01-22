#pragma once

#include "Sand.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/PropertiesPanel.h"
#include "Panels/ConsolePanel.h"
#include "Sand/Renderer/EditorCamera.h"

#include "Sand/Renderer/Texture.h"

namespace Sand
{

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnGuiRender() override;
		virtual void OnEvent(Event& e) override;
	private:
		void SetDarkTheme();
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void SaveSceneAs();
		void OpenScene();
	private:
		Ref<Framebuffer> m_ViewportFramebuffer;

		Ref<Scene> m_ActiveScene;

		SceneHierarchyPanel m_SceneHierarchyPanel;
		PropertiesPanel m_PropertiesPanel;
		ConsolePanel m_ConsolePanel;

		EditorCamera m_EditorCamera;

		int m_GizmoType = -1;

		glm::vec2 m_ViewportBounds[2]{};
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0, 0 };
	};

}