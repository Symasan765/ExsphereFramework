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