/*=================================================
//								FILE :	FrameCnt.h
//		�t�@�C������ :
//		�t���[�����[�g���J�E���g����B
//		�g�p����o�b�t�@�Ȃǂɗ��p����
//									���{ �Y�V��
=================================================*/
#pragma once

class cFrameCnt {
public:
	cFrameCnt();	// �Q�[���Ƃ��Đ݂��Ă���o�b�t�@��
	~cFrameCnt() = default;
	void Update();
	static unsigned GetNowIndex() { return (m_FrameCnt - 1) % m_LatencyNum; };
	static unsigned GetOldBufferCnt() { return (m_FrameCnt - 1) % m_LatencyNum; };
	static unsigned GetFrameNo() { return m_FrameCnt; };
private:
	static unsigned m_LatencyNum;
	static unsigned m_FrameCnt;
};