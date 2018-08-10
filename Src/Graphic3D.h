/*=================================================
//								FILE :	Graphic3D.h
//		ファイル説明 :
//		メッシュ、マテリアル、アニメーションデータを保持するクラス
//
//									松本 雄之介
=================================================*/
#pragma once
#include "Mesh3D.h"
#include "Material.h"
#include "Animation3D.h"

class cGraphic3D
{
public:
	cGraphic3D(const cMesh3D& mesh,const cMaterial& material, const cAnimation3D& anim);
	~cGraphic3D() = default;

	cMesh3D m_Mesh;
	cMaterial m_Material;
	cAnimation3D m_Animation3D;
private:
	cGraphic3D() = default;
};