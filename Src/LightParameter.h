/*=================================================
//								FILE :	LightParameter.h
//		ファイル説明 :
//		ライティングで使用するライトのパラメータを定義する構造体
//
//									松本 雄之介
=================================================*/
#pragma once
#include "DXMath.h"

struct DirectionalLight {
	DirectX::XMFLOAT3 direction;
	DirectX::XMFLOAT3 color;
};

struct PointLight {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 color;
	float distance;
	float decay;
};

struct SpotLight {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 direction;
	DirectX::XMFLOAT3 color;
	float distance;
	float decay;
	float coneCos;
	float penumbraCos;
};