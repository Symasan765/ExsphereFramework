#include "MainCommand.h"

cMainCommand::cMainCommand()
{
}

cMainCommand::~cMainCommand()
{
}

void cMainCommand::Create(ID3D12Device * dev, Microsoft::WRL::ComPtr<IDXGIFactory2> dxgi, HWND hwnd, const UINT width, const UINT heigit, const UINT bufferNum)
{
	m_Param.dev = dev;
	m_Param.dxgi = dxgi;
	m_Param.hwnd = hwnd;
	m_Param.width = width;
	m_Param.heigit = heigit;
	m_Param.bufferNum = bufferNum;
	Init();
}

void cMainCommand::Init()
{
	m_Allocators.reset(new cMainCommandAllocators);
	m_Lists.reset(new cMainCommandLists);
	m_SwapChain.reset(new cMainSwapChain);

	m_Allocators->Create(m_Param.dev);
	m_Lists->Create(m_Param.dev, m_Allocators->GetBeginAlloc().Get());
}
