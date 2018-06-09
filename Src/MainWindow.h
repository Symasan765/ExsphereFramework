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
#include <wrl/client.h>
#include <d3d12.h>
#include "DrawCommand.h"

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

class cMainWindow {
public:
	cMainWindow(HINSTANCE _hInst);
	~cMainWindow();

	static inline HWND GetHWND() { return m_hWindow; };
	static inline HINSTANCE GetHInstance() { return m_hInstance; };

	HRESULT CreateMainWindow();
	void CreateRenderBuffer();
private:
	void CreateRenderBufferDescriptor();

	static HWND m_hWindow;
	static HINSTANCE m_hInstance;

	static Microsoft::WRL::ComPtr<ID3D12Resource> m_D3DBuffer[WindowOptions::g_FrameBuuferNum];
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DescHeapRtv;	// レンダーターゲット用
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DescHeapDsv;	// デプスステンシル用
	static Microsoft::WRL::ComPtr<ID3D12Resource> mDsvResource;
};