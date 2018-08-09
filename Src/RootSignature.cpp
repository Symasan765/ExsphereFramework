#include "RootSignature.h"
#include "Utility.h"

void cRootSignature::AddDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE rangeType, UINT baseShaderRegister, UINT registerSpace, UINT offsetInDescriptorsFromTableStart)
{
	// ディスクリプターテーブルを作成
	const UINT numDescriptors = 1;
	m_Range.push_back({ rangeType, numDescriptors, baseShaderRegister, registerSpace, offsetInDescriptorsFromTableStart });
}

void cRootSignature::AddSRV(UINT baseShaderRegister, UINT registerSpace, UINT offsetInDescriptorsFromTableStart)
{
	AddDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, baseShaderRegister, registerSpace, offsetInDescriptorsFromTableStart);
}

void cRootSignature::AddCBV(UINT baseShaderRegister, UINT registerSpace, UINT offsetInDescriptorsFromTableStart)
{
	AddDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, baseShaderRegister, registerSpace, offsetInDescriptorsFromTableStart);
}

void cRootSignature::AddUAV(UINT baseShaderRegister, UINT registerSpace, UINT offsetInDescriptorsFromTableStart)
{
	AddDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, baseShaderRegister, registerSpace, offsetInDescriptorsFromTableStart);
}

void cRootSignature::AddSamplers(CD3DX12_STATIC_SAMPLER_DESC samp)
{
	m_Samplers.push_back(samp);
}

void cRootSignature::AddSamplers(UINT shaderRegister, D3D12_FILTER filter, D3D12_TEXTURE_ADDRESS_MODE addressU, D3D12_TEXTURE_ADDRESS_MODE addressV, D3D12_TEXTURE_ADDRESS_MODE addressW, FLOAT mipLODBias, UINT maxAnisotropy, D3D12_COMPARISON_FUNC comparisonFunc, D3D12_STATIC_BORDER_COLOR borderColor, FLOAT minLOD, FLOAT maxLOD, D3D12_SHADER_VISIBILITY shaderVisibility, UINT registerSpace)
{
	m_Samplers.push_back(CD3DX12_STATIC_SAMPLER_DESC{ shaderRegister, filter, addressU, addressV, addressW, mipLODBias, maxAnisotropy, comparisonFunc, borderColor, minLOD, maxLOD, shaderVisibility, registerSpace });
}

void cRootSignature::CreateCommit(D3D12_ROOT_SIGNATURE_FLAGS type)
{
	for (int i = 0; i < m_Range.size(); i++) {
		// ルートパラメータを作成し、それを保存しておく
		const UINT num = 1;
		m_RootParams.push_back(CD3DX12_ROOT_PARAMETER());

		m_RootParams[i].InitAsDescriptorTable(num, &m_Range[i]);
	}


	UINT paramNum = m_RootParams.size();
	UINT numSampler = m_Samplers.size();
	
	CD3DX12_ROOT_SIGNATURE_DESC rsdesc;
	rsdesc.Init(paramNum, &m_RootParams[0], numSampler, &m_Samplers[0], type);

	ID3DBlob* pSignature;
	CheckHR(D3D12SerializeRootSignature(&rsdesc, D3D_ROOT_SIGNATURE_VERSION_1, &pSignature, nullptr));

	CheckHR(cDirectX12::GetDevice()->CreateRootSignature(0, pSignature->GetBufferPointer(), pSignature->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature)));
}
