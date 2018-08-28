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

	MSG m_Msg;	//���b�Z�[�W�\����
};