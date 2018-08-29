#include "MainCommandQueue.h"
#include "Utility.h"

void cMainCommandQueue::Create(ID3D12Device * dev)
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	CheckHR(dev->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(m_CommandQueue.ReleaseAndGetAddressOf())));
}

void cMainCommandQueue::Exe(ID3D12CommandList** pLists, unsigned listNum)
{
	m_CommandQueue->ExecuteCommandLists(listNum, pLists);
}
