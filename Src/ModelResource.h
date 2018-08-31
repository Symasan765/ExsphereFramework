/*=================================================
//								FILE :	ModelResource.h
//		�t�@�C������ :
//		��t�@�C�����̃��f���f�[�^����舵���N���X
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include "Graphic3D.h"
#include "BoneStruct.h"
#include "InstVertexBuffer.h"

class cModelResource
{
public:
	cModelResource() = default;
	~cModelResource() = default;
	void Draw(DirectX::XMFLOAT4X4* matPtr, ID3D12GraphicsCommandList * cmdList,bool texDrawFlag,UINT RootParameterIndex, UINT InstanceCount);

	std::vector<cGraphic3D> m_Graphic;
	std::vector<BoneStruct> m_Bone;
	cInstVertexBuffer m_InstBuffer;
};