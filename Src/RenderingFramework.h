/*=================================================
//								FILE :	RenderingFramework.h
//		ファイル説明 :
//		レンダリングの制御を行うフレームワーク。
//
//									松本 雄之介
=================================================*/
#pragma once
#include "DirectX12.h"
#include "DrawParam.h"
#include <string>
#include <unordered_map>
#include <vector>

class cRenderComponent;

struct DrawObjMap {
	std::unordered_map<UINT, std::vector<cRenderComponent*>> m_Map;		//リソースIDにレンダリング対象を配列で紐づける
};

// アクセス権をstaticの有無でコントロールする
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

	/// <summary>
	/// シーンから描画対象のオブジェクトを登録する関数
	/// </summary>
	/// <param name="ResourceID">ResourceID</param>
	/// <param name="pRenderPtr">レンダリングコンポーネントのアドレス</param>
	static void RnederingRegister(UINT ResourceID, cRenderComponent* pRenderPtr);

	/// <summary>
	/// コマンドリストへコマンドの発行を行う関数。
	/// </summary>
	void CommandIssue(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>* cmdLists,int listNum);
private:
	static std::unordered_map<std::string, UINT> m_IDMap;
	static DrawObjMap m_DrawObjMap[DrawParam::g_ThreadNum];		//リソースIDにレンダリング対象を配列で紐づける
};