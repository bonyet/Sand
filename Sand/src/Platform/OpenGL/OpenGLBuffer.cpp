#include "sandpch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

namespace Sand 
{

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		SAND_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_Handle);
		glNamedBufferData(m_Handle, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		SAND_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_Handle);
		glNamedBufferData(m_Handle, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		SAND_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_Handle);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		SAND_PROFILE_FUNCTION();

		glNamedBufferSubData(m_Handle, 0, size, data);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		SAND_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_Handle);		
		glNamedBufferData(m_Handle, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		SAND_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_Handle);
	}

}
