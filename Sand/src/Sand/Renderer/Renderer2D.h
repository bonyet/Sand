#pragma once

#include "Sand/Renderer/Texture.h"
#include "Sand/Renderer/Camera.h"
#include "Sand/Renderer/Shader.h"
#include "Sand/Renderer/EditorCamera.h"

namespace Sand
{

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void Begin(const glm::mat4& proj, const glm::mat4& transform);
		static void Begin(const Camera& camera, const glm::mat4& transform);
		static void Begin(const EditorCamera& camera);
		static void End();
		static void Flush();

		// Primitive quads
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, uint32_t actorID);
		// Textured quads
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, uint32_t actorID);

		static Ref<Shader> GetShader();
		static void SetShader(const Ref<Shader>& shader);

		// Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		};

		static void ResetStats();
		static Statistics GetStats();
	private:
		static void FlushAndReset();
	};

}