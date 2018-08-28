
#include "RootSignatureTest.h"
#include "Utility.h"
#include "DirectX12.h"
#include <d3dx12.h>

using namespace DirectX;
#include "Helper/WaveFrontReader.h"
#include "InputLayout.h"
#include "AssimpLoader.h"

cRootSignatureTest::cRootSignatureTest()
{
	cTextureLoader::LoadTextureFromFile("Laser.png", &m_Tex);
}

cRootSignatureTest::~cRootSignatureTest()
{
}

void cRootSignatureTest::Init()
{
	//DoNotUseHelperInit();
	UseHelperInit();
}


void cRootSignatureTest::MeshAssimopLoad()
{
	
	cAssimpLoader Loader;
	m_pModel = Loader.Load("Private/Link.x");
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
	mVB->Unmap(0, nullptr);

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
	m_RootSignature.AddSRV(0);
	m_RootSignature.CreateCommit();

	{
		m_Shader.CompileFromFile("Private/Mesh.hlsl","VSMain","PSMain");

		cInputLayout inpLay;
		inpLay.AddElement<DirectX::XMFLOAT3>("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
		inpLay.AddElement<DirectX::XMFLOAT3>("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
		inpLay.AddElement<DirectX::XMFLOAT2>("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
		inpLay.AddElement<DirectX::XMFLOAT3>("TANGENT", DXGI_FORMAT_R32G32B32_FLOAT);
		inpLay.AddElement<DirectX::XMINT4>("BONEINDEX", DXGI_FORMAT_R32G32B32A32_SINT);
		inpLay.AddElement<DirectX::XMFLOAT4>("WEIGHT", DXGI_FORMAT_R32G32B32A32_FLOAT);

		mc_PSO.InputLayoutSetting(&inpLay);
		DXGI_FORMAT format[] = { DXGI_FORMAT_R8G8B8A8_UNORM };
		mc_PSO.RenderTargetSetting(format, 1);
		mc_PSO.RootSignatureSetting(m_RootSignature.GetRootSignature().Get());
		mc_PSO.ShaderBytecodeSetting(&m_Shader);
		mc_PSO.CreatePipelineState();
	}

	MeshLoad();
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
		float rate = 1.0f;
		XMMATRIX worldMat, viewMat, projMat;
		worldMat = XMMatrixIdentity();
		worldMat *= XMMatrixScaling(rate, rate, rate);
		worldMat *= XMMatrixRotationY(XMConvertToRadians(rot));
		worldMat *= XMMatrixTranslation(0.7f, 0.0f, 0.0f);
		viewMat = XMMatrixLookAtLH({ 0, 0.5f, -1.5f }, { 0, 0.5f, 0 }, { 0, 1, 0 });
		projMat = XMMatrixPerspectiveFovLH(45, (float)1920 / 1080, 0.01f, 50.0f);
		auto mvpMat = XMMatrixTranspose(worldMat * viewMat * projMat);

		auto worldTransMat = XMMatrixTranspose(worldMat);

		XMStoreFloat4x4(&m_ConstHelBuf.data.worldViewProjMatrix, mvpMat);
		XMStoreFloat4x4(&m_ConstHelBuf.data.worldMatrix, worldTransMat);
		m_ConstHelBuf.Upload();
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

		cmdList->SetPipelineState(mc_PSO.GetPipelineState().Get());

		auto cbvSrvUavDescHeap = m_ConstHelBuf.GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
		ID3D12DescriptorHeap* descHeaps[] = { m_ConstHelBuf.GetDescriptorHeap() };
		cmdList->SetDescriptorHeaps(ARRAYSIZE(descHeaps), descHeaps);
		cmdList->SetGraphicsRootDescriptorTable(0, cbvSrvUavDescHeap);

		auto cbvbonecHeap = m_Bones.GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
		ID3D12DescriptorHeap* boneHeaps[] = { m_Bones.GetDescriptorHeap() };
		cmdList->SetDescriptorHeaps(ARRAYSIZE(boneHeaps), boneHeaps);
		cmdList->SetGraphicsRootDescriptorTable(1, cbvbonecHeap);

		m_pModel->Draw(cmdList, true, 2, 1);

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
		data[i] = XMMatrixTranspose(data[i]);
		XMStoreFloat4x4(&m_Bones.data.bones[i], data[i]);
	}

	m_Bones.Upload();
}
