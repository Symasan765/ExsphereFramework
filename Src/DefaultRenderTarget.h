/*=================================================
//								FILE :	DefaultRenderTarget.h
//		ファイル説明 :
//		デフォルトの描画バッファ、深度ステンシルバッファを作成する
//
//									松本 雄之介
=================================================*/
#pragma once
#include "DirectX12.h"
#include "WindowParam.h"

class cDefaultRenderTarget
{
public:
	cDefaultRenderTarget() = default;
	~cDefaultRenderTarget() = default;
	void Create(Microsoft::WRL::ComPtr<IDXGISwapChain1> SwapChain);
	Microsoft::WRL::ComPtr<ID3D12Resource> GetBuffer(const int no);
	inline Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescHeapRtv() { return m_DescHeapRtv; };
	inline Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescHeapDsv() { return m_DescHeapDsv; };
private:
	void CreateRenderBufferDescriptor();

	Microsoft::WRL::ComPtr<ID3D12Resource> m_D3DBuffer[WindowOptions::g_FrameBuuferNum];
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DescHeapRtv;	// レンダーターゲット用
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DescHeapDsv;	// デプスステンシル用
	Microsoft::WRL::ComPtr<ID3D12Resource> mDsvResource;
};