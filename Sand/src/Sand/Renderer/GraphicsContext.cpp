#include "sandpch.h"
#include "Sand/Renderer/GraphicsContext.h"

#include "Sand/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Sand 
{

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    SAND_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
			//case RendererAPI::API::D3D11:   return CreateScope<D3D11Context>(static_cast<GLFWwindow*>(window));
		}

		SAND_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}