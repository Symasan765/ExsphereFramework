/*=================================================
//								FILE :	DrawParam.h
//		ファイル説明 :
//		描画系のパラメータを定義しておく
//
//									松本 雄之介
=================================================*/
#pragma once

namespace DrawParam {
	constexpr unsigned g_ThreadNum = 4;		// 並列処理スレッド数
	constexpr unsigned g_MaxFrameLatency = 2;	// 処理フレーム数
}