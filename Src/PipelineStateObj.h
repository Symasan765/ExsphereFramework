/*=================================================
//								FILE : PipelineStateObj.h
//		ファイル説明 :
//		パイプラインステートオブジェクトの設定、作成を行うクラスを定義する。
//		
//									松本 雄之介
=================================================*/
#pragma once
#include <wrl/client.h>
#include <d3d12.h>
class cInputLayout;
class cShaderByte;

class cPipelineStateObj
{
public:
	cPipelineStateObj();
	~cPipelineStateObj() = default;
	void CreatePipelineState();

	//=====以下の関数は設定必須======
	void RenderTargetSetting(DXGI_FORMAT* format, unsigned RTNum);
	void ShaderBytecodeSetting(ID3D10Blob* vs, ID3D10Blob* ps, ID3D10Blob* ds = nullptr, ID3D10Blob* hs = nullptr, ID3D10Blob* gs = nullptr);
	void ShaderBytecodeSetting(cShaderByte* pShaderData);
	void RootSignatureSetting(ID3D12RootSignature* rs);
	void InputLayoutSetting(D3D12_INPUT_ELEMENT_DESC* inputLayout,unsigned num);
	void InputLayoutSetting(cInputLayout* inputLayout);
	// ==========================

	// 他の設定項目への対応は暫定的にオブジェクト自体を渡して行う
	inline D3D12_GRAPHICS_PIPELINE_STATE_DESC* GetPipelineDESC() { return &psoDesc; };
	inline Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState() { return m_Pso; };
private:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_Pso;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
};