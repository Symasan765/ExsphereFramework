#include "FenceObj.h"
#include "Utility.h"

cFenceObj::cFenceObj()
{
	CheckHR(cDirectX12::GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
	m_fenceValue = 1;

	// Create an event handle to use for frame synchronization.
	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (m_fenceEvent == nullptr)
	{
		CheckHR(HRESULT_FROM_WIN32(GetLastError()));
	}
}

cFenceObj::~cFenceObj()
{
	CloseHandle(m_fenceEvent);
}

void cFenceObj::WaitForPreviousFrame(ID3D12CommandQueue* m_commandQueue, DWORD MillSecTimeOut)
{
	const UINT64 fence = m_fenceValue;
	CheckHR(m_commandQueue->Signal(m_fence.Get(), fence));
	m_fenceValue++;

	// Wait until the previous frame is finished.
	if (m_fence->GetCompletedValue() < fence)
	{
		CheckHR(m_fence->SetEventOnCompletion(fence, m_fenceEvent));
		WaitForSingleObject(m_fenceEvent, MillSecTimeOut);
	}
}

void cFenceObj::WaitForPreviousFrame(UINT value, DWORD MillSecTimeOut)
{
	m_fence->SetEventOnCompletion(value, m_fenceEvent);
	DWORD wait = WaitForSingleObject(m_fenceEvent, MillSecTimeOut);	//シグナル状態になるのをここで待ってくれているっぽい

																	//シグナル状態にならなかった
	if (wait != WAIT_OBJECT_0)
		throw std::runtime_error("Failed WaitForSingleObject().");
}
