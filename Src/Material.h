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
	cMaterial(const std::vector<cTexture>& textures);
	~cMaterial() = default;

private:
	std::vector<cTexture> m_Tex;
};