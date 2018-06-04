#include "DirectX11.h"
#include "Utility.h"


// static変数の初期化
IDXGISwapChain * cDirectX11::m_pSwapChain = nullptr;
ID3D11Device* cDirectX11::m_pD3DDevice = nullptr;
ID3D11DeviceContext* cDirectX11::m_pImmediateContext = nullptr;
ID3D11RenderTargetView* cDirectX11::m_pRenderTargetView = nullptr;


namespace {
	D3D_FEATURE_LEVEL g_pFeatureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
	UINT              g_FeatureLevels = 3;   // 配列の要素数
	D3D_FEATURE_LEVEL g_FeatureLevelsSupported; // デバイス作成時に返される機能レベル
}

HRESULT cDirectX11::InitDirect3D(HWND _hwnd)
{
	// ウインドウのクライアント エリア
	RECT rc;
	GetClientRect(_hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	// デバイスとスワップ チェインの作成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));	// 構造体の値を初期化
	sd.BufferCount = 3;		// バック バッファ数
	sd.BufferDesc.Width = width;	// バック バッファの幅
	sd.BufferDesc.Height = height;	// バック バッファの高さ
									//sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // フォーマット
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // フォーマット
	sd.BufferDesc.RefreshRate.Numerator = 60;  // リフレッシュ レート(分子)
	sd.BufferDesc.RefreshRate.Denominator = 1; // リフレッシュ レート(分母)
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;	// スキャンライン
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;			// スケーリング
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // バック バッファの使用法
	sd.OutputWindow = _hwnd;	// 関連付けるウインドウ
	sd.SampleDesc.Count = 1;		// マルチ サンプルの数
	sd.SampleDesc.Quality = 0;		// マルチ サンプルのクオリティ
	sd.Windowed = TRUE;				// ウインドウ モード
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // モード自動切り替え

#if defined(DEBUG) || defined(_DEBUG)
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
	UINT createDeviceFlags = 0;
#endif

	// ハードウェア・デバイスを作成
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags,
		g_pFeatureLevels, g_FeatureLevels, D3D11_SDK_VERSION, &sd,
		&m_pSwapChain, &m_pD3DDevice, &g_FeatureLevelsSupported,
		&m_pImmediateContext);
	if (FAILED(hr)) {
		// WARPデバイスを作成
		hr = D3D11CreateDeviceAndSwapChain(
			NULL, D3D_DRIVER_TYPE_WARP, NULL, createDeviceFlags,
			g_pFeatureLevels, g_FeatureLevels, D3D11_SDK_VERSION, &sd,
			&m_pSwapChain, &m_pD3DDevice, &g_FeatureLevelsSupported,
			&m_pImmediateContext);
		if (FAILED(hr)) {
			// リファレンス・デバイスを作成
			hr = D3D11CreateDeviceAndSwapChain(
				NULL, D3D_DRIVER_TYPE_REFERENCE, NULL, createDeviceFlags,
				g_pFeatureLevels, g_FeatureLevels, D3D11_SDK_VERSION, &sd,
				&m_pSwapChain, &m_pD3DDevice, &g_FeatureLevelsSupported,
				&m_pImmediateContext);
			if (FAILED(hr)) {
				return DXTRACE_ERR("InitDirect3D D3D11CreateDeviceAndSwapChain", hr);
			}
		}
	}

	// **********************************************************
	// バック バッファの初期化
	hr = InitBackBuffer();
	if (FAILED(hr))
		return DXTRACE_ERR("InitDirect3D InitBackBuffer", hr);

	return hr;
}

HRESULT cDirectX11::InitBackBuffer()
{
	HRESULT hr;

	// スワップ・チェインから最初のバック・バッファを取得する
	ID3D11Texture2D *pBackBuffer;  // バッファのアクセスに使うインターフェイス
	hr = m_pSwapChain->GetBuffer(
		0,                         // バック・バッファの番号
		__uuidof(ID3D11Texture2D), // バッファにアクセスするインターフェイス
		(LPVOID*)&pBackBuffer);    // バッファを受け取る変数
	if (FAILED(hr))
		return DXTRACE_ERR("InitBackBuffer g_pSwapChain->GetBuffer", hr);  // 失敗

																		   // バック・バッファの情報
	D3D11_TEXTURE2D_DESC descBackBuffer;
	pBackBuffer->GetDesc(&descBackBuffer);

	// バック・バッファの描画ターゲット・ビューを作る
	hr = m_pD3DDevice->CreateRenderTargetView(
		pBackBuffer,           // ビューでアクセスするリソース
		NULL,                  // 描画ターゲット・ビューの定義
		&m_pRenderTargetView); // 描画ターゲット・ビューを受け取る変数
	SAFE_RELEASE(pBackBuffer);  // 以降、バック・バッファは直接使わないので解放
	if (FAILED(hr))
		return DXTRACE_ERR("InitBackBuffer g_pD3DDevice->CreateRenderTargetView", hr);  // 失敗

	return S_OK;
}