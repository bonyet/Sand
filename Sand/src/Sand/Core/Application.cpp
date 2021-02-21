#include "sandpch.h"
#include "Sand/Core/Application.h"

#include "Sand/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

#include "Sand/Scene/Scene.h"

namespace Sand
{
	Application* Application::s_Instance = nullptr;
	float Time::m_Time;

	Application::Application(const std::string& name)
	{
		SAND_PROFILE_FUNCTION();

		SAND_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(SAND_BIND_EVENT_FN(Application::OnEvent));
		m_Window->SetVSync(true);

		Renderer::Init();

		m_UILayer = new ImGuiLayer();
		PushOverlay(m_UILayer);
	}

	Application::~Application()
	{
		SAND_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
		SAND_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		SAND_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& e)
	{		
		SAND_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(SAND_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(SAND_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled) 
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::Run()
	{
		SAND_PROFILE_FUNCTION();

		while (m_Running)
		{
			SAND_PROFILE_SCOPE("Run Loop");

			float time = (float)glfwGetTime(); // Lifetime of app
			Timestep timestep = time - m_LastFrameTime; // Last frame
			m_LastFrameTime = time;
			Time::m_Time = time;

			if (!m_Minimized)
			{
				{
					SAND_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : m_LayerStack)
					{
						layer->OnUpdate(timestep);
					}
				}

				m_UILayer->Begin();
				{
					SAND_PROFILE_SCOPE("LayerStack OnImGuiRender");
				
					for (Layer* layer : m_LayerStack)
					{
						layer->OnGuiRender();
					}
				}
				m_UILayer->End();
			}

			Input::ClearState();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		SAND_PROFILE_FUNCTION();

		// check if window was minimized
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}
