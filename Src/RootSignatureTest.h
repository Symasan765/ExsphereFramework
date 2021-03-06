#pragma once
#include "MeshTest.h"

#include <wrl/client.h>
#include <d3d12.h>
#include <Windows.h>
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include "TextureLoader.h"

#include "PipelineStateObj.h"
#include "ShaderByte.h"

#include "RootSignature.h"
#include "ModelResource.h"

#include "LightParameter.h"
#include "ParameterPBR.h"

struct CBuffer {
	DirectX::XMFLOAT4X4 worldViewProjMatrix;
	DirectX::XMFLOAT4X4 worldMatrix;
};

struct cBones {
	DirectX::XMFLOAT4X4 bones[128];
};

struct PBRLight
{
	DirectX::XMFLOAT4 PointLightPosition[48];
	DirectX::XMFLOAT4 PointLightColor[48];
	float PointLightDistance[48];
	float PointLightDecay[48];
};

/// <summary>
/// あくまで暫定的なテスト用クラス
/// </summary>
class cRootSignatureTest {
public:
	cRootSignatureTest();
	~cRootSignatureTest();
	void Init();
	void MeshAssimopLoad();
	void MeshLoad();
	void Draw(ID3D12GraphicsCommandList*);

	void UseHelperInit();

	void DoNotUseHelperDraw(ID3D12GraphicsCommandList*);
	void UseHelperDraw(ID3D12GraphicsCommandList*);

	void BoneCalc();
	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature;
	Microsoft::WRL::ComPtr<ID3D12Resource> mVB;

	UINT mIndexCount = 0;
	UINT mVBIndexOffset = 0;
	D3D12_VERTEX_BUFFER_VIEW mVBView = {};
	D3D12_INDEX_BUFFER_VIEW mIBView = {};

	cConstBuf<CBuffer> m_ConstBuf;
	cConstBuf<CBuffer> m_ConstHelBuf;

	cConstBuf<cBones> m_Bones;

	cConstBuf<ParameterPBR> m_PbrParam;
	cConstBuf<PBRLight> m_Lights;
	cConstBuf<DirectionalLight> m_Dir;

	cTexture m_Tex;
	cPipelineStateObj mc_PSO;
	cShaderByte m_Shader;
	cRootSignature m_RootSignature;
	std::unique_ptr<cModelResource> m_pModel;
};