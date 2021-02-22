#include "sandpch.h"
#include "D3D11Context.h"

#include <comdef.h>

namespace Sand
{

	D3D11SwapChain::D3D11SwapChain(HWND hWnd)
		: m_WindowHandle(hWnd)
	{
	}

	// Remember to release the backBuffer once you are done!
	ID3D11Resource* D3D11SwapChain::GetBackBuffer()
	{
		ID3D11Resource* backBuffer = nullptr;
		D3D11_CALL(pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&backBuffer)));
		return backBuffer;
	}

	void D3D11SwapChain::Present(uint32_t syncInterval)
	{
		D3D11_CALL(pSwapChain->Present(syncInterval, 0u));
	}

	void D3D11SwapChain::CreateFromDeviceAndContext(ID3D11Device** device, ID3D11DeviceContext** context)
	{
		DXGI_SWAP_CHAIN_DESC description = {};
		description.BufferDesc.Width = 0;
		description.BufferDesc.Height = 0;
		description.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		description.BufferDesc.RefreshRate.Numerator = 0;
		description.BufferDesc.RefreshRate.Denominator = 0;
		description.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		description.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		description.SampleDesc.Count = 1;
		description.SampleDesc.Quality = 0;
		description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		description.BufferCount = 1;
		description.OutputWindow = m_WindowHandle;
		description.Windowed = true;
		description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		description.Flags = 0;

		// Create device, front / back buffers, & rendering context
		D3D11_CALL(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION,
			&description, &pSwapChain, device, nullptr, context));
	}

}