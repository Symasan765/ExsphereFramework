/*=================================================
//								FILE :  GameLoop.h
//		�t�@�C������ :
//		�Q�[�����[�v�𐧌䂷��N���X���`����B
//		���̃N���X�ł͍X�V�ƕ`��̕��񏈗����������邱�ƁA
//		���[�v��Forward+�����_�����O�ɑΉ�������
//									���{ �Y�V��
=================================================*/
#pragma once

class cMainLoop {
public:
	cMainLoop();
	~cMainLoop() = default;
	void ExecuteGameLoop();
private:
	void MainLoop();

	MSG m_Msg;				//���b�Z�[�W�\����
};