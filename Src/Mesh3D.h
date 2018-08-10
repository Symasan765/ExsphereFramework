/*=================================================
//								FILE :	Mesh3D.h
//		ファイル説明 :
//		メッシュ情報を格納するクラスを定義する
//
//									松本 雄之介
=================================================*/
#pragma once
#include <vector>
#include "DirectX12.h"
#include "VertexStruct.h"

class cMesh3D
{
public:
	cMesh3D(const std::vector<DefaultVertex>& vertices,
	const std::vector<UINT>& indices);

	~cMesh3D() = default;

private:
	std::vector<DefaultVertex> vertices;
	std::vector<UINT> indices;



	Microsoft::WRL::ComPtr<ID3D12Resource> mVB;
	UINT mIndexCount = 0;
	UINT mVBIndexOffset = 0;
	D3D12_VERTEX_BUFFER_VIEW mVBView = {};
	D3D12_INDEX_BUFFER_VIEW mIBView = {};
};