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
private:
	static std::unordered_map<UINT, std::unique_ptr<cModelResource>> m_ResourceMap;
};