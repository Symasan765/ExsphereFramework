/*=================================================
//								FILE : PipelineStateObj.h
//		�t�@�C������ :
//		�p�C�v���C���X�e�[�g�I�u�W�F�N�g�̐ݒ�A�쐬���s���N���X���`����B
//		
//									���{ �Y�V��
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

	//=====�ȉ��̊֐��͐ݒ�K�{======
	void RenderTargetSetting(DXGI_FORMAT* format, unsigned RTNum);
	void ShaderBytecodeSetting(ID3D10Blob* vs, ID3D10Blob* ps, ID3D10Blob* ds = nullptr, ID3D10Blob* hs = nullptr, ID3D10Blob* gs = nullptr);
	void ShaderBytecodeSetting(cShaderByte* pShaderData);
	void RootSignatureSetting(ID3D12RootSignature* rs);
	void InputLayoutSetting(D3D12_INPUT_ELEMENT_DESC* inputLayout,unsigned num);
	void InputLayoutSetting(cInputLayout* inputLayout);
	// ==========================

	// ���̐ݒ荀�ڂւ̑Ή��͎b��I�ɃI�u�W�F�N�g���̂�n���čs��
	inline D3D12_GRAPHICS_PIPELINE_STATE_DESC* GetPipelineDESC() { return &psoDesc; };
	inline Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState() { return m_Pso; };
private:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_Pso;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
};