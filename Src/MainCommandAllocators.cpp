#include "MainCommandAllocators.h"
#include "Utility.h"

void cMainCommandAllocators::Create(ID3D12Device* dev)
{
	for (auto& LatencyArray : m_CommandAllocator)
	{
		for (auto& ThreadArray : LatencyArray)
		{
			CheckHR(dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(ThreadArray.ReleaseAndGetAddressOf())));
		}
	}
}

Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cMainCommandAllocators::GetBeginAlloc()
{
	// æ“ª‚ğ•Ô‚·‚Ì‚Å0,0
	return m_CommandAllocator[0][0];
}
