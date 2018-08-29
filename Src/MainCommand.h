/*=================================================
//								FILE :	MainCommand.h
//		ファイル説明 :
//		ゲーム中で利用するメインの描画コマンドを管理するクラス
//
//									松本 雄之介
=================================================*/
#pragma once
#include<memory>
#include "MainCommandAllocators.h"
#include "MainCommandLists.h"
#include "RenderBufferStruct.h"

class cRootSignatureTest;
class cMainCommand {
public:
	cMainCommand() = default;
	~cMainCommand() = default;
	void Create(ID3D12Device * dev);
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetSelectAlloc(UINT vertical, UINT Horizontal);

	void DrawBegin(RenderBufferStruct& data,const unsigned cmdIndex);

	void DrawGameScene(RenderBufferStruct& data, const unsigned cmdIndex);		// 仮実装。並列処理に対応していない

	void DrawEnd(RenderBufferStruct& data, const unsigned cmdIndex);
	void SetResourceBarrier(ID3D12GraphicsCommandList* commandList, ID3D12Resource* res, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetPrologueList();
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetEpilogueList();
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetMainCommandLists();	// TODO テスト実装！現在は複数のリストを取得できないようになっている

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>* GetMainCommandListPtr();
private:
	std::unique_ptr<cMainCommandAllocators> m_Allocators;
	std::unique_ptr<cMainCommandLists> m_Lists;

	cRootSignatureTest* m_RootSig;
};