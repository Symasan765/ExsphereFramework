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
#include "MainScene.h"

class cGameLoop {
public:
	cGameLoop() = default;
	~cGameLoop() = default;
	void ExeMainLoop(float delta_time);
	void Create();
private:
	void JobExe(float delta_time);

	cMainScene m_MainScene;
};