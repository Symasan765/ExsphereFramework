/*=================================================
//								FILE :	ResourceCreateCommand.h
//		ファイル説明 :
//		リソース作成のために利用するためのコマンドア機能を定義する
//
//									松本 雄之介
=================================================*/
#pragma once
#include "DirectX12.h"

class cResourceCreateCommand
{
public:
	cResourceCreateCommand() = default;
	~cResourceCreateCommand() = default;
	void Init(ID3D12Device * dev);

	inline Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetList() { return m_List; };
	inline Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetAlloc() { return m_Allocator; };
	inline Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetQueue() { return m_CommandQueue; };
private:
	void InitQueue(ID3D12Device * dev);
	void InitAlloc(ID3D12Device * dev);
	void InitList(ID3D12Device * dev);

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_List;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_Allocator;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CommandQueue;
};
