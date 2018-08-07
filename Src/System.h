/*=================================================
//								FILE :	System.h
//		ファイル説明 :
//		ゲームの基底となるシステムの管理を行うクラスを定義する
//
//									松本 雄之介
=================================================*/
#pragma once
#include "FrameCnt.h"

class cSystem {
public:
	friend class cMainSystem;
	cSystem();
	~cSystem() = default;
	static cFrameCnt m_FrameCnt;		// フレーム数をカウントする
private:
	void Update();
};