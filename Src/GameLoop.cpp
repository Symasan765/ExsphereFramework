/*=================================================
//								FILE :  GameLoop.cpp
//		�t�@�C������ :
//		�Q�[�����[�v�𐧌䂷��N���X���`����B
//		���̃N���X�ł͍X�V�ƕ`��̕��񏈗����������邱�ƁA
//		���[�v��Forward+�����_�����O�ɑΉ�������
//									���{ �Y�V��
=================================================*/
#include "GameLoop.h"
#include "JobScheduler.h"

/// <summary>
/// ���͍X�V��A�b�v�f�[�g�A�`����Ăяo���匳
/// </summary>
void cGameLoop::ExeMainLoop(float delta_time)
{
	JobExe(delta_time);
}

void cGameLoop::Create()
{
	m_MainScene.Create();
}

void cGameLoop::JobExe(float delta_time)
{
	auto JobScheduler = JobScheduler::Instance();
	JobScheduler->Execute(delta_time);
}