/*=================================================
//								FILE :	ModelResource.h
//		ファイル説明 :
//		一ファイル分のモデルデータを取り扱うクラス
//
//									松本 雄之介
=================================================*/
#pragma once
#include "Graphic3D.h"

class cModelResource
{
public:
	cModelResource() = default;
	~cModelResource() = default;


	std::vector<cGraphic3D> m_Graphic;
	std::vector<BoneStruct> m_Bone;
};