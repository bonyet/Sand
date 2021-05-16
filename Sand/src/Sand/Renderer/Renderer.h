#pragma once

#include "Sand/Renderer/RenderCommand.h"

#include "Sand/Renderer/EditorCamera.h"
#include "Sand/Renderer/Camera.h"
#include "Sand/Renderer/Shader.h"

namespace Sand 
{

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);
		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}
