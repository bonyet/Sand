#pragma once

#include "Sand/Renderer/GraphicsContext.h"
#include "D3D11SwapChain.h"

#include <Windows.h>
#include <d3d11.h>

struct GLFWwindow;

namespace Sand
{

	class D3D11Context : public GraphicsContext
	{
	public:
		D3D11Context(GLFWwindow* window);
		~D3D11Context();

		virtual void Init() override;
		virtual void SwapBuffers() override;

		ID3D11Device* const GetDevice() const { return pDevice; }
		ID3D11RenderTargetView* const GetTarget() const { return pTarget; }
		ID3D11RenderTargetView** const GetTargetPointer() { return &pTarget; }

		ID3D11DeviceContext* const GetDeviceContext() const { return pContext; }
		Ref<D3D11SwapChain> GetSwapChain() const { return m_SwapChain; }

		static D3D11Context* GetInstance() { return s_Instance; }
	private:
		HWND m_hWnd;
		GLFWwindow* m_WindowHandle;

		ID3D11Device* pDevice = nullptr;
		ID3D11DeviceContext* pContext = nullptr;
		ID3D11RenderTargetView* pTarget = nullptr;

		Ref<D3D11SwapChain> m_SwapChain;
	private:
		static D3D11Context* s_Instance;
	};

}