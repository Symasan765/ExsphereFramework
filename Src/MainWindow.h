/*=================================================
//								FILE : MainWindow.h
//		�t�@�C������ :
//		�`�悳��郁�C���E�B���h�E�̏������s���N���X�B
//		���C���E�B���h�E�̓T�C�Y�ύX�s�A
//		�t���[�����[�g60fps�Ƃ��ď������s��.
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

	constexpr CHAR		g_szWndClass[] = "ExsphereFramework";

	// �E�B���h�E�X�^�C��
	constexpr DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_DLGFRAME;

	// �𑜓x
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