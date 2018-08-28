/*=================================================
//								FILE : MainSystem.h
//		ファイル説明 :
//		フレームワークのシステム処理部分に必要なクラスを
//		一元管理し、処理、実行することを目的としたクラス。
//
//									松本 雄之介
=================================================*/
#pragma once
#include "MainWindow.h"
#include "GameLoop.h"
#include "DirectX12.h"
#include "CommandManager.h"
#include "FrameCnt.h"
#include "TimeCheck.h"

class cMainSystem {
public:
	cMainSystem(HINSTANCE hInst);
	~cMainSystem();

	void SystemLoop();
	void Destroy();
private:
	void Update(float delta_time);

	cMainWindow m_MainWindow;
	cGameLoop m_MainLoop;
	cDirectX12 m_DirectX12;
	cCommandManager m_CommandManager;
	cFrameCnt m_FrameCnt;
	cTimeCheck m_TimeCheck;

	MSG m_Msg;	//メッセージ構造体
};