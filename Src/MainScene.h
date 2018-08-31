/*=================================================
//								FILE :	MainScene.h
//		ファイル説明 :
//		最初に起動されるシーンクラス。2018/8/29現在、仮実装
//
//									松本 雄之介
=================================================*/
#pragma once
#include "Actor.h"

class cMainScene
{
public:
	cMainScene();
	~cMainScene();
	void Create();
	void Rendering();
private:
	Actor m_Actor[2];
};