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

class cMainCommand {
public:
	cMainCommand();
	~cMainCommand();
	void Create(ID3D12Device * dev);
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetSelectAlloc(UINT vertical, UINT Horizontal);
private:
	std::unique_ptr<cMainCommandAllocators> m_Allocators;
	std::unique_ptr<cMainCommandLists> m_Lists;
};