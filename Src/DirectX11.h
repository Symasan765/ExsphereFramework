/*=================================================
//								FILE : DirectX11.h
//		ファイル説明 :
//		DirectXの機能の管理や処理を行うシステムクラス。
//		他のクラスへデバイスなどの提供を行う
//									松本 雄之介
=================================================*/
#pragma once
#include <windows.h>
#include <mmsystem.h>
#include <crtdbg.h>
#include <d3dx11.h>
#include <DirectXMath.h>
#include <dxerr.h>

// TODO 必ずinit関数とdestroy関数を作ってMainSystemで処理させる
class cDirectX11 {
public:
	friend class cMainSystem;
	cDirectX11() = default;
	~cDirectX11() = default;

	// 取得関数群
	inline static ID3D11Device* GetDevice() { return m_pD3DDevice; };
	inline static ID3D11DeviceContext* GetContext() { return m_pImmediateContext; };
	inline static IDXGISwapChain* GetSwapChain() { return m_pSwapChain; };
	inline static ID3D11RenderTargetView* GetRenderTargetView() { return m_pRenderTargetView; };
private:
	HRESULT InitDirect3D(HWND _hwnd);
	HRESULT InitBackBuffer();

	void Destroy();

	static IDXGISwapChain* m_pSwapChain;
	static ID3D11Device* m_pD3DDevice;
	static ID3D11DeviceContext* m_pImmediateContext;
	static ID3D11RenderTargetView* m_pRenderTargetView;	//ターゲットビュー
};