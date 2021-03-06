
#include "RootSignatureTest.h"
#include "Utility.h"
#include "DirectX12.h"
#include <d3dx12.h>

using namespace DirectX;
#include "Helper/WaveFrontReader.h"
#include "InputLayout.h"
#include "AssimpLoader.h"
#include "Game/StageStruct.h"
#include "Wave.h"

cRootSignatureTest::cRootSignatureTest()
{
}

cRootSignatureTest::~cRootSignatureTest()
{
}

void cRootSignatureTest::Init()
{
	//DoNotUseHelperInit();
	UseHelperInit();

	{
		m_Dir.data.color = { 1.0f,1.0f,1.0f,1.0f };
		auto dir = XMVector4Normalize({ 0.0f,-1.0f,-1.0f,1.0f });
		XMStoreFloat4(&m_Dir.data.direction, dir);
	}
	{
		DirectX::XMFLOAT4 color[] = {
			{1.0f,0.0f,0.0f,1.0f},{0.0f,1.0f,0.0f,1.0f},{0.2f,0.2f,1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}
		};

		for (int i = 0; i < 48; i++) {
			m_Lights.data.PointLightColor[i] = { (float)(rand() % 255) / 255.0f,(float)(rand() % 255) / 255.0f ,(float)(rand() % 255) / 255.0f,10.0f };
			m_Lights.data.PointLightColor[i] = color[i % 4];
			m_Lights.data.PointLightColor[i].w = 2.0f;
			m_Lights.data.PointLightDistance[i] = 1.0f;
			m_Lights.data.PointLightDecay[i] = 0.4f;

			const float troutSize = 1.8f;		// マス自体のワールド上のサイズ
			const float startX = -4.5f;
			const float startY = 12.6f;

			int x = (i % 6);
			int y = (i / 6);
			m_Lights.data.PointLightPosition[i] = { startX + (float)x * troutSize ,startY - (float)y * troutSize ,-1.0f ,1.0f};
		}
	}
	m_Lights.Upload();

	m_PbrParam.data.metallic = 0.4;
	m_PbrParam.data.roughness = 1.0f;
	m_PbrParam.Upload();
}


void cRootSignatureTest::MeshAssimopLoad()
{
	
	cAssimpLoader Loader;
	m_pModel = Loader.Load("Pac1.fbx");
}

void cRootSignatureTest::MeshLoad()
{
	WaveFrontReader<uint16_t> mesh;
	CheckHR(mesh.Load(L"Private/teapot.obj"));

	int size = sizeof(mesh.indices[0]);
	mIndexCount = static_cast<UINT>(mesh.indices.size());
	mVBIndexOffset = static_cast<UINT>(sizeof(mesh.vertices[0]) * mesh.vertices.size());
	UINT IBSize = static_cast<UINT>(sizeof(mesh.indices[0]) * mIndexCount);

	void* vbData = mesh.vertices.data();
	void* ibData = mesh.indices.data();
	CheckHR(cDirectX12::GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(mVBIndexOffset + IBSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(mVB.ReleaseAndGetAddressOf())));
	cDirectX12::GetDevice()->SetName(L"VertexBuffer");
	char* vbUploadPtr = nullptr;
	CheckHR(mVB->Map(0, nullptr, reinterpret_cast<void**>(&vbUploadPtr)));
	memcpy_s(vbUploadPtr, mVBIndexOffset, vbData, mVBIndexOffset);
	memcpy_s(vbUploadPtr + mVBIndexOffset, IBSize, ibData, IBSize);
	//mVB->Unmap(0, nullptr);

	mVBView.BufferLocation = mVB->GetGPUVirtualAddress();
	mVBView.StrideInBytes = sizeof(mesh.vertices[0]);
	mVBView.SizeInBytes = mVBIndexOffset;
	mIBView.BufferLocation = mVB->GetGPUVirtualAddress() + mVBIndexOffset;
	mIBView.Format = DXGI_FORMAT_R16_UINT;
	mIBView.SizeInBytes = IBSize;
}

void cRootSignatureTest::Draw(ID3D12GraphicsCommandList* cmdList)
{
	UseHelperDraw(cmdList);
	//DoNotUseHelperDraw(cmdList);
}

