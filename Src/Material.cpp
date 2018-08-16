#include "Material.h"

void cMaterial::DataSet(const std::vector<cTexture>& textures)
{
	m_Tex = textures;
}

void cMaterial::SetTexture(ID3D12GraphicsCommandList * cmdList, UINT RootParameterIndex)
{
	if (m_Tex.size() != 0) {
		ID3D12DescriptorHeap* srvHeaps[] = { m_Tex[0].GetDescriptorHeap().Get() };
		cmdList->SetDescriptorHeaps(ARRAYSIZE(srvHeaps), srvHeaps);
		cmdList->SetGraphicsRootDescriptorTable(RootParameterIndex, m_Tex[0].GetDescriptorHeap().Get()->GetGPUDescriptorHandleForHeapStart());
	}
}
