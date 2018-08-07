/*=================================================
//								FILE :	MainCommandAllocators.h
//		�t�@�C������ :
//		�Q�[�����[�v���Ń��C���ŗ��p�����R�}���h�A���P�[�^�[��
//		�N���X�����A��`����
//									���{ �Y�V��
=================================================*/
#pragma once
#include "DirectX12.h"
#include "DrawParam.h"

class cMainCommandAllocators
{
public:
	cMainCommandAllocators() = default;
	~cMainCommandAllocators() = default;
	void Create(ID3D12Device* dev);
private:
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_CommandAllocator[DrawParam::g_MaxFrameLatency][DrawParam::g_ThreadNum];
};