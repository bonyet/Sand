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

		static void BeginScene(EditorCamera& camera);
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const glm::mat4& projection, const glm::mat4& transform);
		static void EndScene();

		static void DrawCube(const glm::vec3& position, const glm::vec3& scale, const glm::vec4& color);

		static void OnWindowResize(uint32_t width, uint32_t height);
		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}
