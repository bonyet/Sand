#include "sandpch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Sand 
{

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		SAND_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		SAND_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SAND_CORE_ASSERT(status, "Failed to initialize Glad!");

		SAND_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Sand requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers()
	{
		SAND_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}
