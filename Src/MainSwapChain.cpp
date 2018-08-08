#include "MainSwapChain.h"
#include "Utility.h"

void cMainSwapChain::Create(Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue, Microsoft::WRL::ComPtr<IDXGIFactory2> dxgi, 
	HWND hwnd, const UINT width,const UINT heigit,const UINT bufferNum)
{
	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Width = width;
	scDesc.Height = heigit;
	scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.SampleDesc.Count = 1;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = bufferNum;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	CheckHR(dxgi->CreateSwapChainForHwnd(queue.Get(), hwnd, &scDesc, nullptr, nullptr, m_SwapChain.ReleaseAndGetAddressOf()));
}
