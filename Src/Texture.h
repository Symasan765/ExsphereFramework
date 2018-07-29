/*=================================================
//								FILE :	Texture.h
//		ファイル説明 :
//		テクスチャリソースを管理するためのクラス
//
//									松本 雄之介
=================================================*/
#pragma once
#include <wrl/client.h>
#include <d3d12.h>
#include <string>

class cTexture {
public:
	cTexture(D3D12_DESCRIPTOR_HEAP_FLAGS flag = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);			// flag変数はどこのシェーダから読めるようにするか
	~cTexture();
	inline Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return m_srvHeap; };
	inline Microsoft::WRL::ComPtr<ID3D12Resource> GetTextureResource() { return m_pTex; };
	inline std::string GetPath() { return m_FilePath; };
	void SetFilePath(std::string path);
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_pTex;		// テクスチャリソース
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_srvHeap;	
	std::string m_FilePath;
};