/*=================================================
//								FILE :  GameLoop.cpp
//		ファイル説明 :
//		ゲームループを制御するクラスを定義する。
//		このクラスでは更新と描画の並列処理を実現すること、
//		ループをForward+レンダリングに対応させる
//									松本 雄之介
=================================================*/
#include "GameLoop.h"
#include "JobScheduler.h"

/// <summary>
/// 入力更新やアップデート、描画を呼び出す大元
/// </summary>
void cGameLoop::ExeMainLoop(float delta_time)
{
	JobExe(delta_time);
	m_MainScene.Rendering();
}

void cGameLoop::Create()
{
	m_MainScene.Create();
}

void cGameLoop::JobExe(float delta_time)
{
	auto JobScheduler = JobScheduler::Instance();
	JobScheduler->Execute(delta_time);
}