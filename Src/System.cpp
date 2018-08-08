#include "System.h"

cFrameCnt cSystem::m_FrameCnt;

cSystem::cSystem()
{
}

void cSystem::Update()
{
	m_FrameCnt.Update();
}
