/*=================================================
//								FILE :  GameLoop.cpp
//		�t�@�C������ :
//		�Q�[�����[�v�𐧌䂷��N���X���`����B
//		���̃N���X�ł͍X�V�ƕ`��̕��񏈗����������邱�ƁA
//		���[�v��Forward+�����_�����O�ɑΉ�������
//									���{ �Y�V��
=================================================*/
#include "GameLoop.h"

cMainLoop::cMainLoop(HINSTANCE _hInst) : m_MainWindow(_hInst)
{
	ZeroMemory(&m_Msg, sizeof(m_Msg));
	m_MainWindow.CreateMainWindow(_hInst);
}

void cMainLoop::ExecuteGameLoop()
{
	do
	{
		if (PeekMessage(&m_Msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&m_Msg);
			DispatchMessage(&m_Msg);
		}
		else
		{
			//�����Ƀ��C������
			MainLoop();
		}
	} while (m_Msg.message != WM_QUIT);		//�Ȃɂ�����΂��̂܂܏����𔲂���
}

/// <summary>
/// ���͍X�V��A�b�v�f�[�g�A�`����Ăяo���匳
/// </summary>
void cMainLoop::MainLoop()
{
	// TODO ���[�v�̍ŏ��œ��͏������s��

	// TODO �X�V�����ƕ`�揈�������ő��点��

	// TODO �}�l�[�W���[�n�̉�������m�F���s��
}
