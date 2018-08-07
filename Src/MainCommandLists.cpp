#include "MainCommandLists.h"
#include "Utility.h"

void cMainCommandLists::Create(ID3D12Device* dev,ID3D12CommandAllocator* alloc)
{
	MainCreate(dev, alloc);
	ProlCreate(dev, alloc);
	EpirCreate(dev, alloc);
}

void cMainCommandLists::MainCreate(ID3D12Device * dev, ID3D12CommandAllocator * alloc)
{
	for (int i = 0; i < DrawParam::g_ThreadNum; i++)
	{
		CheckHR(dev->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			alloc,
			nullptr,
			IID_PPV_ARGS(m_CommandList[i].ReleaseAndGetAddressOf())));

		m_CommandList[i]->Close();		// �쐬����͋L�^��ԂɂȂ��Ă���̂ŕ��Ă���
	}
}

void cMainCommandLists::ProlCreate(ID3D12Device * dev, ID3D12CommandAllocator * alloc)
{
	// ���Ƀo�b�t�@�̃N���A�ȂǕ`��O�����p�̃R�}���h���X�g���쐬����
	{
		CheckHR(dev->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			alloc,
			nullptr,
			IID_PPV_ARGS(m_CommandListProl.ReleaseAndGetAddressOf())));
		m_CommandListProl->Close();		// ��Ɠ��������Ă���
	}
}

void cMainCommandLists::EpirCreate(ID3D12Device * dev, ID3D12CommandAllocator * alloc)
{
	// �Ō�Ɍ㏈���p�̃R�}���h���X�g�̍쐬
	{
		CheckHR(dev->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			alloc,
			nullptr,
			IID_PPV_ARGS(m_CommandListEpir.ReleaseAndGetAddressOf())));
		m_CommandListEpir->Close();
	}
}
