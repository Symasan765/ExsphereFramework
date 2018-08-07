/*=================================================
//								FILE :	MainCommandQueue.h
//		�t�@�C������ :
//		���C���ŗ��p����R�}���h�L���[�̍쐬���s��
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include "DirectX12.h"

class cMainCommandQueue {
public:
	cMainCommandQueue() = default;
	~cMainCommandQueue() = default;
	void Create(ID3D12Device* dev);
private:
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CommandQueue;
};