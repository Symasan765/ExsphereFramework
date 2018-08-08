#include "CommandManager.h"
#include "Utility.h"
#include "DescHandleStep.h"

void cCommandManager::Create(ID3D12Device * dev, Microsoft::WRL::ComPtr<IDXGIFactory2> dxgi, HWND hwnd, const UINT width, const UINT heigit, 
	const UINT bufferNum, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtv, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsv)
{
	m_Param.dev = dev;
	m_Param.dxgi = dxgi;
	m_Param.hwnd = hwnd;
	m_Param.width = width;
	m_Param.heigit = heigit;
	m_Param.bufferNum = bufferNum;

	m_NowTarget.HeapDsv = dsv;
	m_NowTarget.HeapRtv = rtv;

	Init();
}

void cCommandManager::CommandBuild(Microsoft::WRL::ComPtr<ID3D12Resource>nowBuffer,unsigned totalFrame, unsigned frameIndex)
{
	// �`��t���[���X�V
	FrameUpdate(nowBuffer,frameIndex);

	// �A�v���P�[�V�����J�n����ɃR�}���h�����܂�؂�܂ł̊ԑ҂�����֐�
	WaitForFence(totalFrame, frameIndex);

	// �o�b�t�@�N���A�Ȃǂ̑O��������
	m_Command->DrawBegin(m_NowTarget, frameIndex);
}

void cCommandManager::CommandQueueExe(unsigned totalFrame)
{
	ExePrologue();

	// TODO �{�������ǉ�����

	ExeEpilogue();
	

	m_Fence->Signal(m_Queue->GetQueue(), totalFrame);

	m_SwapChain->Present();
}

void cCommandManager::Init()
{
	m_Command.reset(new cMainCommand);
	m_Queue.reset(new cMainCommandQueue);
	m_SwapChain.reset(new cMainSwapChain);
	m_Fence.reset(new cFenceObj);

	m_Command->Create(m_Param.dev);
	m_Queue->Create(m_Param.dev);
	m_SwapChain->Create(m_Queue->GetQueue(), m_Param.dxgi, m_Param.hwnd, m_Param.width, m_Param.heigit, m_Param.bufferNum);

	// �q�[�v�T�C�Y���O���[�o���Ƃ��Ď擾���Ă���
	cDescHandleStep descStep;
	descStep.Init(m_Param.dev);
}

void cCommandManager::FrameUpdate(Microsoft::WRL::ComPtr<ID3D12Resource> nowBuffer, unsigned frameIndex)
{
	m_NowTarget.buffer = nowBuffer;
	m_NowTarget.descHandleDsv = m_NowTarget.HeapDsv->GetCPUDescriptorHandleForHeapStart();
	m_NowTarget.descHandleRtv = m_NowTarget.HeapRtv->GetCPUDescriptorHandleForHeapStart();

	m_NowTarget.descHandleRtv.ptr += frameIndex * cDescHandleStep::GetSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);		// �擪�A�h���X += (No * �A�h���X�T�C�Y)
}

void cCommandManager::WaitForFence(unsigned totalFrame, unsigned idx)
{
	// �R�}���h�����܂�؂�̂�҂��Ă���
	if (totalFrame > m_Param.bufferNum)
	{
		m_Fence->WaitForPreviousFrame(totalFrame - m_Param.bufferNum);

		CheckHR(m_Command->GetSelectAlloc(idx, 0).Get()->Reset());
	}
}

void cCommandManager::ExePrologue()
{
	auto const list = m_Command->GetPrologueList().Get();

	m_Queue->Exe(list, 1);
}

void cCommandManager::ExeEpilogue()
{
	auto const list = m_Command->GetEpilogueList().Get();

	m_Queue->Exe(list, 1);
}
