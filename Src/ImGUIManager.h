/*=================================================
//								FILE :	ImGUIManager.h
//		ファイル説明 :
//		全体のImGUIを管理するマネージャークラス
//
//									松本 雄之介
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