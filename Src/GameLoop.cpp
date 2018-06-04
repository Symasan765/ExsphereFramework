/*=================================================
//								FILE :  GameLoop.cpp
//		�t�@�C������ :
//		�Q�[�����[�v�𐧌䂷��N���X���`����B
//		���̃N���X�ł͍X�V�ƕ`��̕��񏈗����������邱�ƁA
//		���[�v��Forward+�����_�����O�ɑΉ�������
//									���{ �Y�V��
=================================================*/
#include "GameLoop.h"
#include "DirectX11.h"

/// <summary>
/// ���͍X�V��A�b�v�f�[�g�A�`����Ăяo���匳
/// </summary>
void cGameLoop::ExeMainLoop()
{
	// TODO ���[�v�̍ŏ��œ��͏������s��


	// �`��^�[�Q�b�g�̃N���A
	const float ClearColor[4] = { 0.1f, 0.2f, 0.6f, 1.0f };
	//const float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	cDirectX11::GetContext()->ClearRenderTargetView(
		cDirectX11::GetRenderTargetView(), // �N���A����`��^�[�Q�b�g
		ClearColor);         // �N���A����l

	//�����_�����O�����ʂɐ؂�ւ�
	const auto tmp = cDirectX11::GetRenderTargetView();
	cDirectX11::GetContext()->OMSetRenderTargets(1, &tmp, true ? NULL : NULL);

	cDirectX11::GetSwapChain()->Present(1,	// ���������ōX�V����
		0);	// ��ʂ����ۂɍX�V����
	// TODO �X�V�����ƕ`�揈�������ő��点��

	// TODO �}�l�[�W���[�n�̉�������m�F���s��
}
