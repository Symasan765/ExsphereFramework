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
	friend class cMainSystem;
	cDirectX12();
	~cDirectX12();
	inline static ID3D12Device* GetDevice() { return m_Device; };
private:
	void CreateDevice(HWND hWnd);
	void Destroy();
	static ID3D12Device * m_Device;
	static Microsoft::WRL::ComPtr<IDXGIFactory2> m_DxgiFactory;
};