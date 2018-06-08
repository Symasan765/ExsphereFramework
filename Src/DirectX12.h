#pragma once
#include <Windows.h>
#include <tchar.h>
#include <wrl/client.h>
#include <stdexcept>
#include <ppl.h>
#include <dxgi1_3.h>
#include <d3d12.h>
#include <d3dcompiler.h>

class cDirectX12 {
public:
	cDirectX12(HWND hWnd);
	~cDirectX12();
private:
	ID3D12Device * m_Device;
	Microsoft::WRL::ComPtr<IDXGIFactory2> m_DxgiFactory;
};