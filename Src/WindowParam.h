/*=================================================
//								FILE :	WindowParam.h
//		�t�@�C������ :
//		�E�B���h�E�Ɋւ���p�����[�^���L�q����
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include <Windows.h>

// ���C���E�B���h�E�̐ݒ荀��
namespace WindowOptions {
	// �E�B���h�E�^�C�g���o�^
#if defined(DEBUG) || defined(_DEBUG)
	constexpr CHAR		g_szAppTitle[] = "ExsphereFramework";
#else
	constexpr CHAR		g_szAppTitle[] = "ExsphereFramework";
#endif

	constexpr CHAR		g_szWndClass[] = "ExsphereFrameworkClass";

	// �E�B���h�E�X�^�C��
	constexpr DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_DLGFRAME;

	// �𑜓x
	constexpr int g_WindowSizeX = 1920;
	constexpr int g_WindowSizeY = 1080;


	constexpr unsigned g_FrameBuuferNum = 2;		// �`��o�b�t�@��
}