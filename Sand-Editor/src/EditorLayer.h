#pragma once

#include "Sand.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/AssetManagerPanel.h"

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
		void SetupImGuiStyle();
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
		AssetManagerPanel m_AssetManagerPanel;

		int m_GizmoType = -1;
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0, 0 };
	};

}