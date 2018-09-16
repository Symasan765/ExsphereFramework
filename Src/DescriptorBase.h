/*=================================================
//								FILE :	DescriptorBase.h
//		�t�@�C������ :
//		�f�B�X�N���v�^�[���������߂̊��N���X
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include "DirectX12.h"

class cDescriptorBase
{
public:
	cDescriptorBase() = default;
	virtual ~cDescriptorBase() = default;

	void CreateHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT num, UINT NodeMask = 0);

protected:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DescHeap = nullptr;
	UINT m_DescriptorHandleIncrementSize = 0;
};