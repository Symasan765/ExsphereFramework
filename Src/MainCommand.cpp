#include "MainCommand.h"

cMainCommand::cMainCommand()
{
	m_Allocators = nullptr;
}

cMainCommand::~cMainCommand()
{
}

void cMainCommand::Init()
{
	m_Allocators.reset(new cMainCommandAllocators);
	m_Lists.reset(new cMainCommandLists);
}
