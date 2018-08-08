/*=================================================
//								FILE : MainWindow.h
//		�t�@�C������ :
//		�`�悳��郁�C���E�B���h�E�̏������s���N���X�B
//		���C���E�B���h�E�̓T�C�Y�ύX�s�A
//		�t���[�����[�g60fps�Ƃ��ď������s��.
//									���{ �Y�V��
=================================================*/
#pragma once
#include "WindowParam.h"
#include "DirectX12.h"
#include "DefaultRenderTarget.h"

class cMainWindow {
public:
	cMainWindow(HINSTANCE _hInst);
	~cMainWindow();

	static inline HWND GetHWND() { return m_hWindow; };
	static inline HINSTANCE GetHInstance() { return m_hInstance; };
	static inline Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescHeapRtv() { return m_RenderTarget.GetDescHeapRtv(); };
	static inline Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescHeapDsv() { return m_RenderTarget.GetDescHeapDsv(); };
	static Microsoft::WRL::ComPtr<ID3D12Resource> GetBuffer(const int no);

	HRESULT CreateMainWindow();
	void CreateRenderBuffer(Microsoft::WRL::ComPtr<IDXGISwapChain1> SwapChain);
private:

	static HWND m_hWindow;
	static HINSTANCE m_hInstance;
	static cDefaultRenderTarget m_RenderTarget;
};