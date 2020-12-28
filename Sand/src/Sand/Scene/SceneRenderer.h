#pragma once

#include <glm/glm.hpp>
#include "Sand/Renderer/EditorCamera.h"

namespace Sand
{

	class SceneRenderer
	{
	public:
		static void Init();

		static void Begin(EditorCamera& camera);
		static void Submit(const glm::mat4& transform, const glm::vec4& color, uint32_t entityID);
		static void End();
	private:
		static void Flush();
		static void FlushAndReset();
	public:
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();
	};

}