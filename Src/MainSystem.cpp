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
			m_TimeCheck.TimerEnd();		// ループ前に計測を開始しているので先に止める
			InputDataUpdate();	// 入力更新

			Update(m_TimeCheck.GetProcessingTime());
			m_TimeCheck.TimerStart();		// ループ終了後に計測を再度開始
		}
	} while (m_Msg.message != WM_QUIT);		//なにかあればそのまま処理を抜ける
}

void cMainSystem::Destroy()
{
	// システム系の解放処理を行う
	Input::cKeyboard::getInstance()->UninitKeyboard();
	m_DirectX12.Destroy();
}

void cMainSystem::Update(float delta_time)
{
	// フレームアップデート処理
	m_FrameCnt.Update();

	//ここにメイン処理
	m_MainLoop.ExeMainLoop(delta_time);
	m_CommandManager.CommandBuild(cMainWindow::GetBuffer(cFrameCnt::GetNowIndex()), cFrameCnt::GetFrameNo(), cFrameCnt::GetNowIndex());

	// TODO 今後、上記のアップデートと、ここの描画部分を並列化させる
	m_CommandManager.CommandQueueExe(cFrameCnt::GetFrameNo());
}

void cMainSystem::InputDataUpdate()
{
	Input::cKeyboard::getInstance()->UpdateKeyboard();
	Input::XInput::getInstance()->UpdateGamePad();
}
