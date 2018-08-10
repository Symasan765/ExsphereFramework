/*=================================================
//								FILE :	Graphic3D.h
//		�t�@�C������ :
//		���b�V���A�}�e���A���A�A�j���[�V�����f�[�^��ێ�����N���X
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include "Mesh3D.h"
#include "Material.h"
#include "Animation3D.h"

class cGraphic3D
{
public:
	cGraphic3D(cMesh3D mesh,cMaterial material, cAnimation3D anim);
	~cGraphic3D() = default;

	cMesh3D m_Mesh;
	cMaterial m_Material;
	cAnimation3D m_Animation3D;
};