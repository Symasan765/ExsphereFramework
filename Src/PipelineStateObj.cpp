#include "PipelineStateObj.h"
#include <d3dx12.h>
#include "Utility.h"
#include "DirectX12.h"

cPipelineStateObj::cPipelineStateObj()
{
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(CD3DX12_DEFAULT());
	psoDesc.BlendState = CD3DX12_BLEND_DESC(CD3DX12_DEFAULT());
	psoDesc.DepthStencilState.DepthEnable = true;
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.StencilEnable = false;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;
}

void cPipelineStateObj::CreatePipelineState()
{
	CheckHR(cDirectX12::GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(m_Pso.ReleaseAndGetAddressOf())));
}

void cPipelineStateObj::RenderTargetSetting(DXGI_FORMAT * format, unsigned RTNum)
{
	psoDesc.NumRenderTargets = RTNum;

	for (int i = 0; i < RTNum; i++)
		psoDesc.RTVFormats[i] = format[i];
}

void cPipelineStateObj::ShaderBytecodeSetting(ID3D10Blob * vs, ID3D10Blob * ps, ID3D10Blob * ds, ID3D10Blob * hs, ID3D10Blob * gs)
{
	if (vs != nullptr) {
		psoDesc.VS.pShaderBytecode = vs->GetBufferPointer();
		psoDesc.VS.BytecodeLength = vs->GetBufferSize();
	}

	if (ps != nullptr) {
		psoDesc.PS.pShaderBytecode = ps->GetBufferPointer();
		psoDesc.PS.BytecodeLength = ps->GetBufferSize();
	}

	if (ds != nullptr) {
		psoDesc.DS.pShaderBytecode = ds->GetBufferPointer();
		psoDesc.DS.BytecodeLength = ds->GetBufferSize();
	}

	if (hs != nullptr) {
		psoDesc.HS.pShaderBytecode = hs->GetBufferPointer();
		psoDesc.HS.BytecodeLength = hs->GetBufferSize();
	}

	if (gs != nullptr) {
		psoDesc.GS.pShaderBytecode = gs->GetBufferPointer();
		psoDesc.GS.BytecodeLength = gs->GetBufferSize();
	}
}

void cPipelineStateObj::RootSignatureSetting(ID3D12RootSignature * rs)
{
	psoDesc.pRootSignature = rs;
}

void cPipelineStateObj::InputLayoutSetting(D3D12_INPUT_ELEMENT_DESC * inputLayout, unsigned num)
{
	psoDesc.InputLayout.NumElements = num;
	psoDesc.InputLayout.pInputElementDescs = inputLayout;
}

