/*=================================================
//								FILE :	MainSwapChain.h
//		�t�@�C������ :
//		���C���ŗ��p����X���b�v�`�F�C���̒�`
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include "DirectX12.h"

class cMainSwapChain
{
public:
	cMainSwapChain() = default;
	~cMainSwapChain() = default;
	void Create(Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue, Microsoft::WRL::ComPtr<IDXGIFactory2> dxgi,HWND hwnd,const UINT width, const UINT heigit, const UINT bufferNum);
	void Present();
	inline Microsoft::WRL::ComPtr<IDXGISwapChain1> Get() { return m_SwapChain; };
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain1> m_SwapChain;
};