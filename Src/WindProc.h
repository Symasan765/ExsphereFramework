/*=================================================
//								FILE : WindProc.h
//		�t�@�C������ :
//		�E�B���h�E�v���V�[�W���̒�`�A�������L�q����
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include <Windows.h>
#include "DirectX12.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_dx12.h"

/*-------------------------------------------
�E�B���h�E����
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
			// �E�C���h�E�����
			PostQuitMessage(0);
			return 0;
		}
		case WM_KEYDOWN: {
				// �L�[���͂̏���
				switch (wParam)
				{
				case VK_ESCAPE:	// [ESC]�L�[�ŃE�C���h�E�����
					PostMessage(hWnd, WM_CLOSE, 0, 0);
					break;
				}
		}
	}
	// �f�t�H���g����
	return DefWindowProc(hWnd, msg, wParam, lParam);
}