#pragma once
#include "DirectX12.h"

// コマンド系の作成に必要なパラメータを保持する
struct CommandParamStruct
{
	ID3D12Device * dev;
	Microsoft::WRL::ComPtr<IDXGIFactory2> dxgi;
	HWND hwnd;
	UINT width;
	UINT heigit;
	UINT bufferNum;
};