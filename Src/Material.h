/*=================================================
//								FILE :	Material.h
//		�t�@�C������ :
//		�}�e���A�������i�[����N���X
//
//									���{ �Y�V��
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