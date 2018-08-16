/*=================================================
//								FILE :	Material.h
//		ファイル説明 :
//		マテリアル情報を格納するクラス
//
//									松本 雄之介
=================================================*/
#pragma once
#include <vector>
#include "Texture.h"

class cMaterial
{
public:
	cMaterial() = default;
	~cMaterial() = default;

	void DataSet(const std::vector<cTexture>& textures);
	void SetTexture(ID3D12GraphicsCommandList * cmdList, UINT RootParameterIndex);
private:
	std::vector<cTexture> m_Tex;
};