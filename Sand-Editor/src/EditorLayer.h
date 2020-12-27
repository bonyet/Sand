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
		void SetLightTheme();
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		Entity GetHoveredEntity();

		void NewScene();
		void SaveScene();
		void SaveSceneAs();
		void OpenScene();
	private:
		Ref<Framebuffer> m_Framebuffer;
		Ref<Framebuffer> m_IDFramebuffer;

		Ref<Scene> m_ActiveScene;

		SceneHierarchyPanel m_SceneHierarchyPanel;
		PropertiesPanel m_PropertiesPanel;
		ConsolePanel m_ConsolePanel;

		EditorCamera m_EditorCamera;
		Entity m_HoveredEntity;

		int m_GizmoType = -1;
		const std::array<Ref<Texture2D>, 4> m_GizmoImages = {
			Texture2D::Create("assets/textures/mouse.png"),
			Texture2D::Create("assets/textures/translate.png"),
			Texture2D::Create("assets/textures/rotate.png"),
			Texture2D::Create("assets/textures/scale.png"),
		};

		glm::vec2 m_ViewportBounds[2]{};
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0, 0 };
	};

}