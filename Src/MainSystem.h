/*=================================================
//								FILE : MainSystem.h
//		�t�@�C������ :
//		�t���[�����[�N�̃V�X�e�����������ɕK�v�ȃN���X��
//		�ꌳ�Ǘ����A�����A���s���邱�Ƃ�ړI�Ƃ����N���X�B
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include "MainWindow.h"
#include "GameLoop.h"

class cMainSystem {
public:
	cMainSystem(HINSTANCE hInst);
	~cMainSystem();

	void SystemLoop();
	void Destroy();
private:
	cMainWindow m_MainWindow;
	cGameLoop m_MainLoop;

	MSG m_Msg;	//���b�Z�[�W�\����
};