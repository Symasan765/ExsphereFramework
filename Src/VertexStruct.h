/*=================================================
//								FILE :	VertexStruct.h
//		�t�@�C������ :
//		���_�\�����`����
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include <Windows.h>
#include <DirectXMath.h>

struct DefaultVertex {
	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 normal = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT2 texcoord = { 0.0f,0.0f };
	DirectX::XMFLOAT3 tangent = { 0.0f,0.0f,0.0f };
	DirectX::XMINT4 bindex = { 0,0,0,0 };					// �e������{�[���C���f�b�N�X
	DirectX::XMFLOAT4 bweight = { 0.0f,0.0f,0.0f,0.0f };	// �e�{�[���̏d�ݏ��
};

struct VERTEX_TEST {
	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 normal = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT2 texcoord = { 0.0f,0.0f };
};