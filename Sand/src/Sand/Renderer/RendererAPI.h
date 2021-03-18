#pragma once

#include <glm/glm.hpp>

#include "Sand/Renderer/VertexArray.h"

namespace Sand 
{

	enum class TopologyType
	{
		Lines, Triangles
	};

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1/*, D3D11 = 2,*/
		};
	public:
		virtual ~RendererAPI() {}

		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

		virtual void SetTopology(TopologyType type) = 0;
		virtual void SetLineWidth(float width) = 0;

		static API GetAPI() { return s_API; }

		static Scope<RendererAPI> Create();
	private:
		static API s_API;
	};

}
