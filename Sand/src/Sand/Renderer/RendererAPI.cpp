#include "sandpch.h"
#include "Sand/Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Platform/D3D11/D3D11RendererAPI.h"

namespace Sand 
{

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::D3D11;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None:    SAND_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateScope<OpenGLRendererAPI>();
			case RendererAPI::API::D3D11:   return CreateScope<D3D11RendererAPI>();
		}

		SAND_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}