void cRootSignatureTest::UseHelperInit()
{
	m_RootSignature.AddSamplers(0);
	m_RootSignature.AddCBV(0);
	m_RootSignature.AddCBV(1);
	m_RootSignature.AddCBV(2);
	m_RootSignature.AddCBV(3);
	m_RootSignature.AddSRV(0);
	m_RootSignature.CreateCommit();

	{
		m_Shader.CompileFromFile("HLSL/PBR.hlsl","VSMain","PSMain");

		cInputLayout inpLay;
		inpLay.AddElement<DirectX::XMFLOAT3>("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
		inpLay.AddElement<DirectX::XMFLOAT3>("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
		inpLay.AddElement<DirectX::XMFLOAT2>("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
		inpLay.AddElement<DirectX::XMFLOAT3>("TANGENT", DXGI_FORMAT_R32G32B32_FLOAT);
		inpLay.AddElement<DirectX::XMINT4>("BONEINDEX", DXGI_FORMAT_R32G32B32A32_SINT);
		inpLay.AddElement<DirectX::XMFLOAT4>("WEIGHT", DXGI_FORMAT_R32G32B32A32_FLOAT);
		inpLay.AddElement<DirectX::XMFLOAT4>("MATRIX", DXGI_FORMAT_R32G32B32A32_FLOAT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 0, 1, 1);
		inpLay.AddElement<DirectX::XMFLOAT4>("MATRIX", DXGI_FORMAT_R32G32B32A32_FLOAT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1, 1, 1);
		inpLay.AddElement<DirectX::XMFLOAT4>("MATRIX", DXGI_FORMAT_R32G32B32A32_FLOAT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 2, 1, 1);
		inpLay.AddElement<DirectX::XMFLOAT4>("MATRIX", DXGI_FORMAT_R32G32B32A32_FLOAT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 3, 1, 1);

		mc_PSO.InputLayoutSetting(&inpLay);
		DXGI_FORMAT format[] = { DXGI_FORMAT_R8G8B8A8_UNORM };
		mc_PSO.RenderTargetSetting(format, 1);
		mc_PSO.RootSignatureSetting(m_RootSignature.GetRootSignature().Get());
		mc_PSO.ShaderBytecodeSetting(&m_Shader);
		mc_PSO.CreatePipelineState();
	}

	MeshAssimopLoad();
}


void cRootSignatureTest::DoNotUseHelperDraw(ID3D12GraphicsCommandList* cmdList)
{
	// 定数バッファの更新
	// Upload constant buffer
	{
		static float rot = 0.0f;
		rot += 1.0f;
		if (rot >= 360.0f) rot = 0.0f;

		XMMATRIX worldMat, viewMat, projMat;
		worldMat = XMMatrixIdentity();
		worldMat *= XMMatrixScaling(0.5f, 0.5f, 0.5f);
		worldMat *= XMMatrixRotationY(XMConvertToRadians(rot));
		worldMat *= XMMatrixTranslation(-0.3f, 0.0f, 0.0f);
		viewMat = XMMatrixLookAtLH({ 0, 0.5f, -1.5f }, { 0, 0.5f, 0 }, { 0, 1, 0 });
		projMat = XMMatrixPerspectiveFovLH(45, (float)1920 / 1080, 0.01f, 50.0f);
		auto mvpMat = XMMatrixTranspose(worldMat * viewMat * projMat);

		auto worldTransMat = XMMatrixTranspose(worldMat);

		XMStoreFloat4x4(&m_ConstBuf.data.worldViewProjMatrix, mvpMat);
		XMStoreFloat4x4(&m_ConstBuf.data.worldMatrix, worldTransMat);
		m_ConstBuf.Upload();
	}

	// Viewport & Scissor
	D3D12_VIEWPORT viewport = {};
	viewport.Width = (float)1920;
	viewport.Height = (float)1080;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	cmdList->RSSetViewports(1, &viewport);
	D3D12_RECT scissor = {};
	scissor.right = (LONG)1920;
	scissor.bottom = (LONG)1080;
	cmdList->RSSetScissorRects(1, &scissor);

	// Draw
	cmdList->SetGraphicsRootSignature(mRootSignature.Get());

	ID3D12DescriptorHeap* descHeaps[] = { m_ConstBuf.GetDescriptorHeap() };
	cmdList->SetDescriptorHeaps(ARRAYSIZE(descHeaps), descHeaps);
	ID3D12DescriptorHeap* srvHeaps[] = { m_Tex.GetDescriptorHeap().Get() };
	cmdList->SetDescriptorHeaps(ARRAYSIZE(srvHeaps), srvHeaps);
	cmdList->SetGraphicsRootDescriptorTable(1, m_Tex.GetDescriptorHeap().Get()->GetGPUDescriptorHandleForHeapStart());

	{
		auto cbvSrvUavDescHeap = m_ConstBuf.GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
		cmdList->SetDescriptorHeaps(ARRAYSIZE(descHeaps), descHeaps);
		cmdList->SetGraphicsRootDescriptorTable(0, cbvSrvUavDescHeap);
		cmdList->SetDescriptorHeaps(ARRAYSIZE(srvHeaps), srvHeaps);
		cmdList->SetGraphicsRootDescriptorTable(1, m_Tex.GetDescriptorHeap().Get()->GetGPUDescriptorHandleForHeapStart());
		cmdList->SetPipelineState(mc_PSO.GetPipelineState().Get());
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		cmdList->IASetVertexBuffers(0, 1, &mVBView);
		cmdList->IASetIndexBuffer(&mIBView);
		cmdList->DrawIndexedInstanced(mIndexCount, 1, 0, 0, 0);
	}
}

void cRootSignatureTest::UseHelperDraw(ID3D12GraphicsCommandList * cmdList)
{
	// 定数バッファの更新
	// Upload constant buffer
	{
		static float rot = 0.0f;
		rot += 1.0f;
		if (rot >= 360.0f) rot = 0.0f;
		float rate = 0.1;
		XMMATRIX worldMat, viewMat, projMat;
		worldMat = XMMatrixIdentity();
		worldMat *= XMMatrixScaling(rate, rate, rate);
		worldMat *= XMMatrixRotationY(XMConvertToRadians(rot));
		worldMat *= XMMatrixTranslation(0.7f, 0.0f, 0.0f);
		viewMat = XMMatrixLookAtLH({ 0, -1.5f, -8.5f }, { 0, 4.5f, 0 }, { 0, 1, 0 });
		projMat = XMMatrixPerspectiveFovLH(45, (float)1920 / 1080, 0.01f, 50.0f);
		//auto mvpMat = XMMatrixTranspose(worldMat * viewMat * projMat);
		auto mvpMat = XMMatrixTranspose(viewMat * projMat);	// インスタンシング描画のテストでワールドを除外

		auto worldTransMat = XMMatrixTranspose(viewMat);

		XMStoreFloat4x4(&m_ConstHelBuf.data.worldViewProjMatrix, mvpMat);
		XMStoreFloat4x4(&m_ConstHelBuf.data.worldMatrix, worldTransMat);
		m_ConstHelBuf.Upload();
	}
	{
		StageData stage;
		for (int i = 0; i < 48; i++) {
			if (!stage.LightJudge(i % 6, i / 6)) {
				m_Lights.data.PointLightColor[i] = { 0.0f,0.0f,0.0f,0.0f };
			}
			else {
				DirectX::XMFLOAT4 color[] = {
			{1.0f,0.0f,0.0f,2.0f},{0.0f,1.0f,0.0f,2.0f},{0.2f,0.2f,1.0f,2.0f},{1.0f,1.0f,1.0f,2.0f}
				};
				m_Lights.data.PointLightColor[i] = color[i % 4];
			}
		}
	}


	// Viewport & Scissor
	D3D12_VIEWPORT viewport = {};
	viewport.Width = (float)1920;
	viewport.Height = (float)1080;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	cmdList->RSSetViewports(1, &viewport);
	D3D12_RECT scissor = {};
	scissor.right = (LONG)1920;
	scissor.bottom = (LONG)1080;
	cmdList->RSSetScissorRects(1, &scissor);

	// Draw
	cmdList->SetGraphicsRootSignature(m_RootSignature.GetRootSignature().Get());

	BoneCalc();
	{
		m_Lights.Upload();
		m_PbrParam.Upload();
		m_Dir.Upload();

		cmdList->SetPipelineState(mc_PSO.GetPipelineState().Get());

		{
			auto cbvSrvUavDescHeap = m_ConstHelBuf.GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
			ID3D12DescriptorHeap* descHeaps[] = { m_ConstHelBuf.GetDescriptorHeap() };
			cmdList->SetDescriptorHeaps(ARRAYSIZE(descHeaps), descHeaps);
			cmdList->SetGraphicsRootDescriptorTable(0, cbvSrvUavDescHeap);
		}
		{
			auto cbvLightHeap = m_Dir.GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
			ID3D12DescriptorHeap* lightHeaps[] = { m_Dir.GetDescriptorHeap() };
			cmdList->SetDescriptorHeaps(ARRAYSIZE(lightHeaps), lightHeaps);
			cmdList->SetGraphicsRootDescriptorTable(1, cbvLightHeap);
		}
		{
			auto cbvLightHeap = m_Lights.GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
			ID3D12DescriptorHeap* lightHeaps[] = { m_Lights.GetDescriptorHeap() };
			cmdList->SetDescriptorHeaps(ARRAYSIZE(lightHeaps), lightHeaps);
			cmdList->SetGraphicsRootDescriptorTable(2, cbvLightHeap);
		}
		{
			auto cbvPbrHeap = m_PbrParam.GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
			ID3D12DescriptorHeap* pbrHeaps[] = { m_PbrParam.GetDescriptorHeap() };
			cmdList->SetDescriptorHeaps(ARRAYSIZE(pbrHeaps), pbrHeaps);
			cmdList->SetGraphicsRootDescriptorTable(3, cbvPbrHeap);
		}
		
		//m_pModel->Draw(cmdList, true, 2, 1);

	}
}

void cRootSignatureTest::BoneCalc()
{
	auto& bones = m_pModel->m_Bone;
	XMMATRIX data[100];
	for (int i = 0; i < 100; i++) {
		data[i] = XMMatrixIdentity();
	}

	for (int i = 0; i < bones.size(); i++) {
		data[i] = XMLoadFloat4x4(&bones[i].Transpose);
		//data[i] = XMMatrixIdentity();
	}

	for (int i = 0; i < bones.size(); i++) {
		// 親が存在する
		if (bones[i].parent != -1) {
			int parent = bones[i].parent;
			data[i] *= data[parent];
		}
	}

	for (int i = 0; i < bones.size(); i++) {
		data[i] = XMLoadFloat4x4(&bones[i].Offset) * data[i];
		data[i] *= XMMatrixScaling(10.0f, 10.0f, 10.0f);
		data[i] = XMMatrixTranspose(data[i]);
		XMStoreFloat4x4(&m_Bones.data.bones[i], data[i]);
	}

	m_Bones.Upload();
}
