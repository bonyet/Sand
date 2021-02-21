#pragma once

#include <d3d11.h>
#include <Windows.h>

namespace Sand
{

	class D3D11SwapChain
	{
	public:
		D3D11SwapChain(HWND hWnd);

		IDXGISwapChain* AsD3D11Resource() { return pSwapChain; }
		IDXGISwapChain** AsD3D11ResourcePointer() { return &pSwapChain; }

		const DXGI_SWAP_CHAIN_DESC& GetDescription() const { return m_Description; }

		ID3D11Resource* GetBackBuffer();

		inline void Release() const { pSwapChain->Release(); }
		void Present(uint32_t syncInterval);
	private:
		IDXGISwapChain* pSwapChain = nullptr;
		DXGI_SWAP_CHAIN_DESC m_Description;
	};

}