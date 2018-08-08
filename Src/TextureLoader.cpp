#include "TextureLoader.h"
#include "Utility.h"
#include <WICTextureLoader.h>
#include <d3dx12.h>
#include <vector>

using namespace Microsoft::WRL;
using namespace DirectX;

cResourceCreateCommand cTextureLoader::m_Command;

void cTextureLoader::LoadTextureFromFile(std::string fileName, cTexture * texture)
{
	ComPtr<ID3D12Resource> textureUploadHeap;		// 一時変数でよし
	ID3D12DescriptorHeap* srvHeap = texture->GetDescriptorHeap().Get();
	ID3D12Resource* tex = texture->GetTextureResource().Get();
	auto m_commandList = m_Command.GetList().Get();
	auto m_commandQueue = m_Command.GetQueue().Get();
	// stringをwstringへ変換する
	std::wstring wStr = std::wstring(fileName.begin(), fileName.end());
	const UINT subresoucesize = 1;

	ResourceLoading(wStr,  tex, textureUploadHeap, subresoucesize);
	SetBarrier(tex);
	CreateShaderResourceView(tex, srvHeap, textureUploadHeap, subresoucesize);
	ResourceUpload();

	texture->SetFilePath(fileName);	// パスを登録しておく
}

void cTextureLoader::Init(ID3D12Device * dev)
{
	m_Command.Init(dev);
}

void cTextureLoader::ResourceLoading(std::wstring& wStr, ID3D12Resource*& tex, ComPtr<ID3D12Resource>& textureUploadHeap, const UINT subresoucesize)
{
	// テクスチャをロードする
	std::unique_ptr<uint8_t[]> textureData;
	D3D12_SUBRESOURCE_DATA subresouceData;
	CheckHR(LoadWICTextureFromFile(cDirectX12::GetDevice(),
		wStr.c_str(),
		&tex,
		textureData,
		subresouceData));

	// データのアップロードに使用するバッファの必要なサイズを取得。
	
	const UINT64 uploadBufferSize
		= GetRequiredIntermediateSize(tex, 0, subresoucesize);

	// アップロード用バッファの作成
	CheckHR(cDirectX12::GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&textureUploadHeap)));

	m_Command.GetList()->Reset(m_Command.GetAlloc().Get(), nullptr);

	// サブリソースの更新
	UpdateSubresources(m_Command.GetList().Get(),
		tex,										// 更新先アドレス
		textureUploadHeap.Get(),		// 中間アドレス
		0,											// オフセット値
		0,											// リソース内の最初のサブリソースのインデックス
		subresoucesize,					// リソース内のサブリソースの数
		&subresouceData);					// 更新に利用するサブリソースデータを含む配列アドレス
		tex->SetName(L"TextureResource");
}

void cTextureLoader::SetBarrier(ID3D12Resource*& tex)
{
	// バリアの設置
	m_Command.GetList()->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(tex,
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}

void cTextureLoader::CreateShaderResourceView(ID3D12Resource*& tex, ID3D12DescriptorHeap*& srvHeap, Microsoft::WRL::ComPtr<ID3D12Resource>& textureUploadHeap, const UINT subresoucesize)
{
	D3D12_RESOURCE_DESC textureDesc = tex->GetDesc();

	// Describe and create a SRV for the texture.
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = textureDesc.Format;	// テクスチャフォーマット(TKが取得してくれたもの。基本RGBA8)
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = subresoucesize;
	cDirectX12::GetDevice()->CreateShaderResourceView(tex,
		&srvDesc,
		srvHeap->GetCPUDescriptorHandleForHeapStart());

	// 不要になった中間データを破棄するコマンド
	m_Command.GetList().Get()->DiscardResource(textureUploadHeap.Get(), nullptr);
}

void cTextureLoader::ResourceUpload()
{
	// Close the command list and execute it to begin the initial GPU setup.
	CheckHR(m_Command.GetList()->Close());
	ID3D12CommandList* ppCommandLists[] = { m_Command.GetList().Get() };
	m_Command.GetQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// GPUにアップロードされるのを待つ
	cFenceObj FenceObj;
	FenceObj.WaitForPreviousFrame(m_Command.GetQueue().Get());
}

