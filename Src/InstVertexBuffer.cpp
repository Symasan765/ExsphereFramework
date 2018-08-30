#include "InstVertexBuffer.h"
#include "Utility.h"
#include "FrameCnt.h"
#include <d3dx12.h>

void cInstVertexBuffer::Create()
{
	for (int i = 0; i < DrawParam::g_MaxFrameLatency; i++) {
		CheckHR(cDirectX12::GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(DirectX::XMFLOAT4X4) * DrawParam::g_MaxInstNum),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(mVB[i].ReleaseAndGetAddressOf())));
		cDirectX12::GetDevice()->SetName(L"InstVertexBuffer");
		CheckHR(mVB[i]->Map(0, nullptr, reinterpret_cast<void**>(&vbUploadPtr[i])));
	}
}

void cInstVertexBuffer::SetInstVertexData(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList, DirectX::XMFLOAT4X4 * data, int num)
{
	UINT size = sizeof(DirectX::XMFLOAT4X4) * num;
	memcpy_s(vbUploadPtr[cFrameCnt::GetNowIndex()], size, data, size);

	mVBView[cFrameCnt::GetNowIndex()].BufferLocation = mVB[cFrameCnt::GetNowIndex()]->GetGPUVirtualAddress();
	mVBView[cFrameCnt::GetNowIndex()].StrideInBytes = sizeof(DirectX::XMFLOAT4X4);
	mVBView[cFrameCnt::GetNowIndex()].SizeInBytes = size;

	cmdList->IASetVertexBuffers(1, 1, &mVBView[cFrameCnt::GetNowIndex()]);
}
