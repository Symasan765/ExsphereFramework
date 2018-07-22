#include "RootSignatureTest.h"
#include "Utility.h"
#include "DirectX12.h"
#include <d3dx12.h>
using namespace DirectX;
#include "Helper/WaveFrontReader.h"

cRootSignatureTest::cRootSignatureTest()
{
	cTextureLoader::LoadTextureFromFile("Laser.png", &m_Tex);
}

cRootSignatureTest::~cRootSignatureTest()
{
}

void cRootSignatureTest::Init()
{
	D3D12_STATIC_SAMPLER_DESC sampler = {};
	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.MipLODBias = 0;
	sampler.MaxAnisotropy = 0;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.MinLOD = 0.0f;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.ShaderRegister = 0;
	sampler.RegisterSpace = 0;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_DESCRIPTOR_RANGE ranges[2];
	D3D12_ROOT_PARAMETER rootParameters[2];

	ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;         // このDescriptorRangeは定数バッファ
	ranges[0].NumDescriptors = 1;               // Descriptorは1つ
	ranges[0].BaseShaderRegister = 0;              // シェーダ側の開始インデックスは0番
	ranges[0].RegisterSpace = 0;               // TODO: SM5.1からのspaceだけど、どういうものかよくわからない
	ranges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;  // TODO: とりあえず-1を入れておけばOK？

	ranges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;         // このDescriptorRangeは定数バッファ
	ranges[1].NumDescriptors = 1;               // Descriptorは1つ
	ranges[1].BaseShaderRegister = 0;              // シェーダ側の開始インデックスは0番
	ranges[1].RegisterSpace = 0;               // TODO: SM5.1からのspaceだけど、どういうものかよくわからない
	ranges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;  // TODO: とりあえず-1を入れておけばOK？

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   // このパラメータはDescriptorTableとして使用する
	rootParameters[0].DescriptorTable.NumDescriptorRanges = 1;        // DescriptorRangeの数は1つ
	rootParameters[0].DescriptorTable.pDescriptorRanges = &ranges[0];       // DescriptorRangeの先頭アドレス
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;     // このパラメータは頂点シェーダからのみ見える
																			 // D3D12_SHADER_VISIBILITY_ALL にすればすべてのシェーダからアクセス可能

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   // このパラメータはDescriptorTableとして使用する
	rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;        // DescriptorRangeの数は1つ
	rootParameters[1].DescriptorTable.pDescriptorRanges = &ranges[1];       // DescriptorRangeの先頭アドレス
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;     // このパラメータは頂点シェーダからのみ見える
	

	D3D12_ROOT_SIGNATURE_DESC desc;
	desc.NumParameters = _countof(rootParameters);
	desc.pParameters = rootParameters;
	desc.NumStaticSamplers = 1;
	desc.pStaticSamplers = &sampler;
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* pSignature;
	CheckHR(D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &pSignature, nullptr));

	CheckHR(cDirectX12::GetDevice()->CreateRootSignature(0, pSignature->GetBufferPointer(), pSignature->GetBufferSize(), IID_PPV_ARGS(&mRootSignature)));

	{
		ID3D10Blob *vs, *ps;
		{
			ID3D10Blob *info;
			UINT flag = 0;
#if _DEBUG
			flag |= D3DCOMPILE_DEBUG;
#endif /* _DEBUG */
			CheckHR(D3DCompileFromFile(L"Private/Mesh.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", flag, 0, &vs, &info));
			CheckHR(D3DCompileFromFile(L"Private/Mesh.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", flag, 0, &ps, &info));
		}
		D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.InputLayout.NumElements = 3;
		psoDesc.InputLayout.pInputElementDescs = inputLayout;
		psoDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		psoDesc.pRootSignature = mRootSignature.Get();
		psoDesc.VS.pShaderBytecode = vs->GetBufferPointer();
		psoDesc.VS.BytecodeLength = vs->GetBufferSize();
		psoDesc.PS.pShaderBytecode = ps->GetBufferPointer();
		psoDesc.PS.BytecodeLength = ps->GetBufferSize();
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(CD3DX12_DEFAULT());
		psoDesc.BlendState = CD3DX12_BLEND_DESC(CD3DX12_DEFAULT());
		psoDesc.DepthStencilState.DepthEnable = true;
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		psoDesc.DepthStencilState.StencilEnable = false;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.SampleDesc.Count = 1;
		CheckHR(cDirectX12::GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(m_Pso.ReleaseAndGetAddressOf())));
		vs->Release();
		ps->Release();
	}

	MeshLoad();
}

void cRootSignatureTest::MeshLoad()
{
	WaveFrontReader<uint16_t> mesh;
	CheckHR(mesh.Load(L"Private/teapot.obj"));

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

	ID3D12DescriptorHeap* descHeaps[] = { m_ConstBuf.GetDescriptorHeap()};
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
		cmdList->SetPipelineState(m_Pso.Get());
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		cmdList->IASetVertexBuffers(0, 1, &mVBView);
		cmdList->IASetIndexBuffer(&mIBView);
		cmdList->DrawIndexedInstanced(mIndexCount, 1, 0, 0, 0);
	}
}
