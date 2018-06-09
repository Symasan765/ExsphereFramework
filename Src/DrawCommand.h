/*=================================================
//								FILE : DrawCommand.h
//		ファイル説明 :
//		DirectX12の描画コマンドシステム周りの機能を実装する。
//		システム的なマルチスレッドを包括する
//									松本 雄之介
=================================================*/
#pragma once
#include "DirectX12.h"

namespace DrawParam{
	constexpr int g_ThreadNum = 4;		// 並列処理スレッド数
	constexpr const unsigned MaxFrameLatency = 2;	// 処理フレーム数
}

class cDrawCommand {
public:
	cDrawCommand();
	~cDrawCommand();
private:
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_CommandAllocator[DrawParam::MaxFrameLatency][DrawParam::g_ThreadNum];
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CommandQueue;
};