/*=================================================
//								FILE :	MainCommand.h
//		ファイル説明 :
//		ゲーム中で利用するメインの描画コマンドを管理するクラス
//
//									松本 雄之介
=================================================*/
#pragma once
#include<memory>
#include "MainCommandAllocators.h"
#include "MainCommandLists.h"
#include "MainSwapChain.h"
#include "CommandParamStruct.h"

class cMainCommand {
public:
	cMainCommand();
	~cMainCommand();
	void Create(ID3D12Device * dev, Microsoft::WRL::ComPtr<IDXGIFactory2> dxgi, HWND hwnd, const UINT width, const UINT heigit, const UINT bufferNum);
	
private:
	void Init();
	std::unique_ptr<cMainCommandAllocators> m_Allocators;
	std::unique_ptr<cMainCommandLists> m_Lists;
	std::unique_ptr<cMainSwapChain> m_SwapChain;

	CommandParamStruct m_Param;
};