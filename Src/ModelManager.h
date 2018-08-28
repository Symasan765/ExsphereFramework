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

	/// <summary>
	/// ファイル名を元にリソースIDを取得する
	/// </summary>
	/// <param name="fileName">読み込むファイル名。パスは自動的に付与する</param>
	UINT GetID(std::string fileName);
private:
	std::unordered_map<std::string, UINT> m_IDMap;
	std::unordered_map<UINT, std::unique_ptr<cModelResource>> m_ResourceMap;
};