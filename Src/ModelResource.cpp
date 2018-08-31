#include "ModelResource.h"

void cModelResource::Draw(DirectX::XMFLOAT4X4* matPtr, ID3D12GraphicsCommandList * cmdList, bool texDrawFlag, UINT RootParameterIndex, UINT InstanceCount)
{
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (auto& data : m_Graphic) {
		// テクスチャのセット
		if (texDrawFlag) {
			data.m_Material.SetTexture(cmdList, RootParameterIndex);
		}

		m_InstBuffer.SetInstVertexData(cmdList, matPtr, InstanceCount);
		data.m_Mesh.MeshDraw(cmdList, InstanceCount);
	}
}
