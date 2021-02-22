#pragma once

#include <d3d11.h>
#include <Windows.h>

namespace Sand
{

	class D3D11SwapChain
	{
	public:
		D3D11SwapChain(HWND hWnd);

		ID3D11Resource* GetBackBuffer();
		IDXGISwapChain** GetPP() { return &pSwapChain; }

		inline void Release() const { pSwapChain->Release(); }
		void Present(uint32_t syncInterval);

		void CreateFromDeviceAndContext(ID3D11Device** device, ID3D11DeviceContext** context);
	private:
		IDXGISwapChain* pSwapChain = nullptr;
		HWND m_WindowHandle;
	};

}