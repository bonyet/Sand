#include "sandpch.h"
#include "Platform/Windows/WindowsWindow.h"

#include "Sand/Events/ApplicationEvent.h"
#include "Sand/Events/MouseEvent.h"
#include "Sand/Events/KeyEvent.h"

#include "Sand/Renderer/Renderer.h"

#include "Sand/Core/Application.h"

#include "Platform/OpenGL/OpenGLContext.h"
#include <glad\glad.h>

#include "stb_image.h"

#include "Sand/Scene/Scene.h"

namespace Sand 
{	
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		SAND_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		SAND_PROFILE_FUNCTION();

		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		SAND_PROFILE_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		SAND_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		if (s_GLFWWindowCount == 0)
		{
			SAND_PROFILE_SCOPE("glfwInit");
			int success = glfwInit();
			SAND_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			SAND_PROFILE_SCOPE("glfwCreateWindow");
		#if defined(SAND_DEBUG)
			if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		#endif
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			Keycode keycode = (Keycode)key;
			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(keycode, 0);
					data.EventCallback(event);
					Input::OnKeyPressed(keycode);

					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(keycode);
					data.EventCallback(event);
					Input::OnKeyReleased(keycode);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(keycode, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int key)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event((Keycode)key);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event((Mousecode)button);
					data.EventCallback(event);
					Input::OnMouseButtonPressed(static_cast<Mousecode>(button));
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event((Mousecode)button);
					data.EventCallback(event);
					Input::OnMouseButtonReleased(static_cast<Mousecode>(button));
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	}

	void WindowsWindow::Shutdown()
	{
		SAND_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void WindowsWindow::OnUpdate()
	{
		SAND_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();		
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		SAND_PROFILE_FUNCTION();

		if (enabled == true)
			glfwSwapInterval(1);
		else if (enabled == false)
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::SetTitle(const std::string& name)
	{
		std::string windowtitle = "Sand | " + name + " - " + RendererAPI::GetAPIAsString();

		m_Data.Title = windowtitle;

		glfwSetWindowTitle(m_Window, windowtitle.c_str());
	}

	void WindowsWindow::SetIcon(const std::string& filepath)
	{
		int x, y, channels;
		auto pixels = stbi_load(filepath.c_str(), &x, &y, &channels, 0);;

		GLFWimage* image = new GLFWimage();
		image->height = y, image->width = x;
		image->pixels = pixels;

		glfwSetWindowIcon(m_Window, 1, image);

		stbi_image_free(pixels);

		delete image;
	}

}
