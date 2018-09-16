#include "DescriptorBase.h"
#include "Utility.h"
#include <d3dx12.h>

void cDescriptorBase::CreateHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT num, UINT NodeMask = 0)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = type;
	desc.NumDescriptors = num;
	desc.NodeMask = NodeMask;
	CheckHR(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(m_DescHeap.ReleaseAndGetAddressOf())));

	// ハンドルのインクリメントサイズを取得しておく
	m_DescriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(type);
}