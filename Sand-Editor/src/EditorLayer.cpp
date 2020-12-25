#include "EditorLayer.h"

#include <glm\glm\gtc\type_ptr.hpp>

#include "Sand/Scene/SceneSerializer.h"
#include "Sand/Utils/PlatformUtils.h"
#include "ImGuizmo.h"

#include "Sand/Math/Math.h"

namespace Sand
{

	EditorLayer::EditorLayer()
		: Layer("Editor")
	{
	}

	void EditorLayer::OnAttach()
	{
		SAND_PROFILE_FUNCTION();

		FramebufferSpecification framebufferSpecs = { 1280, 720 };
		m_Framebuffer = Framebuffer::Create(framebufferSpecs);

		m_ActiveScene = CreateRef<Scene>("Unnamed Scene");
		Scene::SetActiveScene(m_ActiveScene);

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		SetupImGuiStyle();
	}

	void EditorLayer::SetupImGuiStyle()
	{
		auto& style = ImGui::GetStyle();
		style.FrameRounding = 10.0f;		

		auto& colors = style.Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		// Headers
		colors[ImGuiCol_Header]        = ImVec4{ 0.20f, 0.20f, 0.20f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.30f, 0.30f, 0.30f, 1.0f };
		colors[ImGuiCol_HeaderActive]  = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button]        = ImVec4{ 0.20f, 0.20f, 0.20f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.30f, 0.30f, 0.30f, 1.0f };
		colors[ImGuiCol_ButtonActive]  = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg]          = ImVec4{ 0.20f, 0.20f, 0.20f, 1.0f };
		colors[ImGuiCol_FrameBgHovered]   = ImVec4{ 0.30f, 0.30f, 0.30f, 1.0f };
		colors[ImGuiCol_FrameBgActive]    = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab]                = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TabHovered]         = ImVec4{ 0.38f, 0.38f, 0.38f, 1.0f };
		colors[ImGuiCol_TabActive]          = ImVec4{ 0.28f, 0.28f, 0.28f, 1.0f };
		colors[ImGuiCol_TabUnfocused]       = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.20f, 0.20f, 0.20f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg]               = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TitleBgActive]         = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed]      = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		// Resize grip
		colors[ImGuiCol_ResizeGrip]            = ImVec4{ 0.30, 0.30, 0.30, 1.0f };
		colors[ImGuiCol_ResizeGripHovered]     = ImVec4{ 0.35, 0.35, 0.35, 1.0f };
		colors[ImGuiCol_ResizeGripActive]      = ImVec4{ 0.30, 0.30, 0.30, 1.0f };

		colors[ImGuiCol_DockingPreview] = ImVec4{ 0.45f, 0.45f, 0.45f, 1.0f };
	}

	void EditorLayer::OnDetach() {}

	void EditorLayer::CheckToResizeFramebuffer()
	{
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0 && m_ViewportSize.y > 0 && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			glm::vec2 size = m_ViewportSize;
			m_Framebuffer->Resize((uint32_t)size.x, (uint32_t)size.y);
			m_ActiveScene->OnViewportResize(size.x, size.y);
		}
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		SAND_PROFILE_FUNCTION();

		Renderer2D::ResetStats();

		CheckToResizeFramebuffer();

		m_Framebuffer->Bind();

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.12f, 1.0f });
		RenderCommand::Clear();

		m_ActiveScene->OnUpdate(ts);

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnGuiRender()
	{
		SAND_PROFILE_FUNCTION();

		// dockspace nonsense
		{
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);

			window_flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

			ImGui::Begin("DockSpace Demo", (bool*)true, window_flags);

			ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();
			float minWinSizeX = style.WindowMinSize.x;
			style.WindowMinSize.x = 300.0f;
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("DockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}
			style.WindowMinSize.x = minWinSizeX;
		}

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					NewScene();
				}
				if (ImGui::MenuItem("Open", "Ctrl+O"))
				{
					OpenScene();
				}
				if (ImGui::MenuItem("Save", "Shift+S"))
				{
					SaveScene();
				}
				if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}
		
				ImGui::EndMenu();
			}
		
			ImGui::EndMainMenuBar();
		}

		static bool showSettingsWindow = true;
		if (showSettingsWindow)
		{
			ImGui::Begin("Application Statistics", &showSettingsWindow);

			auto font = ImGui::GetFont();
			font->Scale = 1.1f;
			ImGui::PushFont(font);
			ImGui::Text("%.3fms (%.1ffps)", 1000 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			font->Scale = 1.0f;
			ImGui::PopFont();

			ImGui::Separator();
			auto stats = Renderer2D::GetStats();
			ImGui::Text("Renderer2D:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			ImGui::End();
		}

		m_SceneHierarchyPanel.OnGuiRender();
		m_AssetManagerPanel.OnGuiRender();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport", false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

		m_ViewportFocused = ImGui::IsWindowFocused(), m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		if (m_ViewportSize != *((glm::vec2*) & viewportPanelSize))
		{
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		// GIZMOS
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();

		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			float windowWidth = (float)ImGui::GetWindowWidth(), windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Get camera
			auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			const glm::mat4& cameraProjection = camera.GetProjection();
			glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());
			
			// Entity transform
			auto& transformComponent = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = transformComponent.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(Keycode::LeftControl);
			float snapValue = 0.5f; // 0.5m for translation and scale
			// Snap to 15 degrees for rotation
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 15.0f;
			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(transform), 
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing()) 
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - transformComponent.Rotation;
				transformComponent.Position = translation;
				transformComponent.Rotation += deltaRotation;
				transformComponent.Scale = scale;
			}
		}


		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<KeyPressedEvent>(SAND_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		SAND_PROFILE_FUNCTION();

		// Shortcuts
        if (e.GetRepeatCount() > 0)
			return false;

		bool controlPressed = Input::IsKeyPressed(Keycode::LeftControl);
		bool shiftPressed = Input::IsKeyPressed(Keycode::LeftShift);

		switch (e.GetKey())
		{
			case Keycode::N:
			{
				if (controlPressed)
					NewScene();
				break;
			}
			case Keycode::O: 
			{
				if (controlPressed)
					OpenScene();
				break;
			}
			case Keycode::S:
			{
				if (controlPressed && shiftPressed)
					SaveSceneAs();
				else if (controlPressed && !shiftPressed)
					SaveScene();
				break;
			}
			// GIZMOS YEA

			case Keycode::Q: // SELECT
				m_GizmoType = -1;
				break;
			case Keycode::W: // TRANSLATE
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case Keycode::E: // ROTATE
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case Keycode::R: // SCALE
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
		}
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	static std::string currentSceneFilepath;
	void EditorLayer::SaveScene()
	{
		if (!currentSceneFilepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(currentSceneFilepath);

			// TODO: visual feedback that the scene saved
		}
	}
	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Sand Scene (*.sscene)\0*.sscene\0");
		if (!filepath.empty())
		{
			currentSceneFilepath = filepath;
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
	}
	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Sand Scene (*.sscene)\0*.sscene\0");
		if (!filepath.empty())
		{
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);
			Scene::SetActiveScene(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filepath);
		}
	}
}