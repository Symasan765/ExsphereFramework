#include "MainCommand.h"

cMainCommand::cMainCommand()
{
}

cMainCommand::~cMainCommand()
{
}

void cMainCommand::Create(ID3D12Device * dev)
{
	m_Allocators.reset(new cMainCommandAllocators);
	m_Lists.reset(new cMainCommandLists);

	m_Allocators->Create(dev);
	m_Lists->Create(dev, m_Allocators->GetBeginAlloc().Get());
}

Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cMainCommand::GetSelectAlloc(UINT vertical, UINT Horizontal)
{
	return m_Allocators->GetSelectAlloc(vertical, Horizontal);
}
