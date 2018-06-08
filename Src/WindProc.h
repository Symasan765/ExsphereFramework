/*=================================================
//								FILE : WindProc.h
//		�t�@�C������ :
//		�E�B���h�E�v���V�[�W���̒�`�A�������L�q����
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include <Windows.h>

/*-------------------------------------------
�E�B���h�E����
--------------------------------------------*/
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = S_OK;

	switch (msg)
	{
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