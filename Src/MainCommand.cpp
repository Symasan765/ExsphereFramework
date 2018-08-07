#include "MainCommand.h"

cMainCommand::cMainCommand()
{
	m_Allocators = nullptr;
}

cMainCommand::~cMainCommand()
{
}

void cMainCommand::Init(ID3D12Device * dev)
{
	m_Allocators.reset(new cMainCommandAllocators);
	m_Lists.reset(new cMainCommandLists);

	m_Allocators->Create(dev);
	m_Lists->Create(dev, m_Allocators->GetBeginAlloc().Get());
}
