/*=================================================
//								FILE : MainWindow.cpp
//		ファイル説明 :
//		描画されるメインウィンドウの処理を行うクラス
//
//									松本 雄之介
=================================================*/
#include "MainWindow.h"
#include "WindProc.h"
#include "Utility.h"
#include "DrawCommand.h"
#include "DirectX12.h"
#include <d3dx12.h>

using namespace WindowOptions;

HWND cMainWindow::m_hWindow = nullptr;
HINSTANCE cMainWindow::m_hInstance = nullptr;
Microsoft::WRL::ComPtr<ID3D12Resource> cMainWindow::m_D3DBuffer[WindowOptions::g_FrameBuuferNum];
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> cMainWindow::m_DescHeapRtv;	// レンダーターゲット用
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> cMainWindow::m_DescHeapDsv;	// デプスステンシル用
Microsoft::WRL::ComPtr<ID3D12Resource> cMainWindow::mDsvResource;

cMainWindow::cMainWindow(HINSTANCE _hInst)
{
	// アプリケーションのインスタンス ハンドルを保存
	m_hInstance = _hInst;
	m_hWindow = nullptr;
}

cMainWindow::~cMainWindow()
{
	// ウインドウ クラスの登録解除
	UnregisterClass(WindowOptions::g_szWndClass, m_hInstance);
}

Microsoft::WRL::ComPtr<ID3D12Resource> cMainWindow::GetBuffer(const int no)
{
	return m_D3DBuffer[no];
}

HRESULT cMainWindow::CreateMainWindow()
{
	// ウインドウ クラスの登録
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WindowOptions::g_szWndClass;

	if (!RegisterClass(&wc))
		return E_FAIL;

	// メイン ウインドウ作成
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = WindowOptions::g_WindowSizeX;
	rect.bottom = WindowOptions::g_WindowSizeY;
	AdjustWindowRect(&rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_DLGFRAME, TRUE);

	rect.top = 0.0f;		// これを無くすとバー位置を画面外へ追いやり、フルスクリーンのような状態にできる
	m_hWindow = CreateWindow(WindowOptions::g_szWndClass, WindowOptions::g_szAppTitle,
		WindowOptions::dwStyle,
		rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, m_hInstance, NULL);

	if (m_hWindow == NULL)
		return E_FAIL;

	// ウインドウ表示
	ShowWindow(m_hWindow, SW_SHOWNORMAL);
	UpdateWindow(m_hWindow);

	return S_OK;
}

void cMainWindow::CreateRenderBuffer()
{
	for (int i = 0; i < g_FrameBuuferNum; i++)
	{
		CheckHR(cDrawCommand::GetSwapChain()->GetBuffer(i, IID_PPV_ARGS(m_D3DBuffer[i].ReleaseAndGetAddressOf())));
		m_D3DBuffer[i]->SetName(L"SwapChain_Buffer");
	}

	CreateRenderBufferDescriptor();

	// レンダーターゲットビューの作成
	auto rtvStep = cDirectX12::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	for (auto i = 0u; i < g_FrameBuuferNum; i++)
	{
		auto d = m_DescHeapRtv->GetCPUDescriptorHandleForHeapStart();
		d.ptr += i * rtvStep;		// バッファ格納位置アドレスをずらしている
		cDirectX12::GetDevice()->CreateRenderTargetView(m_D3DBuffer[i].Get(), nullptr, d);
	}

	// デプスステンシルビューの作成
	auto resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R32_TYPELESS, g_WindowSizeX, g_WindowSizeY, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL,
		D3D12_TEXTURE_LAYOUT_UNKNOWN, 0);
	D3D12_CLEAR_VALUE dsvClearValue;
	dsvClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	dsvClearValue.DepthStencil.Depth = 1.0f;
	dsvClearValue.DepthStencil.Stencil = 0;
	CheckHR(cDirectX12::GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), // No need to read/write by CPU
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&dsvClearValue,
		IID_PPV_ARGS(mDsvResource.ReleaseAndGetAddressOf())));
	mDsvResource->SetName(L"DepthTexture");

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.Texture2D.MipSlice = 0;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	cDirectX12::GetDevice()->CreateDepthStencilView(mDsvResource.Get(), &dsvDesc, m_DescHeapDsv->GetCPUDescriptorHandleForHeapStart());
}

void cMainWindow::CreateRenderBufferDescriptor()
{
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.NumDescriptors = g_FrameBuuferNum;
		desc.NodeMask = 0;
		CheckHR(cDirectX12::GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(m_DescHeapRtv.ReleaseAndGetAddressOf())));

		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		desc.NumDescriptors = g_FrameBuuferNum;
		desc.NodeMask = 0;
		CheckHR(cDirectX12::GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(m_DescHeapDsv.ReleaseAndGetAddressOf())));
	}
}
