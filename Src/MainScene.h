/*=================================================
//								FILE :	MainScene.h
//		�t�@�C������ :
//		�ŏ��ɋN�������V�[���N���X�B2018/8/29���݁A������
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include "Actor.h"
#include "Game/Pacman.h"

class cMainScene
{
public:
	cMainScene();
	~cMainScene();
	void Create();
	void Rendering();
private:
	Actor m_Pac[3];
	Actor m_Stage;
	Actor m_Bait[8][6];
	cPacman m_Pacman;
};