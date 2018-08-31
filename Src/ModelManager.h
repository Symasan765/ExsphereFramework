/*=================================================
//								FILE :	ModelManager.h
//		ファイル説明 :
//		モデルのロードを管理するマネージャークラス
//		モデルを保持しておき、大元にはリソースIDを返す
//									松本 雄之介
=================================================*/
#pragma once
#include <string>
#include <unordered_map>
#include "DirectX12.h"
#include "ModelResource.h"

class cModelManager
{
public:
	cModelManager() = default;
	~cModelManager() = default;

	static UINT Load(std::string fileName);
	void Draw(DirectX::XMFLOAT4X4* matPtr,UINT ResourceID, ID3D12GraphicsCommandList * cmdList, bool texDrawFlag, UINT RootParameterIndex, UINT InstanceCount);
private:
	static std::unordered_map<UINT, std::unique_ptr<cModelResource>> m_ResourceMap;
};