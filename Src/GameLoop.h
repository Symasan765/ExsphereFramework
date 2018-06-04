/*=================================================
//								FILE :  GameLoop.h
//		ファイル説明 :
//		ゲームループを制御するクラスを定義する。
//		このクラスでは更新と描画の並列処理を実現すること、
//		ループをForward+レンダリングに対応させる
//									松本 雄之介
=================================================*/
#pragma once

class cMainLoop {
public:
	cMainLoop();
	~cMainLoop() = default;
	void ExecuteGameLoop();
private:
	void MainLoop();

	MSG m_Msg;				//メッセージ構造体
};