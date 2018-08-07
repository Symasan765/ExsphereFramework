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
	inline unsigned GetNowBufferCnt() { return m_FrameCnt % m_LatencyNum; };
	inline unsigned GetOldBufferCnt() { return (m_FrameCnt - 1) % m_LatencyNum; };
	inline unsigned GetFrameNo() { return m_FrameCnt; };
private:
	unsigned m_LatencyNum;
	unsigned m_FrameCnt;
};