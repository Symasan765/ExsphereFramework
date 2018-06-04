#include "MainSystem.h"


cMainSystem::cMainSystem(HINSTANCE hInst) : m_MainWindow(hInst)
{
	ZeroMemory(&m_Msg, sizeof(m_Msg));
	m_MainWindow.CreateMainWindow();
	m_DirectX11.InitDirect3D(m_MainWindow.GetHWND());
}

cMainSystem::~cMainSystem()
{
}

void cMainSystem::SystemLoop()
{
	do
	{
		if (PeekMessage(&m_Msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&m_Msg);
			DispatchMessage(&m_Msg);
		}
		else
		{
			//‚±‚±‚ÉƒƒCƒ“ˆ—
			m_MainLoop.ExeMainLoop();
		}
	} while (m_Msg.message != WM_QUIT);		//‚È‚É‚©‚ ‚ê‚Î‚»‚Ì‚Ü‚Üˆ—‚ğ”²‚¯‚é
}
