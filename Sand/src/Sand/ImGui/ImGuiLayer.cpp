#include "sandpch.h"
#include "Sand/ImGui/ImGuiLayer.h"
#include "Sand/Core/Application.h"
#include "Platform/D3D11/D3D11Context.h"
#include "Sand/Renderer/RendererAPI.h"

#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_dx11.h>
#include <examples/imgui_impl_win32.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <glad/glad.h>

#include "ImGuizmo.h"

namespace Sand 
{

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	void ImGuiLayer::OnAttach()
	{
		SAND_PROFILE_FUNCTION();

		// setup imgui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
															
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.WindowBorderSize = 0.0f;
			style.WindowPadding = ImVec2(2.0f, 2.0f);

			io.Fonts->AddFontFromFileTTF("assets/fonts/Mulish-SemiBold.ttf", 20);
			io.Fonts->AddFontFromFileTTF("assets/fonts/Mulish-Bold.ttf", 20);
			io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Regular.ttf", 20);
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
		{
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 410");
		}
		else if(RendererAPI::GetAPI() == RendererAPI::API::D3D11)
		{
			auto context = D3D11Context::GetInstance();
			ImGui_ImplWin32_Init(glfwGetWin32Window(window));
			ImGui_ImplDX11_Init(context->GetDevice(), context->GetDeviceContext());
		}
	}

	void ImGuiLayer::OnDetach()
	{
		SAND_PROFILE_FUNCTION();

		if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
		}
		else if (RendererAPI::GetAPI() == RendererAPI::API::D3D11)
		{
			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();
		}

		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}
	
	void ImGuiLayer::Begin()
	{
		SAND_PROFILE_FUNCTION();

		if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
		}
		else if (RendererAPI::GetAPI() == RendererAPI::API::D3D11)
		{
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
		}

		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End()
	{
		SAND_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		else if (RendererAPI::GetAPI() == RendererAPI::API::D3D11)
		{
			ImGui_ImplWin32_NewFrame();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::SetFontScale(float scale)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.FontGlobalScale = scale;
	}

}