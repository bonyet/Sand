#include "sandpch.h"
#include "Sand/Renderer/Renderer.h"
#include "Sand/Renderer/Renderer2D.h"

namespace Sand 
{

	struct RendererData
	{
		Ref<VertexArray> CubeVertexArray;
		Ref<VertexBuffer> CubeVertexBuffer;
		Ref<Shader> CubeShader;
	};

	static RendererData s_Data;

	struct CubeVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		int ObjectID;
	};

	void Renderer::Init()
	{
		SAND_PROFILE_FUNCTION();

		RenderCommand::Init();


		//Renderer2D::Init();
		//
		//s_Data.CubeVertexArray = VertexArray::Create();
		//
		//float vertices[] = {
		//	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0,
		//	 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0,
		//	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0,
		//	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0,
		//	-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0,
		//	 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0,
		//	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0,
		//	-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0,
		//};
		//
		//s_Data.CubeVertexBuffer = VertexBuffer::Create(vertices, 8 * sizeof(CubeVertex));
		//
		//s_Data.CubeVertexBuffer->SetLayout({
		//	{ ShaderDataType::Float3, "a_Position" },
		//	{ ShaderDataType::Float4, "a_Color" },
		//	{ ShaderDataType::Int, "a_ObjectID" },
		//});
		//
		//s_Data.CubeVertexArray->AddVertexBuffer(s_Data.CubeVertexBuffer);
		//
		//uint32_t quadIndices[6 * 6] = {
		//	0, 1, 3, 3, 1, 2,
		//	1, 5, 2, 2, 5, 6,
		//	5, 4, 6, 6, 4, 7,
		//	4, 0, 7, 7, 0, 3,
		//	3, 2, 7, 7, 2, 6,
		//	4, 5, 0, 0, 5, 1
		//};
		//
		//Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, 6 * 6);
		//s_Data.CubeVertexArray->SetIndexBuffer(quadIB);
		//
		//s_Data.CubeShader = Shader::Create("assets/shaders/BasicCube.glsl");
		//s_Data.CubeShader->Bind();
		//s_Data.CubeShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(EditorCamera& camera)
	{
		s_Data.CubeShader->Bind();
		s_Data.CubeShader->SetMat4("u_ViewProjection", camera.GetViewProjection());
	}

	void Renderer::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		s_Data.CubeShader->Bind();
		s_Data.CubeShader->SetMat4("u_ViewProjection", viewProj);
	}

	void Renderer::BeginScene(const glm::mat4& projection, const glm::mat4& transform)
	{
		glm::mat4 viewProj = projection * glm::inverse(transform);

		s_Data.CubeShader->Bind();
		s_Data.CubeShader->SetMat4("u_ViewProjection", viewProj);
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& scale, const glm::vec4& color)
	{
		s_Data.CubeShader->Bind();
		s_Data.CubeVertexArray->Bind();

		RenderCommand::DrawIndexed(s_Data.CubeVertexArray, 6 * 6);
	}

}