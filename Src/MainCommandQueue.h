/*=================================================
//								FILE :	MainCommandQueue.h
//		ファイル説明 :
//		メインで利用するコマンドキューの作成を行う
//
//									松本 雄之介
=================================================*/
#pragma once
#include "DirectX12.h"

class cMainCommandQueue {
public:
	cMainCommandQueue() = default;
	~cMainCommandQueue() = default;
	void Create(ID3D12Device* dev);
private:
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CommandQueue;
};