#include "MainCommand.h"
#include "Utility.h"

void cMainCommand::Create(ID3D12Device * dev)
{
	m_Allocators.reset(new cMainCommandAllocators);
	m_Lists.reset(new cMainCommandLists);

	m_Allocators->Create(dev);
	m_Lists->Create(dev, m_Allocators->GetBeginAlloc().Get());
}

Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cMainCommand::GetSelectAlloc(UINT vertical, UINT Horizontal)
{
	return m_Allocators->GetSelectAlloc(vertical, Horizontal);
}

void cMainCommand::DrawBegin(RenderBufferStruct& data, const unsigned cmdIndex)
{
	// 前処理スタート
	CheckHR(m_Lists->GetBegin()->Reset(m_Allocators->GetSelectAlloc(cmdIndex,0).Get(), nullptr));

	// 現在の描画バッファの取得
	ID3D12Resource* d3dBuffer = data.buffer.Get();

	// バリアをレンダーターゲットに設定する
	SetResourceBarrier(m_Lists->GetBegin().Get(), d3dBuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	// デプスステンシルをクリア
	m_Lists->GetBegin()->ClearDepthStencilView(data.descHandleDsv, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// レンダーターゲットをクリア
	{
		const float ClearColor[4] = { 0.1f, 0.2f, 0.6f, 1.0f };
		m_Lists->GetBegin()->ClearRenderTargetView(data.descHandleRtv, ClearColor, 0, nullptr);
	}

	// 描画前処理の終了
	CheckHR(m_Lists->GetBegin()->Close());
}

void cMainCommand::DrawEnd(RenderBufferStruct & data, const unsigned cmdIndex)
{
	CheckHR(m_Lists->GetEnd()->Reset(m_Allocators->GetSelectAlloc(cmdIndex, 0).Get(), nullptr));
	SetResourceBarrier(m_Lists->GetEnd().Get(), data.buffer.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	CheckHR(m_Lists->GetEnd()->Close());
}

void cMainCommand::SetResourceBarrier(ID3D12GraphicsCommandList* commandList,	// コマンドリスト
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