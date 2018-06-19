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
	friend class cMainSystem;
	cImGUIManager() = default;
	~cImGUIManager() = default;

	static void DrawCommand(Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator, Microsoft::WRL::ComPtr<ID3D12Resource> RenderTargetResource, D3D12_CPU_DESCRIPTOR_HANDLE& descHandleRtv);
	inline static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList() { return g_pd3dCommandList; };
private:
	void Create();
	void CreateDescriptorHeap();
	void CreateCommandList();
	void Destroy();
	void NewFrame();

	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> g_pd3dSrvDescHeap;
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> g_pd3dCommandList;
};