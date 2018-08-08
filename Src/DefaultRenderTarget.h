/*=================================================
//								FILE :	DefaultRenderTarget.h
//		�t�@�C������ :
//		�f�t�H���g�̕`��o�b�t�@�A�[�x�X�e���V���o�b�t�@���쐬����
//
//									���{ �Y�V��
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
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DescHeapRtv;	// �����_�[�^�[�Q�b�g�p
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DescHeapDsv;	// �f�v�X�X�e���V���p
	Microsoft::WRL::ComPtr<ID3D12Resource> mDsvResource;
};