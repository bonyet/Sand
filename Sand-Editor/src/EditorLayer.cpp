#include "EditorLayer.h"

#include "Sand/Scene/SceneSerializer.h"
#include "Sand/Utils/PlatformUtils.h"
#include "Sand/Scene/SceneRenderer.h"
#include "Sand/ImGui/imgui_custom.h"
#include "Sand/Debug/Debug.h"
#include "Sand/Math/Math.h"

#include <glm\glm\gtc\type_ptr.hpp>
#include "ImGuizmo.h"

namespace Sand
{

	EditorLayer::EditorLayer()
		: Layer("Editor")
	{
	}

	void EditorLayer::OnAttach()
	{
		SAND_PROFILE_FUNCTION();

		SceneRenderer::Init();

		// Framebuffers
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_ViewportFramebuffer = Framebuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		SetDarkTheme();

		ImGuizmo::SetOrthographic(false);

		// Make sure our engine logs go to the editor console as well
		Log::GetCoreLogger()->sinks().push_back(std::make_shared<ConsolePanel>());

		m_EditorCamera.Use2DControls = false;
	}

	void EditorLayer::SetDarkTheme()
	{
		auto& style = ImGui::GetStyle();
		style.FrameRounding = 10.0f;
		style.WindowTitleAlign = { 0.5f, 0.5f };
		style.WindowMenuButtonPosition = ImGuiDir_None;

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

		// Child and popups and misc
		colors[ImGuiCol_ChildBg] = ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f };
		colors[ImGuiCol_PopupBg] = ImVec4{ 0.0799999982f, 0.0799999982f, 0.0799999982f, 0.9399998f };
		colors[ImGuiCol_CheckMark] = ImVec4{ 0.259999990f, 0.589999974f, 0.980000019f, 1.0f };

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
		colors[ImGuiCol_ResizeGrip]            = ImVec4{ 0.30f, 0.30f, 0.30f, 1.0f };
		colors[ImGuiCol_ResizeGripHovered]     = ImVec4{ 0.35f, 0.35f, 0.35f, 1.0f };
		colors[ImGuiCol_ResizeGripActive]      = ImVec4{ 0.30f, 0.30f, 0.30f, 1.0f };

