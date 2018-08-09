/*=================================================
//								FILE :	RootSignature.h
//		ファイル説明 :
//		ルートシグネチャの設定を簡易に行えるようにするクラス
//
//									松本 雄之介
=================================================*/
#pragma once
#include <vector>
#include "DirectX12.h"
#include <d3dx12.h>

class cRootSignature
{
public:
	cRootSignature() = default;
	~cRootSignature() = default;


	void AddSRV(
		UINT baseShaderRegister,
		UINT registerSpace = 0,
		UINT offsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND);

	void AddCBV(UINT baseShaderRegister, UINT registerSpace = 0,
		UINT offsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND);

	void AddUAV(UINT baseShaderRegister, UINT registerSpace = 0,
		UINT offsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND);

	void AddSamplers(CD3DX12_STATIC_SAMPLER_DESC samp);

	void AddSamplers(UINT shaderRegister, D3D12_FILTER filter = D3D12_FILTER_ANISOTROPIC, D3D12_TEXTURE_ADDRESS_MODE addressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE addressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE addressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		FLOAT mipLODBias = 0, UINT maxAnisotropy = 16, D3D12_COMPARISON_FUNC comparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL,
		D3D12_STATIC_BORDER_COLOR borderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,
		FLOAT minLOD = 0.f, FLOAT maxLOD = D3D12_FLOAT32_MAX, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL, UINT registerSpace = 0);



	// 必要な登録が出来たらこの関数を実行してルートシグネチャ本体の作成を行う
	void CreateCommit(D3D12_ROOT_SIGNATURE_FLAGS type = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	inline Microsoft::WRL::ComPtr<ID3D12RootSignature>& GetRootSignature() { return m_RootSignature; };
private:
	void AddDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE rangeType,
		UINT baseShaderRegister,
		UINT registerSpace,
		UINT offsetInDescriptorsFromTableStart);


	// 作成までの間、ここに情報を溜めていく
	std::vector<CD3DX12_ROOT_PARAMETER> m_RootParams;
	std::vector<CD3DX12_STATIC_SAMPLER_DESC> m_Samplers;
	std::vector<CD3DX12_DESCRIPTOR_RANGE> m_Range;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_RootSignature;
};