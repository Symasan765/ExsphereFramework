/*=================================================
//								FILE :	FenceObj.h
//		�t�@�C������ :
//		�t�F���X�𗘗p���₷���悤�ɃN���X�����Ă���
//
//									���{ �Y�V��
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
	UINT64 m_fenceValue = 0;									// ���[�_�[�N���X���ێ�
};