		colors[ImGuiCol_Text] = ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f };
		colors[ImGuiCol_ModalWindowDarkening] = ImVec4{ 0.2f, 0.2f, 0.2f, 0.5f };
		colors[ImGuiCol_DockingPreview] = ImVec4{ 0.45f, 0.45f, 0.45f, 1.0f };
		colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.14f, 0.14f, 0.14f, 1.0f };
	}

	void EditorLayer::OnDetach() 
	{
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		SAND_PROFILE_FUNCTION();

		if (m_ViewportHovered) {
			m_EditorCamera.OnUpdate(ts);
		}

		SceneRenderer::ResetStats();
		Renderer2D::ResetStats();

		if (FramebufferSpecification spec = m_ViewportFramebuffer->GetSpecification();
			m_ViewportSize.x > 0 && m_ViewportSize.y > 0 && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_ViewportFramebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
		}

		m_ViewportFramebuffer->Bind();

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.12f, 1.0f });
		RenderCommand::Clear();

		m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

		{
			bool shouldMousePick = !ImGuizmo::IsOver() && !ImGuizmo::IsUsing() && m_ViewportHovered;
			if (Input::WasKeyPressed(Mousecode::Left) && shouldMousePick)
			{
				auto [mx, my] = ImGui::GetMousePos();
				mx -= m_ViewportBounds[0].x;
				my -= m_ViewportBounds[0].y;
				glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
				my = viewportSize.y - my;

				int mouseX = (int)mx;
				int mouseY = (int)my;

				if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
				{
					int pixelData = m_ViewportFramebuffer->ReadPixel(1, mouseX, mouseY);

					if (pixelData < 0 || pixelData > m_ActiveScene->GetNumberOfActors())
					{
						pixelData = -1;
						m_SceneHierarchyPanel.SetSelectedActor({});
					}

					if (pixelData != -1)
					{
						Actor clickedActor = Actor{ (entt::entity)pixelData, m_ActiveScene.get() };
						m_SceneHierarchyPanel.SetSelectedActor(clickedActor);

						SAND_CORE_INFO(clickedActor.GetComponent<TagComponent>().Name);
					}
				}
			}
		}

		m_ViewportFramebuffer->Unbind();
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
				if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Tabs"))
			{
				if (ImGui::MenuItem("Hierarchy")) {
					m_SceneHierarchyPanel.Show();
				}
				if (ImGui::MenuItem("Properties")) {
					m_PropertiesPanel.Show();
				}
				if (ImGui::MenuItem("Console")) {
					m_ConsolePanel.Show();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		ImGui::Begin("Stuff");

		auto font = ImGui::GetFont();
		font->Scale = 1.1f;
		ImGui::PushFont(font);
		ImGui::Text("%.3fms (%.1ffps)", 1000 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		font->Scale = 1.0f;
		ImGui::PopFont();

		static bool vsync = true;
		if (ImGui::Checkbox("VSync", &vsync)) {
			Application::Get().GetWindow().SetVSync(vsync);
		}

		ImGui::Separator();
		if (ImGui::TreeNodeEx("Renderer")) {
			auto stats = SceneRenderer::GetStats();
			ImGui::Text("Renderer2D:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			ImGui::TreePop();
		}

		ImGui::End();

		m_SceneHierarchyPanel.OnGuiRender();
		m_PropertiesPanel.SetSelection(m_SceneHierarchyPanel.GetSelectedActor());
		m_PropertiesPanel.OnGuiRender();
		m_ConsolePanel.OnGuiRender();
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport", false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
		
		auto viewportOffset = ImGui::GetCursorPos();
		auto viewportScreenPos = ImGui::GetCursorScreenPos();

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		//Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_ViewportFramebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		auto windowSize = ImGui::GetWindowSize();
		ImVec2 minBound = ImGui::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;

		ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
		m_ViewportBounds[0] = { minBound.x, minBound.y };
		m_ViewportBounds[1] = { maxBound.x, maxBound.y };

		// GIZMOS
		Actor selectedActor = m_SceneHierarchyPanel.GetSelectedActor();

		if (selectedActor && m_GizmoType != -1)
		{
			ImGuizmo::SetDrawlist();
			float windowWidth = ImGui::GetWindowWidth(), windowHeight = ImGui::GetWindowHeight();
			ImGuizmo::SetRect(viewportScreenPos.x, viewportScreenPos.y, windowWidth, windowHeight);

			// Runtime camera
			//auto cameraActor = m_ActiveScene->GetPrimaryCameraActor();
			//const auto& camera = cameraActor.GetComponent<CameraComponent>().Camera;
			//const glm::mat4& cameraProjection = camera.GetProjection();
			//glm::mat4 cameraView = glm::inverse(cameraActor.GetComponent<TransformComponent>().GetTransform());

			// Editor camera
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Actor transform
			auto& transformComponent = selectedActor.GetComponent<TransformComponent>();
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

			if (ImGuizmo::IsUsing() && !Input::IsKeyPressed(Mousecode::Right))
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

		if (m_ViewportHovered)
			m_EditorCamera.OnEvent(e);

		dispatcher.Dispatch<KeyPressedEvent>(SAND_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(SAND_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		

		return false;
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		SAND_PROFILE_FUNCTION();

		bool controlPressed = Input::IsKeyPressed(Keycode::LeftControl);
		bool shiftPressed = Input::IsKeyPressed(Keycode::LeftShift);

		// shortcuts
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
				break;
			}

			// GIZMOS YEA
			case Keycode::Q:
			{
				if (!m_ViewportHovered || ImGuizmo::IsUsing())
					break;

				m_GizmoType = -1;
				break;
			}
			case Keycode::W:
			{
				if (!m_ViewportHovered || ImGuizmo::IsUsing())
					break;

				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case Keycode::E: 
			{
				if (!m_ViewportHovered || ImGuizmo::IsUsing())
					break;

				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case Keycode::R:
			{
				if (!m_ViewportHovered || ImGuizmo::IsUsing())
					break;

				m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}

			// MISC KEY COMBOS
			
		}

		return false;
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Sand Scene (*.sscene)\0*.sscene\0");
		if (!filepath.empty())
		{
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
			m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filepath);
		}
	}
}