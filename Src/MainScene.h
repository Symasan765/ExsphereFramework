/*=================================================
//								FILE :	MainScene.h
//		�t�@�C������ :
//		�ŏ��ɋN�������V�[���N���X�B2018/8/29���݁A������
//
//									���{ �Y�V��
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