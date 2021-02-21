#include "sandpch.h"
#include "D3D11SwapChain.h"

namespace Sand
{

	D3D11SwapChain::D3D11SwapChain(HWND hWnd)
	{
		m_Description = {};
		m_Description.BufferDesc.Width = 0;
		m_Description.BufferDesc.Height = 0;
		m_Description.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		m_Description.BufferDesc.RefreshRate.Numerator = 0;
		m_Description.BufferDesc.RefreshRate.Denominator = 0;
		m_Description.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		m_Description.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		m_Description.SampleDesc.Count = 1;
		m_Description.SampleDesc.Quality = 0;
		m_Description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		m_Description.BufferCount = 1;
		m_Description.OutputWindow = hWnd;
		m_Description.Windowed = true;
		m_Description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		m_Description.Flags = 0;
	}
	
	D3D11SwapChain::~D3D11SwapChain()
	{
		pSwapChain->Release();
	}

	// Remember to release the backBuffer once you are done!
	ID3D11Resource* D3D11SwapChain::GetBackBuffer()
	{
		ID3D11Resource* backBuffer = nullptr;
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&backBuffer));
		return backBuffer;
	}

}