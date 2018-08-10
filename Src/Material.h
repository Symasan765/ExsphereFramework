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
	cMaterial() = default;
	~cMaterial() = default;

	void DataSet(const std::vector<cTexture>& textures);
private:
	std::vector<cTexture> m_Tex;
};