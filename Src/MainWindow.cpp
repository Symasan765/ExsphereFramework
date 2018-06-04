/*=================================================
//								FILE : MainWindow.cpp
//		ファイル説明 :
//		描画されるメインウィンドウの処理を行うクラス
//
//									松本 雄之介
=================================================*/
#include "MainWindow.h"
#include "WindProc.h"
#include <dxerr.h>

cMainWindow::cMainWindow(HINSTANCE _hInst)
{
	CreateMainWindow(_hInst);
}

cMainWindow::~cMainWindow()
{
	// ウインドウ クラスの登録解除
	UnregisterClass(WindowOptions::g_szWndClass, m_hInstance);
}

HRESULT cMainWindow::CreateMainWindow(HINSTANCE _hInst)
{
	// アプリケーションのインスタンス ハンドルを保存
	m_hInstance = _hInst;

	// ウインドウ クラスの登録
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _hInst;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WindowOptions::g_szWndClass;

	if (!RegisterClass(&wc))
		return DXTRACE_ERR("CreateMainWindow", GetLastError());

	// メイン ウインドウ作成
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = WindowOptions::g_WindowSizeX;
	rect.bottom = WindowOptions::g_WindowSizeY;
	AdjustWindowRect(&rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_DLGFRAME, TRUE);

	m_hWindow = CreateWindow(WindowOptions::g_szWndClass, WindowOptions::g_szAppTitle,
		WindowOptions::dwStyle,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, _hInst, NULL);

	if (m_hWindow == NULL)
		return DXTRACE_ERR("CreateMainWindow", GetLastError());

	// ウインドウ表示
	ShowWindow(m_hWindow, SW_SHOWNORMAL);
	UpdateWindow(m_hWindow);

	return S_OK;
}
