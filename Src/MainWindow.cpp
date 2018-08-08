/*=================================================
//								FILE : MainWindow.cpp
//		�t�@�C������ :
//		�`�悳��郁�C���E�B���h�E�̏������s���N���X
//
//									���{ �Y�V��
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
	// �A�v���P�[�V�����̃C���X�^���X �n���h����ۑ�
	m_hInstance = _hInst;
	m_hWindow = nullptr;
}

cMainWindow::~cMainWindow()
{
	// �E�C���h�E �N���X�̓o�^����
	UnregisterClass(WindowOptions::g_szWndClass, m_hInstance);
}

Microsoft::WRL::ComPtr<ID3D12Resource> cMainWindow::GetBuffer(const int no)
{
	return m_RenderTarget.GetBuffer(no);
}

HRESULT cMainWindow::CreateMainWindow()
{
	// �E�C���h�E �N���X�̓o�^
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

	// ���C�� �E�C���h�E�쐬
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = WindowOptions::g_WindowSizeX;
	rect.bottom = WindowOptions::g_WindowSizeY;
	AdjustWindowRect(&rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_DLGFRAME, TRUE);

	rect.top = 0.0f;		// ����𖳂����ƃo�[�ʒu����ʊO�֒ǂ����A�t���X�N���[���̂悤�ȏ�Ԃɂł���
	m_hWindow = CreateWindow(WindowOptions::g_szWndClass, WindowOptions::g_szAppTitle,
		WindowOptions::dwStyle,
		rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, m_hInstance, NULL);

	if (m_hWindow == NULL)
		return E_FAIL;

	// �E�C���h�E�\��
	ShowWindow(m_hWindow, SW_SHOWNORMAL);
	UpdateWindow(m_hWindow);

	return S_OK;
}

void cMainWindow::CreateRenderBuffer(Microsoft::WRL::ComPtr<IDXGISwapChain1> SwapChain)
{
	m_RenderTarget.Create(SwapChain);
}