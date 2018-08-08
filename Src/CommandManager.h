/*=================================================
//								FILE :	CommandManager.h
//		�t�@�C������ :
//		�`�揈�����܂Ƃ߂ĊǗ�����N���X
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include "MainCommand.h"
#include "MainSwapChain.h"
#include "MainCommandQueue.h"
#include "CommandParamStruct.h"

class cCommandManager
{
public:
	cCommandManager() = default;
	~cCommandManager() = default;
	void Create(ID3D12Device * dev, Microsoft::WRL::ComPtr<IDXGIFactory2> dxgi, HWND hwnd, const UINT width, const UINT heigit, const UINT bufferNum);
private:
	void Init();

	std::unique_ptr<cMainCommand> m_Command;
	std::unique_ptr<cMainCommandQueue> m_Queue;
	std::unique_ptr<cMainSwapChain> m_SwapChain;

	CommandParamStruct m_Param;
};
