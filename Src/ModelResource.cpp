#include "ModelResource.h"

void cModelResource::Draw(ID3D12GraphicsCommandList * cmdList, bool texDrawFlag, UINT RootParameterIndex, UINT InstanceCount)
{
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (auto& data : m_Graphic) {
		// テクスチャのセット
		if (texDrawFlag) {
			data.m_Material.SetTexture(cmdList, RootParameterIndex);
		}
		data.m_Mesh.MeshDraw(cmdList, InstanceCount);
	}
}
