#pragma once

#include "Sand/Renderer/Buffer.h"

namespace Sand 
{

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();
		
		virtual void SetData(const void* data, uint32_t size) override;
		
		virtual uint32_t GetHandle() const override { return m_Handle; }

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
	private:
		uint32_t m_Handle;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual uint32_t GetCount() const override { return m_Count; }
		virtual uint32_t GetHandle() const override { return m_Handle; }
	private:
		uint32_t m_Handle;
		uint32_t m_Count;
	};

}