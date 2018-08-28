/*=================================================
//								FILE :	RenderingFramework.h
//		ファイル説明 :
//		レンダリングの制御を行うフレームワーク。
//
//									松本 雄之介
=================================================*/
#pragma once
#include "DirectX12.h"
#include <string>
#include <unordered_map>

class cRenderingFramework
{
public:
	cRenderingFramework() = default;
	~cRenderingFramework() = default;

	/// <summary>
	/// ファイル名を元にリソースIDを取得する
	/// </summary>
	/// <param name="fileName">読み込むファイル名。パスは自動的に付与する</param>
	static UINT GetID(std::string fileName);
private:
	static std::unordered_map<std::string, UINT> m_IDMap;
};