/*=================================================
//								FILE :	WindowParam.h
//		ファイル説明 :
//		ウィンドウに関するパラメータを記述する
//
//									松本 雄之介
=================================================*/
#pragma once
#include <Windows.h>

// メインウィンドウの設定項目
namespace WindowOptions {
	// ウィンドウタイトル登録
#if defined(DEBUG) || defined(_DEBUG)
	constexpr CHAR		g_szAppTitle[] = "ExsphereFramework";
#else
	constexpr CHAR		g_szAppTitle[] = "ExsphereFramework";
#endif

	constexpr CHAR		g_szWndClass[] = "ExsphereFrameworkClass";

	// ウィンドウスタイル
	constexpr DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_DLGFRAME;

	// 解像度
	constexpr int g_WindowSizeX = 1920;
	constexpr int g_WindowSizeY = 1080;


	constexpr unsigned g_FrameBuuferNum = 2;		// 描画バッファ数
}