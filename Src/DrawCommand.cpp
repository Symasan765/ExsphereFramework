/*=================================================
//								FILE :
//		ファイル説明 :
//		描画で利用するコマンドアロケーターやコマンドリストなどを保持し、
//		必要に応じて処理を行うクラス
//									松本 雄之介
=================================================*/
#include "DrawCommand.h"
#include "MainWindow.h"
#include "Utility.h"
#include <stdexcept>

using namespace DrawParam;

// 各種static変数宣言
Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cDrawCommand::m_CommandAllocator[DrawParam::g_MaxFrameLatency][DrawParam::g_ThreadNum];
Microsoft::WRL::ComPtr<ID3D12CommandQueue> cDrawCommand::m_CommandQueue;
Microsoft::WRL::ComPtr<IDXGISwapChain1> cDrawCommand::m_SwapChain;
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cDrawCommand::m_CommandList[DrawParam::g_ThreadNum];
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cDrawCommand::m_CommandListProl;
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cDrawCommand::m_CommandListEpir;
Microsoft::WRL::ComPtr<ID3D12Fence> cDrawCommand::m_Fence;
HANDLE cDrawCommand::m_FenceEveneHandle = 0;
unsigned cDrawCommand::m_FrameCount = 0;

namespace {
	// コマンドの作成と実行の間で処理するアドレスを保持しておく変数
	ID3D12CommandQueue* g_CommandQueueTmp;
	ID3D12CommandList* g_CmdListsProlTmp;
}

// この関数を呼び出すことで順に初期化を行う
void cDrawCommand::Init()
{
	CreateCommandAllocators();
	CreateCommandQueue();
	CreateSwapChain();
	CreateCommandLists();
	CreateFence();
}

void cDrawCommand::CreateCommandAllocators()
{
	for (auto& LatencyArray : m_CommandAllocator)
	{
		for (auto& ThreadArray : LatencyArray)
		{
			CheckHR(cDirectX12::GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(ThreadArray.ReleaseAndGetAddressOf())));
		}
	}
}

void cDrawCommand::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	CheckHR(cDirectX12::GetDevice()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(m_CommandQueue.ReleaseAndGetAddressOf())));
}

void cDrawCommand::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Width = WindowOptions::g_WindowSizeX;
	scDesc.Height = WindowOptions::g_WindowSizeY;
	scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.SampleDesc.Count = 1;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = WindowOptions::g_FrameBuuferNum;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	CheckHR(cDirectX12::GetDxgiFactory()->CreateSwapChainForHwnd(m_CommandQueue.Get(), cMainWindow::GetHWND(), &scDesc, nullptr, nullptr, m_SwapChain.ReleaseAndGetAddressOf()));
}

void cDrawCommand::CreateCommandLists()
{
	// ゲーム中のオブジェクトなど、メインとなるコマンドリストを作成する。
	{
		for (int i = 0; i < g_ThreadNum; i++)
		{
			CheckHR(cDirectX12::GetDevice()->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				m_CommandAllocator[0][0].Get(),
				nullptr,
				IID_PPV_ARGS(m_CommandList[i].ReleaseAndGetAddressOf())));

			m_CommandList[i]->Close();		// 作成直後は記録状態になっているので閉じておく
		}
	}

	// 次にバッファのクリアなど描画前準備用のコマンドリストを作成する
	{
		CheckHR(cDirectX12::GetDevice()->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_CommandAllocator[0][0].Get(),
			nullptr,
			IID_PPV_ARGS(m_CommandListProl.ReleaseAndGetAddressOf())));
		m_CommandListProl->Close();		// 上と同じく閉じておく
	}

	// 最後に後処理用のコマンドリストの作成
	{
		CheckHR(cDirectX12::GetDevice()->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_CommandAllocator[0][0].Get(),
			nullptr,
			IID_PPV_ARGS(m_CommandListEpir.ReleaseAndGetAddressOf())));
		m_CommandListEpir->Close();
	}
}

