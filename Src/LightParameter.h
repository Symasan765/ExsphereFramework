/*=================================================
//								FILE :	LightParameter.h
//		�t�@�C������ :
//		���C�e�B���O�Ŏg�p���郉�C�g�̃p�����[�^���`����\����
//
//									���{ �Y�V��
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