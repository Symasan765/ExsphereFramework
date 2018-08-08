/*=================================================
//								FILE :	CommandManager.h
//		ファイル説明 :
//		描画処理をまとめて管理するクラス
//
//									松本 雄之介
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
