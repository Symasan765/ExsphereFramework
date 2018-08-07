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

		m_CommandList[i]->Close();		// 作成直後は記録状態になっているので閉じておく
	}
}

void cMainCommandLists::ProlCreate(ID3D12Device * dev, ID3D12CommandAllocator * alloc)
{
	// 次にバッファのクリアなど描画前準備用のコマンドリストを作成する
	{
		CheckHR(dev->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			alloc,
			nullptr,
			IID_PPV_ARGS(m_CommandListProl.ReleaseAndGetAddressOf())));
		m_CommandListProl->Close();		// 上と同じく閉じておく
	}
}

void cMainCommandLists::EpirCreate(ID3D12Device * dev, ID3D12CommandAllocator * alloc)
{
	// 最後に後処理用のコマンドリストの作成
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
