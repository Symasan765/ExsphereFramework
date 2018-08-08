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
#include "ResourceCreateCommand.h"

class cTextureLoader {
public:
	friend class cMainSystem;
	static void LoadTextureFromFile(std::string fileName, cTexture* texture);
private:
	cTextureLoader() = default;
	~cTextureLoader() = default;
	static void Init(ID3D12Device * dev);

	static cResourceCreateCommand m_Command;
};