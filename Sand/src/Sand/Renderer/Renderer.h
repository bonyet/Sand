#pragma once

#include "Sand/Renderer/RenderCommand.h"

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

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void EndScene();

		static void DrawCube();

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		struct SceneData
		{
			glm::mat4 ViewProj;
		};
	private:
		static Scope<SceneData> s_SceneData;
	};
}
