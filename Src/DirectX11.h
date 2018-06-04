/*=================================================
//								FILE : DirectX11.h
//		�t�@�C������ :
//		DirectX�̋@�\�̊Ǘ��⏈�����s���V�X�e���N���X�B
//		���̃N���X�փf�o�C�X�Ȃǂ̒񋟂��s��
//									���{ �Y�V��
=================================================*/
#pragma once
#include <windows.h>
#include <mmsystem.h>
#include <crtdbg.h>
#include <d3dx11.h>
#include <DirectXMath.h>
#include <dxerr.h>

// TODO �K��init�֐���destroy�֐��������MainSystem�ŏ���������
class cDirectX11 {
public:
	friend class cMainSystem;
	cDirectX11() = default;
	~cDirectX11() = default;

	// �擾�֐��Q
	inline ID3D11Device* GetDevice() { return m_pD3DDevice; };
	inline ID3D11DeviceContext* GetContext() { return m_pImmediateContext; };
private:
	HRESULT InitDirect3D(HWND _hwnd);
	HRESULT InitBackBuffer();

	static IDXGISwapChain* m_pSwapChain;
	static ID3D11Device* m_pD3DDevice;
	static ID3D11DeviceContext* m_pImmediateContext;
	static ID3D11RenderTargetView* m_pRenderTargetView;	//�^�[�Q�b�g�r���[
};