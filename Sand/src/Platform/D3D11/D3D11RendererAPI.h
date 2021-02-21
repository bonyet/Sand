#pragma once

#include "Sand/Renderer/RendererAPI.h"

#include <d3d11.h>

namespace Sand
{

	class D3D11RendererAPI : public RendererAPI
	{
	public:
		virtual ~D3D11RendererAPI();

		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
	};

}