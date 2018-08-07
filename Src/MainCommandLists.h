/*=================================================
//								FILE :	MainCommandLists.h
//		ファイル説明 :
//		ゲームループ中でメインに使用するコマンドリストを定義する
//
//									松本 雄之介
=================================================*/
#pragma once
#include "DirectX12.h"
#include "DrawParam.h"

class cMainCommandLists {
public:
	cMainCommandLists() = default;
	~cMainCommandLists() = default;
	void Create(ID3D12Device* dev,ID3D12CommandAllocator* alloc);
private:
	void MainCreate(ID3D12Device* dev, ID3D12CommandAllocator* alloc);
	void ProlCreate(ID3D12Device* dev, ID3D12CommandAllocator* alloc);
	void EpirCreate(ID3D12Device* dev, ID3D12CommandAllocator* alloc);
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandList[DrawParam::g_ThreadNum];
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandListProl;		// 描画前処理用コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandListEpir;		// 描画後処理用のコマンドリスト(主にテクスチャリソース作成など雑用を担当)
};