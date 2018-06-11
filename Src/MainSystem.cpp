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
			//�����Ƀ��C������
			m_MainLoop.ExeMainLoop();
			m_DrawCommand.CommandListBuild();
			// TODO ����A��L�̃A�b�v�f�[�g�ƁA�����̕`�敔������񉻂�����
			m_DrawCommand.CommandQueueExe();
		}
	} while (m_Msg.message != WM_QUIT);		//�Ȃɂ�����΂��̂܂܏����𔲂���
}

void cMainSystem::Destroy()
{
	// �V�X�e���n�̉���������s��
	m_DrawCommand.Destroy();
	m_DirectX12.Destroy();
}
