/*=================================================
//								FILE :	MainCommandAllocators.h
//		ファイル説明 :
//		ゲームループ内でメインで利用されるコマンドアロケーターを
//		クラス化し、定義する
//									松本 雄之介
=================================================*/
#pragma once
#include "DirectX12.h"
#include "DrawParam.h"

class cMainCommandAllocators
{
public:
	cMainCommandAllocators() = default;
	~cMainCommandAllocators() = default;
	void Create(ID3D12Device* dev);
private:
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_CommandAllocator[DrawParam::g_MaxFrameLatency][DrawParam::g_ThreadNum];
};