#include "sandpch.h"
#include "D3D11Context.h"
#include "D3D11SwapChain.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include <comdef.h>

namespace Sand
{

	D3D11Context* D3D11Context::s_Instance;

	D3D11Context::D3D11Context(GLFWwindow* window)
		: m_WindowHandle(window)
	{
		SAND_CORE_ASSERT(s_Instance == nullptr, "Only one Direct3D11 context allowed.");
		s_Instance = this;

		m_hWnd = glfwGetWin32Window(window);
	}

	D3D11Context::~D3D11Context()
	{
		SAND_CORE_INFO("Destroying Direct3D 11 objects...");

		if (pDevice)
			pDevice->Release(); SAND_CORE_INFO("Destroyed device.");
		if (pContext)
			pContext->Release(); SAND_CORE_INFO("Destroyed context.");
		if (pTarget)
			pTarget->Release(); SAND_CORE_INFO("Destroyed render target.");
		if (pSwapChain)
			pSwapChain->Release(); SAND_CORE_INFO("Destroyed swapchain.");
	}
	
	void D3D11Context::Init()
	{ 
		pSwapChain = CreateRef<D3D11SwapChain>(m_hWnd);

		// Create device, front / back buffers, & rendering context
		D3D11_CALL(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION,
			&pSwapChain->GetDescription(), pSwapChain->AsD3D11ResourcePointer(), &pDevice, nullptr, &pContext));
	}
	
	void D3D11Context::SwapBuffers()
	{
		pSwapChain->Present(1u);
	}

}