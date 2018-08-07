#include "FrameCnt.h"
#include "DrawCommand.h"

cFrameCnt::cFrameCnt() : m_LatencyNum(DrawParam::g_MaxFrameLatency)
{
	m_FrameCnt = 0;
}

void cFrameCnt::Update()
{
	m_FrameCnt++;
}
