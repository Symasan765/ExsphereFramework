/*=================================================
//								FILE :	ImGUIManager.h
//		�t�@�C������ :
//		�S�̂�ImGUI���Ǘ�����}�l�[�W���[�N���X
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include "DirectX12.h"

class cImGUIManager {
public:
	cImGUIManager();
	~cImGUIManager();
private:
	void Create();
	void CreateDescriptorHeap();
	void CreateCommandList();
	void DrawCommand();
	void Destroy();

	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> g_pd3dSrvDescHeap;
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> g_pd3dCommandList;
};