#pragma once

#include "Sand.h"

#include "Panels/ConsolePanel.h"
#include "Panels/PropertiesPanel.h"
#include "Panels/SceneLayoutPanel.h"
#include "Panels/AssetManagerPanel.h"

#include "Sand/Renderer/Texture.h"
#include "Sand/Renderer/EditorCamera.h"

#include "Sand/Events/SceneEvent.h"

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
		void SetupGUITheme();
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnSceneEndPlay(SceneEndPlayEvent& e);
		void MousePick();

		void NewScene();
		void SaveSceneAs();
		void OpenScene();
	private:
		Ref<Framebuffer> m_ViewportFramebuffer;

		Ref<Scene> m_ActiveScene;

		SceneLayoutPanel m_SceneLayoutPanel;
		PropertiesPanel m_PropertiesPanel;
		ConsolePanel m_ConsolePanel;
		AssetManagerPanel m_AssetManagerPanel;

		EditorCamera m_EditorCamera;

		int m_GizmoType = -1;

		glm::vec2 m_ViewportBounds[2]{};
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0, 0 };
	};

}