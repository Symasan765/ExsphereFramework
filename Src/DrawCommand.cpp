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
#include "ImGUIManager.h"
#include <stdexcept>
#include "RootSignatureTest.h"		// TODO 暫定処理。後々消す
// TODO 現在のバッファをあらかじめ取得できるようにしておくこと

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
D3D12_CPU_DESCRIPTOR_HANDLE cDrawCommand::m_DescHandleRtv;
unsigned cDrawCommand::m_FrameCount = 0;
unsigned cDrawCommand::m_DescHandleRtvStep;

namespace {
	// コマンドの作成と実行の間で処理するアドレスを保持しておく変数
	ID3D12CommandQueue* g_CommandQueueTmp;
	ID3D12CommandList* g_CmdListsProlTmp;
	ID3D12CommandList* g_CmdListsEpirTmp;
}

unsigned cDrawCommand::GetFrameIndex()
{
	return m_FrameCount % g_MaxFrameLatency;
}

void cDrawCommand::FrameUpdate()
{
	// フレームカウントを進める
	m_FrameCount++;
	// フレームバッファ情報を更新する
	m_DescHandleRtv = cMainWindow::GetDescHeapRtv()->GetCPUDescriptorHandleForHeapStart();		// 先頭アドレスを取得
	m_DescHandleRtv.ptr += ((m_FrameCount - 1) % WindowOptions::g_FrameBuuferNum) * m_DescHandleRtvStep;		// 先頭アドレス += (No * アドレスサイズ)
}

// この関数を呼び出すことで順に初期化を行う
void cDrawCommand::Init()
{
	CreateCommandAllocators();
	CreateCommandQueue();
	CreateSwapChain();
	CreateCommandLists();
	CreateFence();
	m_DescHandleRtvStep = cDirectX12::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);	// RTVのポインタサイズを取得している

	m_RootSig = new cRootSignatureTest();
	m_RootSig->Init();
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
	int cmdIndex = GetFrameIndex();	// 何番のコマンドアロケーターを利用するか？
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
	PreDrawingProcess(cmdListProl, cmdIndex);

	// TODO ここ見直すこと
	auto* cmdList = m_CommandList[0].Get();
	CheckHR(cmdList->Reset(m_CommandAllocator[cmdIndex][0].Get(), nullptr));
	cmdList->OMSetRenderTargets(1, &m_DescHandleRtv, true, &cMainWindow::GetDescHeapDsv()->GetCPUDescriptorHandleForHeapStart());
	m_RootSig->Draw(cmdList);
	// Fix draw command
	CheckHR(cmdList->Close());

	// ImGUI の処理
	auto DescriptorHandleForHeap = cMainWindow::GetDescHeapRtv()->GetCPUDescriptorHandleForHeapStart();
	SIZE_T rtvDescriptorSize = cDirectX12::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	DescriptorHandleForHeap.ptr += rtvDescriptorSize * (m_FrameCount % WindowOptions::g_FrameBuuferNum);

	// 後処理
	CheckHR(cmdListEpir->Reset(m_CommandAllocator[cmdIndex][0].Get(), nullptr));
	SetResourceBarrier(cmdListEpir, cMainWindow::GetBuffer((m_FrameCount - 1) % WindowOptions::g_FrameBuuferNum).Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	CheckHR(cmdListEpir->Close());

	// コマンド実行時に使用する各アドレスをグローバルで保持しておく
	g_CmdListsProlTmp = cmdListProl;
	g_CmdListsEpirTmp = cmdListEpir;
	g_CommandQueueTmp = cmdQueue;
}

void cDrawCommand::CommandQueueExe()
{
	// Exec
	ID3D12CommandList* const cmdListsProl = g_CmdListsProlTmp;
	ID3D12CommandList* const cmdListsEpir = g_CmdListsEpirTmp;
	auto* cmdQueue = g_CommandQueueTmp;

	// 前処理
	cmdQueue->ExecuteCommandLists(1, &cmdListsProl);
	// メイン処理
	ID3D12CommandList* data = m_CommandList[0].Get();
	cmdQueue->ExecuteCommandLists(1, &data);
	// 後処理
	cmdQueue->ExecuteCommandLists(1, &cmdListsEpir);
	// ImGUI処理


	CheckHR(cmdQueue->Signal(m_Fence.Get(), m_FrameCount));

	// Present
	CheckHR(m_SwapChain->Present(1, 0));
}

void cDrawCommand::PreDrawingProcess(ID3D12GraphicsCommandList*& cmdListProl, const int& cmdIndex)
{
	// 前処理スタート
	CheckHR(cmdListProl->Reset(m_CommandAllocator[cmdIndex][0].Get(), nullptr));

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
		cmdListProl->ClearRenderTargetView(m_DescHandleRtv, ClearColor, 0, nullptr);
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
