#include "sandpch.h"
#include "Sand/Renderer/Renderer.h"
#include "Sand/Renderer/Renderer2D.h"
#include <array>

namespace Sand 
{
	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	struct RendererData
	{
		Ref<VertexArray> VertexArray;
		Ref<VertexBuffer> VertexBuffer;
		Ref<Material> Material;		
	};

	static RendererData s_Data;

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	void Renderer::Init()
	{
		SAND_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();

		// setup
#if 0
		float vertices[] =
		{
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f
		};

		s_Data.VertexArray = VertexArray::Create();
		s_Data.VertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		
		s_Data.VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
		});

		s_Data.VertexArray->AddVertexBuffer(s_Data.VertexBuffer);

		uint32_t indices[6 * 6] =
		{
			0, 1, 3, 3, 1, 2,
			1, 5, 2, 2, 5, 6,
			5, 4, 6, 6, 4, 7,
			4, 0, 7, 7, 0, 3,
			3, 2, 7, 7, 2, 6,
			4, 5, 0, 0, 5, 1
		};


		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, 36);
		s_Data.VertexArray->SetIndexBuffer(indexBuffer);

		s_Data.Material = Material::Create("assets/shaders/BaseColor.glsl");
		s_Data.Material->GetShader()->Bind();
#endif
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		s_SceneData->ViewProj = viewProj;
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::DrawCube()
	{
		s_Data.Material->GetShader()->Bind();
		s_Data.Material->GetShader()->SetMat4("u_ViewProjection", s_SceneData->ViewProj);
		
		s_Data.VertexArray->Bind();
		s_Data.VertexBuffer->Bind();
		s_Data.VertexArray->GetIndexBuffer()->Bind();
		RenderCommand::DrawIndexed(s_Data.VertexArray);
	}

}
