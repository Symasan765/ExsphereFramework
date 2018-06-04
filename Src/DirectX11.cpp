#include "DirectX11.h"
#include "Utility.h"


// static�ϐ��̏�����
IDXGISwapChain * cDirectX11::m_pSwapChain = nullptr;
ID3D11Device* cDirectX11::m_pD3DDevice = nullptr;
ID3D11DeviceContext* cDirectX11::m_pImmediateContext = nullptr;
ID3D11RenderTargetView* cDirectX11::m_pRenderTargetView = nullptr;


namespace {
	D3D_FEATURE_LEVEL g_pFeatureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
	UINT              g_FeatureLevels = 3;   // �z��̗v�f��
	D3D_FEATURE_LEVEL g_FeatureLevelsSupported; // �f�o�C�X�쐬���ɕԂ����@�\���x��
}

HRESULT cDirectX11::InitDirect3D(HWND _hwnd)
{
	// �E�C���h�E�̃N���C�A���g �G���A
	RECT rc;
	GetClientRect(_hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	// �f�o�C�X�ƃX���b�v �`�F�C���̍쐬
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));	// �\���̂̒l��������
	sd.BufferCount = 3;		// �o�b�N �o�b�t�@��
	sd.BufferDesc.Width = width;	// �o�b�N �o�b�t�@�̕�
	sd.BufferDesc.Height = height;	// �o�b�N �o�b�t�@�̍���
									//sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // �t�H�[�}�b�g
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // �t�H�[�}�b�g
	sd.BufferDesc.RefreshRate.Numerator = 60;  // ���t���b�V�� ���[�g(���q)
	sd.BufferDesc.RefreshRate.Denominator = 1; // ���t���b�V�� ���[�g(����)
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;	// �X�L�������C��
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;			// �X�P�[�����O
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // �o�b�N �o�b�t�@�̎g�p�@
	sd.OutputWindow = _hwnd;	// �֘A�t����E�C���h�E
	sd.SampleDesc.Count = 1;		// �}���` �T���v���̐�
	sd.SampleDesc.Quality = 0;		// �}���` �T���v���̃N�I���e�B
	sd.Windowed = TRUE;				// �E�C���h�E ���[�h
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // ���[�h�����؂�ւ�

#if defined(DEBUG) || defined(_DEBUG)
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
	UINT createDeviceFlags = 0;
#endif

	// �n�[�h�E�F�A�E�f�o�C�X���쐬
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags,
		g_pFeatureLevels, g_FeatureLevels, D3D11_SDK_VERSION, &sd,
		&m_pSwapChain, &m_pD3DDevice, &g_FeatureLevelsSupported,
		&m_pImmediateContext);
	if (FAILED(hr)) {
		// WARP�f�o�C�X���쐬
		hr = D3D11CreateDeviceAndSwapChain(
			NULL, D3D_DRIVER_TYPE_WARP, NULL, createDeviceFlags,
			g_pFeatureLevels, g_FeatureLevels, D3D11_SDK_VERSION, &sd,
			&m_pSwapChain, &m_pD3DDevice, &g_FeatureLevelsSupported,
			&m_pImmediateContext);
		if (FAILED(hr)) {
			// ���t�@�����X�E�f�o�C�X���쐬
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
	// �o�b�N �o�b�t�@�̏�����
	hr = InitBackBuffer();
	if (FAILED(hr))
		return DXTRACE_ERR("InitDirect3D InitBackBuffer", hr);

	return hr;
}

HRESULT cDirectX11::InitBackBuffer()
{
	HRESULT hr;

	// �X���b�v�E�`�F�C������ŏ��̃o�b�N�E�o�b�t�@���擾����
	ID3D11Texture2D *pBackBuffer;  // �o�b�t�@�̃A�N�Z�X�Ɏg���C���^�[�t�F�C�X
	hr = m_pSwapChain->GetBuffer(
		0,                         // �o�b�N�E�o�b�t�@�̔ԍ�
		__uuidof(ID3D11Texture2D), // �o�b�t�@�ɃA�N�Z�X����C���^�[�t�F�C�X
		(LPVOID*)&pBackBuffer);    // �o�b�t�@���󂯎��ϐ�
	if (FAILED(hr))
		return DXTRACE_ERR("InitBackBuffer g_pSwapChain->GetBuffer", hr);  // ���s

																		   // �o�b�N�E�o�b�t�@�̏��
	D3D11_TEXTURE2D_DESC descBackBuffer;
	pBackBuffer->GetDesc(&descBackBuffer);

	// �o�b�N�E�o�b�t�@�̕`��^�[�Q�b�g�E�r���[�����
	hr = m_pD3DDevice->CreateRenderTargetView(
		pBackBuffer,           // �r���[�ŃA�N�Z�X���郊�\�[�X
		NULL,                  // �`��^�[�Q�b�g�E�r���[�̒�`
		&m_pRenderTargetView); // �`��^�[�Q�b�g�E�r���[���󂯎��ϐ�
	SAFE_RELEASE(pBackBuffer);  // �ȍ~�A�o�b�N�E�o�b�t�@�͒��ڎg��Ȃ��̂ŉ��
	if (FAILED(hr))
		return DXTRACE_ERR("InitBackBuffer g_pD3DDevice->CreateRenderTargetView", hr);  // ���s

	return S_OK;
}