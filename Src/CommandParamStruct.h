#pragma once
#include "DirectX12.h"

// �R�}���h�n�̍쐬�ɕK�v�ȃp�����[�^��ێ�����
struct CommandParamStruct
{
	ID3D12Device * dev;
	Microsoft::WRL::ComPtr<IDXGIFactory2> dxgi;
	HWND hwnd;
	UINT width;
	UINT heigit;
	UINT bufferNum;
};