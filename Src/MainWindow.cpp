/*=================================================
//								FILE : MainWindow.cpp
//		ファイル説明 :
//		描画されるメインウィンドウの処理を行うクラス
//
//									松本 雄之介
=================================================*/
#include "MainWindow.h"
#include "WindProc.h"
#include "Utility.h"
#include <d3dx12.h>

using namespace WindowOptions;

HWND cMainWindow::m_hWindow = nullptr;
HINSTANCE cMainWindow::m_hInstance = nullptr;
cDefaultRenderTarget cMainWindow::m_RenderTarget;

cMainWindow::cMainWindow(HINSTANCE _hInst)
{
	// アプリケーションのインスタンス ハンドルを保存
	m_hInstance = _hInst;
	m_hWindow = nullptr;
}

cMainWindow::~cMainWindow()
{
	// ウインドウ クラスの登録解除
	UnregisterClass(WindowOptions::g_szWndClass, m_hInstance);
}

Microsoft::WRL::ComPtr<ID3D12Resource> cMainWindow::GetBuffer(const int no)
{
	return m_RenderTarget.GetBuffer(no);
}

HRESULT cMainWindow::CreateMainWindow()
{
	// ウインドウ クラスの登録
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WindowOptions::g_szWndClass;

	if (!RegisterClass(&wc))
		return E_FAIL;

	// メイン ウインドウ作成
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = WindowOptions::g_WindowSizeX;
	rect.bottom = WindowOptions::g_WindowSizeY;
	AdjustWindowRect(&rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_DLGFRAME, TRUE);

	rect.top = 0.0f;		// これを無くすとバー位置を画面外へ追いやり、フルスクリーンのような状態にできる
	m_hWindow = CreateWindow(WindowOptions::g_szWndClass, WindowOptions::g_szAppTitle,
		WindowOptions::dwStyle,
		rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, m_hInstance, NULL);

	if (m_hWindow == NULL)
		return E_FAIL;

	// ウインドウ表示
	ShowWindow(m_hWindow, SW_SHOWNORMAL);
	UpdateWindow(m_hWindow);

	return S_OK;
}

void cMainWindow::CreateRenderBuffer(Microsoft::WRL::ComPtr<IDXGISwapChain1> SwapChain)
{
	m_RenderTarget.Create(SwapChain);
}