void cDrawCommand::CreateFence()
{
	CheckHR(cDirectX12::GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_Fence.ReleaseAndGetAddressOf())));

	m_FenceEveneHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void cDrawCommand::CommandListBuild()
{
	m_FrameCount++;
	int cmdIndex = m_FrameCount % g_MaxFrameLatency;	// 何番のコマンドアロケーターを利用するか？
	auto* cmdQueue = m_CommandQueue.Get();
	auto* cmdListProl = m_CommandListProl.Get();
	auto* cmdListEpir = m_CommandListEpir.Get();

	// コマンドが溜まり切るのを待っている
	if (m_FrameCount > g_MaxFrameLatency)
	{
		m_Fence->SetEventOnCompletion(m_FrameCount - g_MaxFrameLatency, m_FenceEveneHandle);
		DWORD wait = WaitForSingleObject(m_FenceEveneHandle, 10000);	//シグナル状態になるのをここで待ってくれているっぽい

		//シグナル状態にならなかった
		if (wait != WAIT_OBJECT_0)
			throw std::runtime_error("Failed WaitForSingleObject().");

		CheckHR(m_CommandAllocator[cmdIndex][0]->Reset());
	}

	// 描画の前処理
	PreDrawingProcess(cmdListProl,cmdIndex);

	// コマンド実行時に使用する各アドレスをグローバルで保持しておく
	g_CmdListsProlTmp = cmdListProl;
	g_CommandQueueTmp = cmdQueue;
}

void cDrawCommand::CommandQueueExe()
{
	// Exec
	ID3D12CommandList* const cmdListsProl = g_CmdListsProlTmp;
	auto* cmdQueue = g_CommandQueueTmp;

	cmdQueue->ExecuteCommandLists(1, &cmdListsProl);
	CheckHR(cmdQueue->Signal(m_Fence.Get(), m_FrameCount));

	// Present
	CheckHR(m_SwapChain->Present(1, 0));
}

void cDrawCommand::PreDrawingProcess(ID3D12GraphicsCommandList*& cmdListProl, const int& cmdIndex)
{
	// 前処理スタート
	CheckHR(cmdListProl->Reset(m_CommandAllocator[cmdIndex][0].Get(), nullptr));

	// 現在のレンダーターゲットデスクリプタを取得する
	auto descHandleRtvStep = cDirectX12::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);	// RTVのポインタサイズを取得している
	D3D12_CPU_DESCRIPTOR_HANDLE descHandleRtv = cMainWindow::GetDescHeapRtv()->GetCPUDescriptorHandleForHeapStart();		// 先頭アドレスを取得
	descHandleRtv.ptr += ((m_FrameCount - 1) % WindowOptions::g_FrameBuuferNum) * descHandleRtvStep;		// 先頭アドレス += (No * アドレスサイズ)

																											// 現在の描画バッファの取得
	ID3D12Resource* d3dBuffer = cMainWindow::GetBuffer((m_FrameCount - 1) % WindowOptions::g_FrameBuuferNum).Get();

	// デプスステンシルの取得
	auto descHandleDsv = cMainWindow::GetDescHeapDsv()->GetCPUDescriptorHandleForHeapStart();

	// バリアをレンダーターゲットに設定する
	SetResourceBarrier(cmdListProl, d3dBuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	// デプスステンシルをクリア
	cmdListProl->ClearDepthStencilView(descHandleDsv, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// レンダーターゲットをクリア
	{
		const float ClearColor[4] = { 0.1f, 0.2f, 0.6f, 1.0f };
		cmdListProl->ClearRenderTargetView(descHandleRtv, ClearColor, 0, nullptr);
	}

	// 描画前処理の終了
	CheckHR(cmdListProl->Close());
}

void cDrawCommand::SetResourceBarrier(ID3D12GraphicsCommandList* commandList,	// コマンドリスト
	ID3D12Resource* res,		// 現在のバッファ
	D3D12_RESOURCE_STATES before,		// 設定しなおす際に以前の状態も入れる必要があるため
	D3D12_RESOURCE_STATES after)
{
	D3D12_RESOURCE_BARRIER desc = {};
	desc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	desc.Transition.pResource = res;
	desc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	desc.Transition.StateBefore = before;
	desc.Transition.StateAfter = after;
	desc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	commandList->ResourceBarrier(1, &desc);
}

void cDrawCommand::Destroy()
{
	CloseHandle(m_FenceEveneHandle);
}
