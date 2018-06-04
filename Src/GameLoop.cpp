/*=================================================
//								FILE :  GameLoop.cpp
//		ファイル説明 :
//		ゲームループを制御するクラスを定義する。
//		このクラスでは更新と描画の並列処理を実現すること、
//		ループをForward+レンダリングに対応させる
//									松本 雄之介
=================================================*/
#include "GameLoop.h"

cMainLoop::cMainLoop(HINSTANCE _hInst) : m_MainWindow(_hInst)
{
	ZeroMemory(&m_Msg, sizeof(m_Msg));
	m_MainWindow.CreateMainWindow(_hInst);
}

void cMainLoop::ExecuteGameLoop()
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
			MainLoop();
		}
	} while (m_Msg.message != WM_QUIT);		//なにかあればそのまま処理を抜ける
}

/// <summary>
/// 入力更新やアップデート、描画を呼び出す大元
/// </summary>
void cMainLoop::MainLoop()
{
	// TODO ループの最初で入力処理を行う

	// TODO 更新処理と描画処理を並列で走らせる

	// TODO マネージャー系の解放処理確認を行う
}
