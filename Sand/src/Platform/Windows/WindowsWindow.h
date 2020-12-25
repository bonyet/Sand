#pragma once

#include "Sand/Core/Window.h"
#include "Sand/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Sand 
{

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		virtual void OnUpdate() override;

		virtual unsigned int GetWidth() const override { return m_Data.Width; }
		virtual unsigned int GetHeight() const override { return m_Data.Height; }
		virtual glm::vec2 GetScale() override { return glm::vec2((float)m_Data.Width, (float)m_Data.Height); }

		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		virtual void SetTitle(const std::string& name) override;
		virtual void SetIcon(const std::string& filepath) override;

		virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title = "Window";
			unsigned int Width = 1280, Height = 720;
			bool VSync = true;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}