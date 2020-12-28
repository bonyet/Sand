#pragma once

#include "Sand/Core/Base.h"

#include "Sand/Core/Window.h"
#include "Sand/Core/LayerStack.h"
#include "Sand/Events/Event.h"
#include "Sand/Events/ApplicationEvent.h"

#include "Sand/Core/Time.h"

#include "Sand/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Sand 
{

	class Scene;

	class Application
	{
	public:
		Application(const std::string& name = "Application");
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void Close();

		Window& GetWindow() { return *m_Window; }
		ImGuiLayer* GetImGuiLayer() { return m_UILayer; }
		static Application& Get() { return *s_Instance; }
	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		Scope<Window> m_Window;
	private:
		ImGuiLayer* m_UILayer;
		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;
		bool m_Running = true, m_Minimized = false;
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
		friend class Scene;
	};

	// To be defined in client
	Application* CreateApplication();

}