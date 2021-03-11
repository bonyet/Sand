#include "EditorLayer.h"

#include "Sand/Scene/SceneSerializer.h"
#include "Sand/Utils/PlatformUtils.h"
#include "Sand/Renderer/Renderer2D.h"
#include "Sand/ImGui/imgui_custom.h"
#include "Sand/Debug/Debug.h"
#include "Sand/Math/Math.h"
#include "Sand/Scripting/ScriptEngine.h"

#include <glm\glm\gtc\type_ptr.hpp>

#include <imgui/imgui.h>
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

		// Framebuffers
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		mViewportFramebuffer = Framebuffer::Create(fbSpec);

		mActiveScene = CreateRef<Scene>();

		mEditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		mSceneHierarchyPanel.SetContext(mActiveScene);

		SetDarkTheme();

		ImGuizmo::SetOrthographic(false);

		// Make sure our engine logs go to the editor console as well
		Log::GetCoreLogger()->sinks().push_back(CreateRef<ConsolePanel>());

		mEditorCamera.Use2DControls = false;

		ScriptEngine::Init();
	}

	void EditorLayer::SetDarkTheme()
	{
		auto& style = ImGui::GetStyle();
		style.FrameRounding = 10.0f;
		style.WindowMenuButtonPosition = ImGuiDir_None;
		style.ScrollbarSize -= 2;

		auto& colors = style.Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.15f, 0.15f, 0.16f, 1.0f };
		colors[ImGuiCol_Border]   = ImVec4{ 0.0f, 0.0f, 0.5f, 1.0f };

		// Headers
		colors[ImGuiCol_Header]        = ImVec4{ 0.20f, 0.20f, 0.20f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.30f, 0.30f, 0.30f, 1.0f };
		colors[ImGuiCol_HeaderActive]  = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button]        = ImVec4{ 0.20f, 0.20f, 0.30f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.30f, 0.30f, 0.40f, 1.0f };
		colors[ImGuiCol_ButtonActive]  = ImVec4{ 0.15f, 0.15f, 0.20f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg]          = ImVec4{ 0.25f, 0.25f, 0.25f, 1.0f };
		colors[ImGuiCol_FrameBgHovered]   = ImVec4{ 0.35f, 0.35f, 0.35f, 1.0f };
		colors[ImGuiCol_FrameBgActive]    = ImVec4{ 0.20f, 0.20f, 0.20f, 1.0f };

		// Child and popups and misc
		colors[ImGuiCol_ChildBg]   = ImVec4{ 0.00f, 0.00f, 0.00f, 0.0f };
		colors[ImGuiCol_PopupBg]   = ImVec4{ 0.08f, 0.08f, 0.08f, 0.9f };
		colors[ImGuiCol_CheckMark] = ImVec4{ 0.25f, 0.60f, 1.00f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab]                = ImVec4{ 0.15f, 0.15f, 0.17f, 1.0f };
		colors[ImGuiCol_TabHovered]         = ImVec4{ 0.38f, 0.38f, 0.39f, 1.0f };
		colors[ImGuiCol_TabActive]          = ImVec4{ 0.28f, 0.28f, 0.30f, 1.0f };
		colors[ImGuiCol_TabUnfocused]       = ImVec4{ 0.15f, 0.15f, 0.16f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.20f, 0.20f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg]               = ImVec4{ 0.15f, 0.15f, 0.18f, 1.0f };
		colors[ImGuiCol_TitleBgActive]         = ImVec4{ 0.15f, 0.15f, 0.2f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed]      = ImVec4{ 0.15f, 0.15f, 0.17f, 1.0f };
		
		// Resize grip
		colors[ImGuiCol_ResizeGrip]            = ImVec4{ 0.30f, 0.30f, 0.30f, 1.0f };
		colors[ImGuiCol_ResizeGripHovered]     = ImVec4{ 0.35f, 0.35f, 0.35f, 1.0f };
		colors[ImGuiCol_ResizeGripActive]      = ImVec4{ 0.30f, 0.30f, 0.30f, 1.0f };

		colors[ImGuiCol_Text]                 = ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f };
		colors[ImGuiCol_ModalWindowDarkening] = ImVec4{ 0.2f, 0.2f, 0.2f, 0.5f };
		colors[ImGuiCol_DockingPreview]       = ImVec4{ 0.45f, 0.45f, 0.45f, 1.0f };
		colors[ImGuiCol_MenuBarBg]            = ImVec4{ 0.14f, 0.14f, 0.14f, 1.0f };
	}

	void EditorLayer::OnDetach()
	{
		ScriptEngine::Cleanup();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		SAND_PROFILE_FUNCTION();

		if (mViewportHovered) {
			mEditorCamera.OnUpdate(ts);
		}

		Renderer2D::ResetStats();

		if (FramebufferSpecification spec = mViewportFramebuffer->GetSpecification();
			mViewportSize.x > 0 && mViewportSize.y > 0 && // zero sized framebuffer is invalid
			(spec.Width != mViewportSize.x || spec.Height != mViewportSize.y))
		{
			mViewportFramebuffer->Resize((uint32_t)mViewportSize.x, (uint32_t)mViewportSize.y);

			mEditorCamera.SetViewportSize(mViewportSize.x, mViewportSize.y);
			mActiveScene->OnViewportResize(mViewportSize.x, mViewportSize.y);
		}

		mViewportFramebuffer->Bind();

		RenderCommand::SetClearColor({ 0.12f, 0.12f, 0.12f, 1.0f });
		RenderCommand::Clear();

		if (Input::WasKeyPressed(Keycode::P))
		{
			if (mActiveScene->IsPlaying())
			{
				mActiveScene->EndPlay();
			}
			else
			{
				mActiveScene->BeginPlay();
			}
		}

		if (!mActiveScene->IsPlaying())
			mActiveScene->OnUpdateEditor(ts, mEditorCamera);
		else
			mActiveScene->OnUpdateRuntime(ts);

		if (Input::WasKeyPressed(Mousecode::Left))
			MousePick();

		mViewportFramebuffer->Unbind();
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
					mSceneHierarchyPanel.Show();
				}
				if (ImGui::MenuItem("Properties")) {
					mPropertiesPanel.Show();
				}
				if (ImGui::MenuItem("Console")) {
					mConsolePanel.Show();
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
			auto stats = Renderer2D::GetStats();
			ImGui::Text("Renderer2D:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			ImGui::TreePop();
		}

		ImGui::End();

		mSceneHierarchyPanel.OnGuiRender();
		mPropertiesPanel.SetSelection(mSceneHierarchyPanel.GetSelectedActor());
		mPropertiesPanel.OnGuiRender();
		mConsolePanel.OnGuiRender();

		mActiveScene->GetPhysicsWorld().ShowDebugWindow(mActiveScene.get());

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport", false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
		
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		mViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		mViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		mViewportFocused = ImGui::IsWindowFocused();
		mViewportHovered = ImGui::IsWindowHovered();

		//Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = mViewportFramebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		// GIZMOS
		Actor selectedActor = mSceneHierarchyPanel.GetSelectedActor();

		if (selectedActor && mGizmoType != -1)
		{
			ImGuizmo::SetDrawlist();
			float windowWidth = ImGui::GetWindowWidth(), windowHeight = ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
			ImGuizmo::SetRect(mViewportBounds[0].x, mViewportBounds[0].y, mViewportBounds[1].x - mViewportBounds[0].x, mViewportBounds[1].y - mViewportBounds[0].y);

			// Runtime camera
			//auto cameraActor = m_ActiveScene->GetPrimaryCameraActor();
			//const auto& camera = cameraActor.GetComponent<CameraComponent>().Camera;
			//const glm::mat4& cameraProjection = camera.GetProjection();
			//glm::mat4 cameraView = glm::inverse(cameraActor.GetComponent<TransformComponent>().GetTransform());

			// Editor camera
			const glm::mat4& cameraProjection = mEditorCamera.GetProjection();
			glm::mat4 cameraView = mEditorCamera.GetViewMatrix();

			// Actor transform
			auto& transformComponent = selectedActor.GetComponent<TransformComponent>();
			glm::mat4 transform = transformComponent.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(Keycode::LeftControl);
			float snapValue = 0.5f; // 0.5m for translation and scale
			// Snap to 15 degrees for rotation
			if (mGizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 15.0f;
			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)mGizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(transform), 
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing() && !Input::IsKeyPressed(Mousecode::Right))
			{
				glm::vec2 translation, scale;
				float rotation;
				Math::DecomposeTransform(transform, translation, scale, rotation);

				float deltaRotation = rotation - transformComponent.Rotation;
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

		if (mViewportHovered)
			mEditorCamera.OnEvent(e);

		dispatcher.Dispatch<KeyPressedEvent>(SAND_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(SAND_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		return false;
	}

	void EditorLayer::MousePick()
	{
		bool shouldPick = !ImGuizmo::IsOver() && !ImGuizmo::IsUsing() && mViewportHovered;
		if (!shouldPick)
			return;

		auto [mx, my] = ImGui::GetMousePos();
		mx -= mViewportBounds[0].x;
		my -= mViewportBounds[0].y;
		glm::vec2 viewportSize = mViewportBounds[1] - mViewportBounds[0];
		my = viewportSize.y - my;

		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = mViewportFramebuffer->ReadPixel(1, mouseX, mouseY);

			if (pixelData < 0 || pixelData > mActiveScene->GetNumberOfActors())
			{
				pixelData = -1;
				mSceneHierarchyPanel.SetSelectedActor({});
			}

			if (pixelData != -1)
			{
				Actor clickedActor = Actor{ (entt::entity)pixelData, mActiveScene.get() };
				mSceneHierarchyPanel.SetSelectedActor(clickedActor);
			}
		}
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
				if (!mViewportHovered || ImGuizmo::IsUsing())
					break;

				mGizmoType = -1;
				break;
			}
			case Keycode::W:
			{
				if (!mViewportHovered || ImGuizmo::IsUsing())
					break;

				mGizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case Keycode::E: 
			{
				if (!mViewportHovered || ImGuizmo::IsUsing())
					break;

				mGizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case Keycode::R:
			{
				if (!mViewportHovered || ImGuizmo::IsUsing())
					break;

				mGizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}

			// CAMERA STUFF
			case Keycode::KP_0:
			{
				if (!mViewportHovered)
					break;

				mEditorCamera.Use2DControls = !mEditorCamera.Use2DControls;
				break;
			}			

			// RANDOM EDITOR KEYBINDS
			case Keycode::F:
			{
				Actor focused = mSceneHierarchyPanel.GetSelectedActor();
				if (!mSceneHierarchyPanel.GetSelectedActor())
					break;

				const auto& tc = focused.GetComponent<TransformComponent>();
				const auto& position = tc.Position;
				const auto& rotation = tc.Rotation;

				mEditorCamera.SetRotation(0.0f, 0.0f);
				mEditorCamera.SetFocalPoint({ position, 0.0f });
				mEditorCamera.SetDistance(10.0f);

				break;
			}

		}

		return false;
	}

	void EditorLayer::NewScene()
	{
		mActiveScene = CreateRef<Scene>();
		mActiveScene->OnViewportResize(mViewportSize.x, mViewportSize.y);
		mSceneHierarchyPanel.SetContext(mActiveScene);
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Sand Scene (*.sscene)\0*.sscene\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(mActiveScene);
			serializer.Serialize(filepath);
		}
	}
	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Sand Scene (*.sscene)\0*.sscene\0");
		if (!filepath.empty())
		{
			mActiveScene = CreateRef<Scene>();
			mActiveScene->OnViewportResize(mViewportSize.x, mViewportSize.y);
			mSceneHierarchyPanel.SetContext(mActiveScene);

			SceneSerializer serializer(mActiveScene);
			serializer.Deserialize(filepath);
		}
	}
}