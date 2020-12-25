#include "sandpch.h"
#include "Sand/Renderer/VertexArray.h"

#include "Sand/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Sand 
{

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    SAND_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexArray>();
		}

		SAND_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}