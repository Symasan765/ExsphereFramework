#pragma once
#include "DXMath.h"
#include "DirectX12.h"
#include "DrawParam.h"

class cInstVertexBuffer
{
public:
	cInstVertexBuffer() = default;
	~cInstVertexBuffer() = default;

	void Create();
	void SetInstVertexData(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList,DirectX::XMFLOAT4X4* data,int num);
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> mVB[DrawParam::g_MaxFrameLatency];
	D3D12_VERTEX_BUFFER_VIEW mVBView[DrawParam::g_MaxFrameLatency] = {};
	char* vbUploadPtr[DrawParam::g_MaxFrameLatency] = { nullptr };
};