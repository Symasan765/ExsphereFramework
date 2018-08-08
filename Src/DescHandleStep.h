/*=================================================
//								FILE :	DescHandleStep.h
//		ファイル説明 :
//		各種ヒープサイズを取得するクラスを作成する
//
//									松本 雄之介
=================================================*/
#pragma once
#include "DirectX12.h"

/// <summary>
/// ヒープサイズの取得を行うクラス
/// </summary>
class cDescHandleStep
{
public:
	cDescHandleStep() = default;
	~cDescHandleStep() = default;
	static UINT GetSize(D3D12_DESCRIPTOR_HEAP_TYPE type);
	void Init(ID3D12Device* dev);
private:
	static UINT m_Size[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];
};