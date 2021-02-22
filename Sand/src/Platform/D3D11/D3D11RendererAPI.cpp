#include "sandpch.h"
#include "D3D11RendererAPI.h"

#include "Sand/Core/Application.h"

#include "D3D11Context.h"

#pragma comment(lib, "d3d11.lib")

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include <comdef.h>

namespace Sand
{
	
	D3D11RendererAPI::~D3D11RendererAPI()
	{

	}

	void D3D11RendererAPI::Init()
	{
		HWND hWnd = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
				
		auto swapChain = D3D11Context::GetInstance()->GetSwapChain();
		auto context = D3D11Context::GetInstance();

		// Create the render target view
		auto backBuffer = swapChain->GetBackBuffer();
		D3D11_CALL(context->GetDevice()->CreateRenderTargetView(backBuffer, nullptr, context->GetTargetPP()));
		backBuffer->Release(); // Only needed this to create the view
	}

	void D3D11RendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
	}

	static glm::vec3 s_ClearColor = { 0.0f, 0.0f, 0.0f };

	void D3D11RendererAPI::SetClearColor(const glm::vec4& color)
	{
		s_ClearColor = color;
	}

	void D3D11RendererAPI::Clear()
	{
		const float colors[] = { s_ClearColor.r, s_ClearColor.g, s_ClearColor.b };

		auto context = D3D11Context::GetInstance();
		context->GetDeviceContext()->ClearRenderTargetView(context->GetTarget(), colors);
	}

	void D3D11RendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
	}

}