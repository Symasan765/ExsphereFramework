#include "DefaultRenderTarget.h"
#include "Utility.h"
#include <d3dx12.h>

using namespace WindowOptions;

void cDefaultRenderTarget::Create(Microsoft::WRL::ComPtr<IDXGISwapChain1> SwapChain)
{
	for (int i = 0; i < g_FrameBuuferNum; i++)
	{
		CheckHR(SwapChain->GetBuffer(i, IID_PPV_ARGS(m_D3DBuffer[i].ReleaseAndGetAddressOf())));
		m_D3DBuffer[i]->SetName(L"SwapChain_Buffer");
	}

	CreateRenderBufferDescriptor();

	// レンダーターゲットビューの作成
	auto rtvStep = cDirectX12::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	for (auto i = 0u; i < g_FrameBuuferNum; i++)
	{
		auto d = m_DescHeapRtv->GetCPUDescriptorHandleForHeapStart();
		d.ptr += i * rtvStep;		// バッファ格納位置アドレスをずらしている
		cDirectX12::GetDevice()->CreateRenderTargetView(m_D3DBuffer[i].Get(), nullptr, d);
	}

	// デプスステンシルビューの作成
	auto resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R32_TYPELESS, g_WindowSizeX, g_WindowSizeY, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL,
		D3D12_TEXTURE_LAYOUT_UNKNOWN, 0);
	D3D12_CLEAR_VALUE dsvClearValue;
	dsvClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	dsvClearValue.DepthStencil.Depth = 1.0f;
	dsvClearValue.DepthStencil.Stencil = 0;
	CheckHR(cDirectX12::GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), // No need to read/write by CPU
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&dsvClearValue,
		IID_PPV_ARGS(mDsvResource.ReleaseAndGetAddressOf())));
	mDsvResource->SetName(L"DepthTexture");

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.Texture2D.MipSlice = 0;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	cDirectX12::GetDevice()->CreateDepthStencilView(mDsvResource.Get(), &dsvDesc, m_DescHeapDsv->GetCPUDescriptorHandleForHeapStart());
}

Microsoft::WRL::ComPtr<ID3D12Resource> cDefaultRenderTarget::GetBuffer(const int no)
{
	return m_D3DBuffer[no % g_FrameBuuferNum];		// バッファ枚数を範囲内に収める
}

void cDefaultRenderTarget::CreateRenderBufferDescriptor()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.NumDescriptors = g_FrameBuuferNum;
	desc.NodeMask = 0;
	CheckHR(cDirectX12::GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(m_DescHeapRtv.ReleaseAndGetAddressOf())));

	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	desc.NumDescriptors = g_FrameBuuferNum;
	desc.NodeMask = 0;
	CheckHR(cDirectX12::GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(m_DescHeapDsv.ReleaseAndGetAddressOf())));
}
