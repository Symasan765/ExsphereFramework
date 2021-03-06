#include "DirectX12.h"
#include "Utility.h"

ID3D12Device* cDirectX12::m_Device = nullptr;
Microsoft::WRL::ComPtr<IDXGIFactory4> cDirectX12::m_DxgiFactory;

cDirectX12::cDirectX12()
{
}

cDirectX12::~cDirectX12()
{
}

void cDirectX12::CreateDevice(HWND hWnd)
{
	// DXGIファクトリーを作成する
#if _DEBUG
	CheckHR(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(m_DxgiFactory.ReleaseAndGetAddressOf())));
#else
	CHK(CreateDXGIFactory2(0, IID_PPV_ARGS(mDxgiFactory.ReleaseAndGetAddressOf())));
#endif /* _DEBUG */


#if _DEBUG
	// デバッグレイヤーを有効にする
	ID3D12Debug* debug = nullptr;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	if (debug)
	{
		debug->EnableDebugLayer();
		debug->Release();
		debug = nullptr;
	}
#endif /* _DEBUG */

	ID3D12Device* dev;
	CheckHR(D3D12CreateDevice(
		nullptr,
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&dev)));
	m_Device = dev;
}

void cDirectX12::Destroy()
{
	SAFE_RELEASE(m_Device);
}
