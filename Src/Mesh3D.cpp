#include "Mesh3D.h"
#include "Utility.h"
#include <d3dx12.h>

void cMesh3D::DataSet(const std::vector<DefaultVertex>& vertices, const std::vector<UINT>& indices)
{
	int size = sizeof(indices[0]);
	mIndexCount = static_cast<UINT>(indices.size());
	mVBIndexOffset = static_cast<UINT>(sizeof(DefaultVertex) * vertices.size());
	UINT IBSize = static_cast<UINT>(sizeof(indices[0]) * mIndexCount);

	void* vbData = (void*)&vertices[0];
	void* ibData = (void*)&indices[0];
	CheckHR(cDirectX12::GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(mVBIndexOffset + IBSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(mVB.ReleaseAndGetAddressOf())));

	cDirectX12::GetDevice()->SetName(L"VertexBuffer");
	char* vbUploadPtr = nullptr;
	CheckHR(mVB->Map(0, nullptr, reinterpret_cast<void**>(&vbUploadPtr)));
	memcpy_s(vbUploadPtr, mVBIndexOffset, vbData, mVBIndexOffset);
	memcpy_s(vbUploadPtr + mVBIndexOffset, IBSize, ibData, IBSize);
	mVB->Unmap(0, nullptr);

	mVBView.BufferLocation = mVB->GetGPUVirtualAddress();
	mVBView.StrideInBytes = sizeof(DefaultVertex);
	mVBView.SizeInBytes = mVBIndexOffset;
	mIBView.BufferLocation = mVB->GetGPUVirtualAddress() + mVBIndexOffset;
	mIBView.Format = DXGI_FORMAT_R32_UINT;
	mIBView.SizeInBytes = IBSize;
}

void cMesh3D::MeshDraw(ID3D12GraphicsCommandList * cmdList, UINT InstanceCount)
{
	cmdList->IASetVertexBuffers(0, 1, &mVBView);
	cmdList->IASetIndexBuffer(&mIBView);
	cmdList->DrawIndexedInstanced(mIndexCount, InstanceCount, 0, 0, 0);
}
