/*=================================================
//								FILE : WindProc.h
//		ファイル説明 :
//		ウィンドウプロシージャの定義、処理を記述する
//
//									松本 雄之介
=================================================*/
#pragma once
#include <Windows.h>
#include "DirectX12.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_dx12.h"

/*-------------------------------------------
ウィンドウ処理
--------------------------------------------*/
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = S_OK;
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		return 0;
		case WM_DESTROY: {
			// ウインドウを閉じる
			PostQuitMessage(0);
			return 0;
		}
		case WM_KEYDOWN: {
				// キー入力の処理
				switch (wParam)
				{
				case VK_ESCAPE:	// [ESC]キーでウインドウを閉じる
					PostMessage(hWnd, WM_CLOSE, 0, 0);
					break;
				}
		}
	}
	// デフォルト処理
	return DefWindowProc(hWnd, msg, wParam, lParam);
}