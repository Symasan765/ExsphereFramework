/*=================================================
//								FILE :	TextureLoader.h
//		�t�@�C������ :
//		png��jpg�Ȃǂ̉摜�����[�h���ADX12�ɂė��p�ł��郊�\�[�X�Ƃ���
//		���p�ł���悤�ɏ�������N���X���L�q����
//									���{ �Y�V��
=================================================*/
#pragma once
#include "Texture.h"
#include "DirectX12.h"
#include "FenceObj.h"
#include <string>

class cTextureLoader {
public:
	static void LoadTextureFromFile(std::string fileName, cTexture* texture);
private:
	cTextureLoader() = default;
	~cTextureLoader() = default;
};