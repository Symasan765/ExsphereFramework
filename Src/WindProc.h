/*=================================================
//								FILE : WindProc.h
//		ファイル説明 :
//		ウィンドウプロシージャの定義、処理を記述する
//
//									松本 雄之介
=================================================*/
#pragma once
#include <Windows.h>

/*-------------------------------------------
ウィンドウ処理
--------------------------------------------*/
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, UINT wParam, LONG lParam)
{
	HRESULT hr = S_OK;

	switch (msg)
	{
	case WM_DESTROY:
		// ウインドウを閉じる
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		// キー入力の処理
		switch (wParam)
		{
		case VK_ESCAPE:	// [ESC]キーでウインドウを閉じる
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
			break;
		}

		// デフォルト処理
		return DefWindowProc(hWnd, msg, wParam, lParam);
	};
}