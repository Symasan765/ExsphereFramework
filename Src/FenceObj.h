/*=================================================
//								FILE :	FenceObj.h
//		ファイル説明 :
//		フェンスを利用しやすいようにクラス化しておく
//
//									松本 雄之介
=================================================*/
#pragma once
#include "DirectX12.h"
#include <wrl/client.h>

class cFenceObj {
public:
	cFenceObj();
	~cFenceObj();
	void WaitForPreviousFrame(ID3D12CommandQueue*,DWORD MillSecTimeOut = INFINITE);
	void WaitForPreviousFrame(UINT value, DWORD MillSecTimeOut = INFINITE);
private:
	Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
	HANDLE m_fenceEvent;
	UINT64 m_fenceValue = 0;									// ローダークラスが保持
};