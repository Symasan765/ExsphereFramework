/*=================================================
//								FILE :	System.h
//		�t�@�C������ :
//		�Q�[���̊��ƂȂ�V�X�e���̊Ǘ����s���N���X���`����
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include "FrameCnt.h"

class cSystem {
public:
	friend class cMainSystem;
	cSystem();
	~cSystem() = default;
	static cFrameCnt m_FrameCnt;		// �t���[�������J�E���g����
private:
	void Update();
};