/*=================================================
//								FILE :	TextureLoader.h
//		ファイル説明 :
//		pngやjpgなどの画像をロードし、DX12にて利用できるリソースとして
//		利用できるように処理するクラスを記述する
//									松本 雄之介
=================================================*/
#pragma once
#include "Texture.h"
#include "DirectX12.h"
#include "FenceObj.h"
#include <string>
#include "ResourceCreateCommand.h"

class cTextureLoader {
public:
	friend class cMainSystem;
	static void LoadTextureFromFile(std::string fileName, cTexture* texture);
private:
	cTextureLoader() = default;
	~cTextureLoader() = default;
	static void Init(ID3D12Device * dev);
	static void ResourceLoading(std::wstring& wStr, ID3D12Resource*& tex, Microsoft::WRL::ComPtr<ID3D12Resource>& textureUploadHeap, const UINT subresoucesize);
	static void SetBarrier(ID3D12Resource*& tex);
	static void CreateShaderResourceView(ID3D12Resource*& tex, ID3D12DescriptorHeap*& srvHeap, Microsoft::WRL::ComPtr<ID3D12Resource>& textureUploadHeap, const UINT subresoucesize);
	static void ResourceUpload();

	static cResourceCreateCommand m_Command;
};