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

class cDrawCommand {
public:
	friend class cMainSystem;
	cDrawCommand() = default;
	~cDrawCommand() = default;

	inline static Microsoft::WRL::ComPtr<IDXGISwapChain1> GetSwapChain() { return m_SwapChain; };
private:
	void Init();		// 初期化用関数。こいつを呼べば↓の関数群が順に呼び出される
	// ==================================//
	void CreateCommandAllocators();
	void CreateCommandQueue();
	void CreateSwapChain();
	void CreateCommandLists();
	void CreateFence();
	// ==================================//
	void Destroy();

	static Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_CommandAllocator[DrawParam::g_MaxFrameLatency][DrawParam::g_ThreadNum];
	static Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CommandQueue;
	static Microsoft::WRL::ComPtr<IDXGISwapChain1> m_SwapChain;
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandList[DrawParam::g_ThreadNum];
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandListProl;		// 描画前処理用コマンドリスト
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandListEpir;		// 描画後処理用のコマンドリスト
	static Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;
	static HANDLE m_FenceEveneHandle;
};