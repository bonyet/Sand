#pragma once

#include "Sand/Renderer/RendererAPI.h"

namespace Sand 
{

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual ~OpenGLRendererAPI() override {}

		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;

		// Inherited via RendererAPI
		virtual void SetTopology(TopologyType type) override;

		// Inherited via RendererAPI
		virtual void SetLineWidth(float width) override;
	};


}
