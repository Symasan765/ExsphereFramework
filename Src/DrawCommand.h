/*=================================================
//								FILE : DrawCommand.h
//		ファイル説明 :
//		DirectX12の描画コマンドシステム周りの機能を実装する。
//		システム的なマルチスレッドを包括する
//									松本 雄之介
=================================================*/
#pragma once
#include "DirectX12.h"

namespace DrawParam{
	constexpr unsigned g_ThreadNum = 4;		// 並列処理スレッド数
	constexpr unsigned g_MaxFrameLatency = 2;	// 処理フレーム数
}
class cRootSignatureTest;

class cDrawCommand {
public:
	friend class cMainSystem;
	cDrawCommand() = default;
	~cDrawCommand() = default;

	inline static Microsoft::WRL::ComPtr<IDXGISwapChain1> GetSwapChain() { return m_SwapChain; };
	inline static ID3D12CommandAllocator* GetStartAllocator() { return m_CommandAllocator[0][0].Get(); };
	inline static ID3D12GraphicsCommandList* GetGeneralCommandList() { return m_CommandListEpir.Get(); };
	inline static ID3D12CommandQueue* GetCommandQueue() { return m_CommandQueue.Get(); };
	static unsigned GetFrameIndex();
private:
	void FrameUpdate();	// 新しいフレームの最初に呼び出すこと
	void Init();		// 初期化用関数。こいつを呼べば↓の関数群が順に呼び出される
	// ==================================//
	void CreateCommandAllocators();
	void CreateCommandQueue();
	void CreateSwapChain();
	void CreateCommandLists();
	void CreateFence();
	// ==================================//

	// 描画系関数
	void CommandListBuild();		// 描画命令をコマンドリストに入れていく
	void CommandQueueExe();		// コマンドリストの命令を実行する
	void PreDrawingProcess(ID3D12GraphicsCommandList*& cmdListProl,const int& cmdIndex);
	void SetResourceBarrier(ID3D12GraphicsCommandList* commandList, ID3D12Resource* res, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

	void Destroy();

	static Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_CommandAllocator[DrawParam::g_MaxFrameLatency][DrawParam::g_ThreadNum];
	static Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CommandQueue;
	static Microsoft::WRL::ComPtr<IDXGISwapChain1> m_SwapChain;
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandList[DrawParam::g_ThreadNum];
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandListProl;		// 描画前処理用コマンドリスト
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandListEpir;		// 描画後処理用のコマンドリスト(主にテクスチャリソース作成など雑用を担当)
	static Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;
	static D3D12_CPU_DESCRIPTOR_HANDLE m_DescHandleRtv;		// 現在のフレームで使用するRTVハンドルを保持
	static HANDLE m_FenceEveneHandle;
	static unsigned m_DescHandleRtvStep;

	// TODO 以下はテスト環境なので削除すること
	cRootSignatureTest* m_RootSig;
	static unsigned m_FrameCount;
};