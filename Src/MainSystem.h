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
#include "DirectX11.h"

class cMainSystem {
public:
	cMainSystem(HINSTANCE hInst);
	~cMainSystem();

	void SystemLoop();
private:
	cMainWindow m_MainWindow;
	cGameLoop m_MainLoop;
	cDirectX11 m_DirectX11;

	MSG m_Msg;	//メッセージ構造体
};