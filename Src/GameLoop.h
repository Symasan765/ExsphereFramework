/*=================================================
//								FILE :  GameLoop.h
//		�t�@�C������ :
//		�Q�[�����[�v�𐧌䂷��N���X���`����B
//		���̃N���X�ł͍X�V�ƕ`��̕��񏈗����������邱�ƁA
//		���[�v��Forward+�����_�����O�ɑΉ�������B
//		�����ɕK�v�ȃV�X�e���n�̃N���X��ێ����邱�ƂɂȂ�B
//									���{ �Y�V��
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

	MSG m_Msg;				//���b�Z�[�W�\����
};