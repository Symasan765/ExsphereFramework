/*=================================================
//								FILE :	TextureLoader.h
//		ファイル説明 :
//		pngやjpgなどの画像をロードし、DX12にて利用できるリソースとして
//		利用できるように処理するクラスを記述する
//									松本 雄之介
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