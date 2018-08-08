/*=================================================
//								FILE :	MainSwapChain.h
//		ファイル説明 :
//		メインで利用するスワップチェインの定義
//
//									松本 雄之介
=================================================*/
#pragma once
#include "DirectX12.h"

class cMainSwapChain
{
public:
	cMainSwapChain() = default;
	~cMainSwapChain() = default;
	void Create(Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue, Microsoft::WRL::ComPtr<IDXGIFactory2> dxgi,HWND hwnd,const UINT width, const UINT heigit, const UINT bufferNum);
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain1> m_SwapChain;
};