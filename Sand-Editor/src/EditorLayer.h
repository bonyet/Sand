#pragma once

#include "Sand.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Sand/Renderer/EditorCamera.h"

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

		void CheckToResizeFramebuffer();
	private:
		void SetDarkTheme();
		void SetLightTheme();
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void SaveScene();
		void SaveSceneAs();
		void OpenScene();
	private:
		Ref<Framebuffer> m_Framebuffer;
		Ref<Framebuffer> m_SecondFramebuffer;

		Ref<Scene> m_ActiveScene;

		SceneHierarchyPanel m_SceneHierarchyPanel;

		EditorCamera m_EditorCamera;

		int m_GizmoType = -1;
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0, 0 };
	};

}