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
	static unsigned GetNowIndex() { return (m_FrameCnt - 1) % m_LatencyNum; };
	static unsigned GetOldBufferCnt() { return (m_FrameCnt - 1) % m_LatencyNum; };
	static unsigned GetFrameNo() { return m_FrameCnt; };
private:
	static unsigned m_LatencyNum;
	static unsigned m_FrameCnt;
};