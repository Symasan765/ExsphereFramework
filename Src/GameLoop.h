/*=================================================
//								FILE :  GameLoop.h
//		ファイル説明 :
//		ゲームループを制御するクラスを定義する。
//		このクラスでは更新と描画の並列処理を実現すること、
//		ループをForward+レンダリングに対応させる。
//		処理に必要なシステム系のクラスを保持することになる。
//									松本 雄之介
=================================================*/
#pragma once
#include "MainWindow.h"

class cMainLoop {
public:
	cMainLoop(HINSTANCE _hInst);
	~cMainLoop() = default;
	void ExecuteGameLoop();
private:
	void MainLoop();
	cMainWindow m_MainWindow;

	MSG m_Msg;				//メッセージ構造体
};