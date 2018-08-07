/*=================================================
//								FILE :	MainCommandLists.h
//		�t�@�C������ :
//		�Q�[�����[�v���Ń��C���Ɏg�p����R�}���h���X�g���`����
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include "DirectX12.h"
#include "DrawParam.h"

class cMainCommandLists {
public:
	cMainCommandLists() = default;
	~cMainCommandLists() = default;
	void Create(ID3D12Device* dev,ID3D12CommandAllocator* alloc);
private:
	void MainCreate(ID3D12Device* dev, ID3D12CommandAllocator* alloc);
	void ProlCreate(ID3D12Device* dev, ID3D12CommandAllocator* alloc);
	void EpirCreate(ID3D12Device* dev, ID3D12CommandAllocator* alloc);
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandList[DrawParam::g_ThreadNum];
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandListProl;		// �`��O�����p�R�}���h���X�g
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandListEpir;		// �`��㏈���p�̃R�}���h���X�g(��Ƀe�N�X�`�����\�[�X�쐬�ȂǎG�p��S��)
};