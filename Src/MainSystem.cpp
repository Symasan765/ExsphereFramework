#include "MainSystem.h"
#include "TextureLoader.h"
#include "Input.h"

cMainSystem::cMainSystem(HINSTANCE hInst) : m_MainWindow(hInst)
{
	ZeroMemory(&m_Msg, sizeof(m_Msg));
	m_MainWindow.CreateMainWindow();
	m_DirectX12.CreateDevice(m_MainWindow.GetHWND());
	cTextureLoader::Init(cDirectX12::GetDevice());


	m_CommandManager.Create(cDirectX12::GetDevice(),cDirectX12::GetDxgiFactory(),cMainWindow::GetHWND(), WindowOptions::g_WindowSizeX, WindowOptions::g_WindowSizeY,DrawParam::g_MaxFrameLatency);
	m_MainWindow.CreateRenderBuffer(m_CommandManager.GetSwapChain());
	m_CommandManager.SetNowTarget(cMainWindow::GetDescHeapRtv(),cMainWindow::GetDescHeapDsv());


	Input::cKeyboard::getInstance()->InitKeyboard(cMainWindow::GetHInstance(), cMainWindow::GetHWND());
}

cMainSystem::~cMainSystem()
{
}

void cMainSystem::SystemLoop()
{
	m_TimeCheck.TimerStart();
	m_MainLoop.Create();

	do
	{
		if (PeekMessage(&m_Msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&m_Msg);
			DispatchMessage(&m_Msg);
		}
		else
		{
			m_TimeCheck.TimerEnd();		// ���[�v�O�Ɍv�����J�n���Ă���̂Ő�Ɏ~�߂�
			InputDataUpdate();	// ���͍X�V

			Update(m_TimeCheck.GetProcessingTime());
			m_TimeCheck.TimerStart();		// ���[�v�I����Ɍv�����ēx�J�n
		}
	} while (m_Msg.message != WM_QUIT);		//�Ȃɂ�����΂��̂܂܏����𔲂���
}

void cMainSystem::Destroy()
{
	// �V�X�e���n�̉���������s��
	Input::cKeyboard::getInstance()->UninitKeyboard();
	m_DirectX12.Destroy();
}

void cMainSystem::Update(float delta_time)
{
	// �t���[���A�b�v�f�[�g����
	m_FrameCnt.Update();

	//�����Ƀ��C������
	m_MainLoop.ExeMainLoop(delta_time);
	m_CommandManager.CommandBuild(cMainWindow::GetBuffer(cFrameCnt::GetNowIndex()), cFrameCnt::GetFrameNo(), cFrameCnt::GetNowIndex());

	// TODO ����A��L�̃A�b�v�f�[�g�ƁA�����̕`�敔������񉻂�����
	m_CommandManager.CommandQueueExe(cFrameCnt::GetFrameNo());
}

void cMainSystem::InputDataUpdate()
{
	Input::cKeyboard::getInstance()->UpdateKeyboard();
	Input::XInput::getInstance()->UpdateGamePad();
}
