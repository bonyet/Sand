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
		void MousePick();

		void NewScene();
		void SaveSceneAs();
		void OpenScene();
	private:
		Ref<Framebuffer> mViewportFramebuffer;

		Ref<Scene> mActiveScene;

		SceneHierarchyPanel mSceneHierarchyPanel;
		PropertiesPanel mPropertiesPanel;
		ConsolePanel mConsolePanel;

		EditorCamera mEditorCamera;

		int mGizmoType = -1;

		glm::vec2 mViewportBounds[2]{};
		bool mViewportFocused = false, mViewportHovered = false;
		glm::vec2 mViewportSize = { 0, 0 };
	};

}