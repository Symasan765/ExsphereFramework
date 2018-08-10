/*=================================================
//								FILE :	Mesh3D.h
//		�t�@�C������ :
//		���b�V�������i�[����N���X���`����
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include <vector>
#include "DirectX12.h"
#include "VertexStruct.h"

class cMesh3D
{
public:
	cMesh3D() = default;
	~cMesh3D() = default;

	void DataSet(const std::vector<DefaultVertex>& vertices,
		const std::vector<UINT>& indices);
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> mVB;
	UINT mIndexCount = 0;
	UINT mVBIndexOffset = 0;
	D3D12_VERTEX_BUFFER_VIEW mVBView = {};
	D3D12_INDEX_BUFFER_VIEW mIBView = {};
};