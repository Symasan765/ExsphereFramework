#include "CommandManager.h"

void cCommandManager::Create(ID3D12Device * dev, Microsoft::WRL::ComPtr<IDXGIFactory2> dxgi, HWND hwnd, const UINT width, const UINT heigit, const UINT bufferNum)
{
	m_Param.dev = dev;
	m_Param.dxgi = dxgi;
	m_Param.hwnd = hwnd;
	m_Param.width = width;
	m_Param.heigit = heigit;
	m_Param.bufferNum = bufferNum;
}

void cCommandManager::Init()
{
	m_Command.reset(new cMainCommand);
	m_Queue.reset(new cMainCommandQueue);
	m_SwapChain.reset(new cMainSwapChain);

	m_Command->Create(m_Param.dev);
	m_Queue->Create(m_Param.dev);
	m_SwapChain->Create(m_Param.dev, m_Param.dxgi, m_Param.hwnd, m_Param.width, m_Param.heigit, m_Param.bufferNum);
}
