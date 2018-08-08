#include "ResourceCreateCommand.h"
#include "Utility.h"

void cResourceCreateCommand::Init(ID3D12Device * dev)
{
	InitQueue(dev);
	InitAlloc(dev);
	InitList(dev);
}

void cResourceCreateCommand::InitQueue(ID3D12Device * dev)
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	CheckHR(dev->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(m_CommandQueue.ReleaseAndGetAddressOf())));
}

void cResourceCreateCommand::InitAlloc(ID3D12Device * dev)
{
	CheckHR(dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_Allocator.ReleaseAndGetAddressOf())));
}

void cResourceCreateCommand::InitList(ID3D12Device * dev)
{
	CheckHR(dev->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_Allocator.Get(),
		nullptr,
		IID_PPV_ARGS(m_List.ReleaseAndGetAddressOf())));
	m_List->Close();		// ã‚Æ“¯‚¶‚­•Â‚¶‚Ä‚¨‚­
}
