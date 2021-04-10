#include "sandpch.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Sand/Core/Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Sand 
{

	static TopologyType s_TopologyType = TopologyType::Triangles;
	
	void OpenGLMessageCallback(
		unsigned source, unsigned type, unsigned id, unsigned severity, 
		int length,	const char* message, const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         SAND_CORE_CRITICAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       SAND_CORE_ERROR(message);    return;
			case GL_DEBUG_SEVERITY_LOW:          SAND_CORE_WARN(message);     return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: SAND_CORE_TRACE(message);    return;
		}
		
		SAND_CORE_ASSERT(false, "Unknown severity level!");
	}

	void OpenGLRendererAPI::Init()
	{
		SAND_PROFILE_FUNCTION();

	#ifdef SAND_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	#endif
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		
		glDrawElements(s_TopologyType == TopologyType::Lines ? GL_LINE_STRIP : GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLRendererAPI::SetTopology(TopologyType type)
	{
		s_TopologyType = type;
	}

	void OpenGLRendererAPI::SetLineWidth(float width)
	{
		glLineWidth(width);
	}

}
