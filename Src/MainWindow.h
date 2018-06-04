/*=================================================
//								FILE : MainWindow.h
//		ファイル説明 :
//		描画されるメインウィンドウの処理を行うクラス。
//		メインウィンドウはサイズ変更不可、
//		フレームレート60fpsとして処理を行う.
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

	constexpr CHAR		g_szWndClass[] = "ExsphereFramework";

	// ウィンドウスタイル
	constexpr DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_DLGFRAME;

	// 解像度
	constexpr int g_WindowSizeX = 1920;
	constexpr int g_WindowSizeY = 1080;
}

class cMainWindow {
public:
	cMainWindow(HINSTANCE _hInst);
	~cMainWindow();

	inline HWND GetHWND() { return m_hWindow; };
	inline HINSTANCE GetHInstance() { return m_hInstance; };

	HRESULT CreateMainWindow();
private:
	HWND m_hWindow;
	HINSTANCE m_hInstance;
};