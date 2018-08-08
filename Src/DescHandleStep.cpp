#include "DescHandleStep.h"

UINT cDescHandleStep::m_Size[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];

UINT cDescHandleStep::GetSize(D3D12_DESCRIPTOR_HEAP_TYPE type)
{
	return m_Size[(int)type];
}

void cDescHandleStep::Init(ID3D12Device* dev)
{
	for (int i = 0; i < D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES; i++) {
		m_Size[i] = dev->GetDescriptorHandleIncrementSize((D3D12_DESCRIPTOR_HEAP_TYPE)i);
	}
}
