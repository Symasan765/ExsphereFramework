/*=================================================
//								FILE :	FrameCnt.h
//		ファイル説明 :
//		フレームレートをカウントする。
//		使用するバッファなどに利用する
//									松本 雄之介
=================================================*/
#pragma once

class cFrameCnt {
public:
	cFrameCnt();	// ゲームとして設けているバッファ数
	~cFrameCnt() = default;
	void Update();
	inline unsigned GetNowBufferCnt() { return m_FrameCnt % m_LatencyNum; };
	inline unsigned GetOldBufferCnt() { return (m_FrameCnt - 1) % m_LatencyNum; };
	inline unsigned GetFrameNo() { return m_FrameCnt; };
private:
	unsigned m_LatencyNum;
	unsigned m_FrameCnt;
};