#include "MainSystem.h"

cMainSystem::cMainSystem(HINSTANCE hInst) : m_MainWindow(hInst)
{
	ZeroMemory(&m_Msg, sizeof(m_Msg));
	m_MainWindow.CreateMainWindow();
	m_DirectX12.CreateDevice(m_MainWindow.GetHWND());
	m_DrawCommand.Init();
	m_MainWindow.CreateRenderBuffer();
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
			//ここにメイン処理
			m_MainLoop.ExeMainLoop();
			m_DrawCommand.CommandListBuild();
			// TODO 今後、上記のアップデートと、ここの描画部分を並列化させる
			m_DrawCommand.CommandQueueExe();
		}
	} while (m_Msg.message != WM_QUIT);		//なにかあればそのまま処理を抜ける
}

void cMainSystem::Destroy()
{
	// システム系の解放処理を行う
	m_DrawCommand.Destroy();
	m_DirectX12.Destroy();
}
