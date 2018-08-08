#include "FrameCnt.h"
#include "DrawParam.h"

unsigned cFrameCnt::m_LatencyNum = DrawParam::g_MaxFrameLatency;
unsigned cFrameCnt::m_FrameCnt = 0;

cFrameCnt::cFrameCnt()
{
}

void cFrameCnt::Update()
{
	m_FrameCnt++;
}
