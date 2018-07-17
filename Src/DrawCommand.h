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
class cRootSignatureTest;

class cDrawCommand {
public:
	friend class cMainSystem;
	cDrawCommand() = default;
	~cDrawCommand() = default;

	inline static Microsoft::WRL::ComPtr<IDXGISwapChain1> GetSwapChain() { return m_SwapChain; };
	inline static ID3D12CommandAllocator* GetStartAllocator() { return m_CommandAllocator[0][0].Get(); };
	inline static ID3D12GraphicsCommandList* GetGeneralCommandList() { return m_CommandListEpir.Get(); };
	inline static ID3D12CommandQueue* GetCommandQueue() { return m_CommandQueue.Get(); };
	static unsigned GetFrameIndex();
private:
	void FrameUpdate();	// �V�����t���[���̍ŏ��ɌĂяo������
	void Init();		// �������p�֐��B�������Ăׂ΁��̊֐��Q�����ɌĂяo�����
	// ==================================//
	void CreateCommandAllocators();
	void CreateCommandQueue();
	void CreateSwapChain();
	void CreateCommandLists();
	void CreateFence();
	// ==================================//

	// �`��n�֐�
	void CommandListBuild();		// �`�施�߂��R�}���h���X�g�ɓ���Ă���
	void CommandQueueExe();		// �R�}���h���X�g�̖��߂����s����
	void PreDrawingProcess(ID3D12GraphicsCommandList*& cmdListProl,const int& cmdIndex);
	void SetResourceBarrier(ID3D12GraphicsCommandList* commandList, ID3D12Resource* res, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

	void Destroy();

	static Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_CommandAllocator[DrawParam::g_MaxFrameLatency][DrawParam::g_ThreadNum];
	static Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CommandQueue;
	static Microsoft::WRL::ComPtr<IDXGISwapChain1> m_SwapChain;
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandList[DrawParam::g_ThreadNum];
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandListProl;		// �`��O�����p�R�}���h���X�g
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandListEpir;		// �`��㏈���p�̃R�}���h���X�g(��Ƀe�N�X�`�����\�[�X�쐬�ȂǎG�p��S��)
	static Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;
	static D3D12_CPU_DESCRIPTOR_HANDLE m_DescHandleRtv;		// ���݂̃t���[���Ŏg�p����RTV�n���h����ێ�
	static HANDLE m_FenceEveneHandle;
	static unsigned m_DescHandleRtvStep;

	// TODO �ȉ��̓e�X�g���Ȃ̂ō폜���邱��
	cRootSignatureTest* m_RootSig;
	static unsigned m_FrameCount;
};