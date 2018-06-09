/*=================================================
//								FILE : DrawCommand.h
//		�t�@�C������ :
//		DirectX12�̕`��R�}���h�V�X�e������̋@�\����������B
//		�V�X�e���I�ȃ}���`�X���b�h������
//									���{ �Y�V��
=================================================*/
#pragma once
#include "DirectX12.h"

namespace DrawParam{
	constexpr unsigned g_ThreadNum = 4;		// ���񏈗��X���b�h��
	constexpr unsigned g_MaxFrameLatency = 2;	// �����t���[����
}

class cDrawCommand {
public:
	friend class cMainSystem;
	cDrawCommand() = default;
	~cDrawCommand() = default;

	inline static Microsoft::WRL::ComPtr<IDXGISwapChain1> GetSwapChain() { return m_SwapChain; };
private:
	void Init();		// �������p�֐��B�������Ăׂ΁��̊֐��Q�����ɌĂяo�����
	// ==================================//
	void CreateCommandAllocators();
	void CreateCommandQueue();
	void CreateSwapChain();
	void CreateCommandLists();
	void CreateFence();
	// ==================================//
	void Destroy();

	static Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_CommandAllocator[DrawParam::g_MaxFrameLatency][DrawParam::g_ThreadNum];
	static Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CommandQueue;
	static Microsoft::WRL::ComPtr<IDXGISwapChain1> m_SwapChain;
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandList[DrawParam::g_ThreadNum];
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandListProl;		// �`��O�����p�R�}���h���X�g
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandListEpir;		// �`��㏈���p�̃R�}���h���X�g
	static Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;
	static HANDLE m_FenceEveneHandle;
};