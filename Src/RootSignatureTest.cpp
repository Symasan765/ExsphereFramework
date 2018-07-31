
#include "RootSignatureTest.h"
#include "Utility.h"
#include "DirectX12.h"
#include <d3dx12.h>

using namespace DirectX;
#include "Helper/WaveFrontReader.h"
#include "InputLayout.h"

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
	
	meshdata.Load("Private/Link.x");

	//void* vbData = mesh.vertices.data();
	//void* ibData = mesh.indices.data();
	//CheckHR(cDirectX12::GetDevice()->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
	//	D3D12_HEAP_FLAG_NONE,
	//	&CD3DX12_RESOURCE_DESC::Buffer(mVBIndexOffset + IBSize),
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(mVB.ReleaseAndGetAddressOf())));
	//cDirectX12::GetDevice()->SetName(L"VertexBuffer");
	//char* vbUploadPtr = nullptr;
	//CheckHR(mVB->Map(0, nullptr, reinterpret_cast<void**>(&vbUploadPtr)));
	//memcpy_s(vbUploadPtr, mVBIndexOffset, vbData, mVBIndexOffset);
	//memcpy_s(vbUploadPtr + mVBIndexOffset, IBSize, ibData, IBSize);
	//mVB->Unmap(0, nullptr);

	//mVBView.BufferLocation = mVB->GetGPUVirtualAddress();
	//mVBView.StrideInBytes = sizeof(mesh.vertices[0]);
	//mVBView.SizeInBytes = mVBIndexOffset;
	//mIBView.BufferLocation = mVB->GetGPUVirtualAddress() + mVBIndexOffset;
	//mIBView.Format = DXGI_FORMAT_R16_UINT;
	//mIBView.SizeInBytes = IBSize;
	int a = 10;
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
	DoNotUseHelperDraw(cmdList);
}

void cRootSignatureTest::UseHelperInit()
{
	// サンプラーの設定
	CD3DX12_STATIC_SAMPLER_DESC sampler;
	sampler.Init(0);		// シェーダレジスタ番号

	// ディスクリプターテーブル、ルートパラメータの定義
	CD3DX12_DESCRIPTOR_RANGE ranges[2];
	CD3DX12_ROOT_PARAMETER rootParameters[2];

	ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	rootParameters[0].InitAsDescriptorTable(1, &ranges[0]);
	rootParameters[1].InitAsDescriptorTable(1, &ranges[1]);

	CD3DX12_ROOT_SIGNATURE_DESC rsdesc;
	rsdesc.Init(_countof(rootParameters), rootParameters, 1U, &sampler, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob* pSignature;
	CheckHR(D3D12SerializeRootSignature(&rsdesc, D3D_ROOT_SIGNATURE_VERSION_1, &pSignature, nullptr));

	CheckHR(cDirectX12::GetDevice()->CreateRootSignature(0, pSignature->GetBufferPointer(), pSignature->GetBufferSize(), IID_PPV_ARGS(&mRootSignature)));

	{
		m_Shader.CompileFromFile("Private/Mesh.hlsl","VSMain","PSMain");

		cInputLayout inpLay;
		inpLay.AddElement<DirectX::XMFLOAT3>("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
		inpLay.AddElement<DirectX::XMFLOAT3>("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
		inpLay.AddElement<DirectX::XMFLOAT2>("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

		mc_PSO.InputLayoutSetting(&inpLay);
		DXGI_FORMAT format[] = { DXGI_FORMAT_R8G8B8A8_UNORM };
		mc_PSO.RenderTargetSetting(format, 1);
		mc_PSO.RootSignatureSetting(mRootSignature.Get());
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

		XMMATRIX worldMat, viewMat, projMat;
		worldMat = XMMatrixIdentity();
		worldMat *= XMMatrixScaling(0.05f, 0.05f, 0.05f);
		worldMat *= XMMatrixRotationY(XMConvertToRadians(rot));
		worldMat *= XMMatrixTranslation(0.3f, 0.0f, 0.0f);
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
	cmdList->SetGraphicsRootSignature(mRootSignature.Get());


	{
		for (int i = 0; i < meshdata.meshes.size(); i++) {


			auto cbvSrvUavDescHeap = m_ConstHelBuf.GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
			ID3D12DescriptorHeap* descHeaps[] = { m_ConstHelBuf.GetDescriptorHeap() };
			cmdList->SetDescriptorHeaps(ARRAYSIZE(descHeaps), descHeaps);
			cmdList->SetGraphicsRootDescriptorTable(0, cbvSrvUavDescHeap);

			if (meshdata.meshes[i].textures.size() != 0) {
				ID3D12DescriptorHeap* srvHeaps[] = { meshdata.meshes[i].textures[0].GetDescriptorHeap().Get() };
				cmdList->SetDescriptorHeaps(ARRAYSIZE(srvHeaps), srvHeaps);
				cmdList->SetGraphicsRootDescriptorTable(1, meshdata.meshes[i].textures[0].GetDescriptorHeap().Get()->GetGPUDescriptorHandleForHeapStart());
			}


			cmdList->SetPipelineState(mc_PSO.GetPipelineState().Get());
			cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			cmdList->IASetVertexBuffers(0, 1, &meshdata.meshes[i].mVBView);
			cmdList->IASetIndexBuffer(&meshdata.meshes[i].mIBView);
			cmdList->DrawIndexedInstanced(meshdata.meshes[i].mIndexCount, 1, 0, 0, 0);
		}
	}
}
