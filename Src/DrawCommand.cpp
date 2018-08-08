/*=================================================
//								FILE :
//		�t�@�C������ :
//		�`��ŗ��p����R�}���h�A���P�[�^�[��R�}���h���X�g�Ȃǂ�ێ����A
//		�K�v�ɉ����ď������s���N���X
//									���{ �Y�V��
=================================================*/
#include "DrawCommand.h"
#include "MainWindow.h"
#include "Utility.h"
#include "ImGUIManager.h"
#include <stdexcept>
#include "RootSignatureTest.h"		// TODO �b�菈���B��X����
// TODO ���݂̃o�b�t�@�����炩���ߎ擾�ł���悤�ɂ��Ă�������

using namespace DrawParam;

// �e��static�ϐ��錾
Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cDrawCommand::m_CommandAllocator[DrawParam::g_MaxFrameLatency][DrawParam::g_ThreadNum];
Microsoft::WRL::ComPtr<ID3D12CommandQueue> cDrawCommand::m_CommandQueue;
Microsoft::WRL::ComPtr<IDXGISwapChain1> cDrawCommand::m_SwapChain;
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cDrawCommand::m_CommandList[DrawParam::g_ThreadNum];
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cDrawCommand::m_CommandListProl;
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cDrawCommand::m_CommandListEpir;
Microsoft::WRL::ComPtr<ID3D12Fence> cDrawCommand::m_Fence;
HANDLE cDrawCommand::m_FenceEveneHandle = 0;
D3D12_CPU_DESCRIPTOR_HANDLE cDrawCommand::m_DescHandleRtv;
unsigned cDrawCommand::m_FrameCount = 0;
unsigned cDrawCommand::m_DescHandleRtvStep;

namespace {
	// �R�}���h�̍쐬�Ǝ��s�̊Ԃŏ�������A�h���X��ێ����Ă����ϐ�
	ID3D12CommandQueue* g_CommandQueueTmp;
	ID3D12CommandList* g_CmdListsProlTmp;
	ID3D12CommandList* g_CmdListsEpirTmp;
}

unsigned cDrawCommand::GetFrameIndex()
{
	return m_FrameCount % g_MaxFrameLatency;
}

void cDrawCommand::FrameUpdate()
{
	// �t���[���J�E���g��i�߂�
	m_FrameCount++;
	// �t���[���o�b�t�@�����X�V����
	m_DescHandleRtv = cMainWindow::GetDescHeapRtv()->GetCPUDescriptorHandleForHeapStart();		// �擪�A�h���X���擾
	m_DescHandleRtv.ptr += ((m_FrameCount - 1) % WindowOptions::g_FrameBuuferNum) * m_DescHandleRtvStep;		// �擪�A�h���X += (No * �A�h���X�T�C�Y)
}

// ���̊֐����Ăяo�����Ƃŏ��ɏ��������s��
void cDrawCommand::Init()
{
	CreateCommandAllocators();
	CreateCommandQueue();
	CreateSwapChain();
	CreateCommandLists();
	CreateFence();
	m_DescHandleRtvStep = cDirectX12::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);	// RTV�̃|�C���^�T�C�Y���擾���Ă���

	m_RootSig = new cRootSignatureTest();
	m_RootSig->Init();
}

void cDrawCommand::CreateCommandAllocators()
{
	for (auto& LatencyArray : m_CommandAllocator)
	{
		for (auto& ThreadArray : LatencyArray)
		{
			CheckHR(cDirectX12::GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(ThreadArray.ReleaseAndGetAddressOf())));
		}
	}
}

void cDrawCommand::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	CheckHR(cDirectX12::GetDevice()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(m_CommandQueue.ReleaseAndGetAddressOf())));
}

void cDrawCommand::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Width = WindowOptions::g_WindowSizeX;
	scDesc.Height = WindowOptions::g_WindowSizeY;
	scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.SampleDesc.Count = 1;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = WindowOptions::g_FrameBuuferNum;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	CheckHR(cDirectX12::GetDxgiFactory()->CreateSwapChainForHwnd(m_CommandQueue.Get(), cMainWindow::GetHWND(), &scDesc, nullptr, nullptr, m_SwapChain.ReleaseAndGetAddressOf()));
}

void cDrawCommand::CreateCommandLists()
{
	// �Q�[�����̃I�u�W�F�N�g�ȂǁA���C���ƂȂ�R�}���h���X�g���쐬����B
	{
		for (int i = 0; i < g_ThreadNum; i++)
		{
			CheckHR(cDirectX12::GetDevice()->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				m_CommandAllocator[0][0].Get(),
				nullptr,
				IID_PPV_ARGS(m_CommandList[i].ReleaseAndGetAddressOf())));

			m_CommandList[i]->Close();		// �쐬����͋L�^��ԂɂȂ��Ă���̂ŕ��Ă���
		}
	}

	// ���Ƀo�b�t�@�̃N���A�ȂǕ`��O�����p�̃R�}���h���X�g���쐬����
	{
		CheckHR(cDirectX12::GetDevice()->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_CommandAllocator[0][0].Get(),
			nullptr,
			IID_PPV_ARGS(m_CommandListProl.ReleaseAndGetAddressOf())));
		m_CommandListProl->Close();		// ��Ɠ��������Ă���
	}

	// �Ō�Ɍ㏈���p�̃R�}���h���X�g�̍쐬
	{
		CheckHR(cDirectX12::GetDevice()->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_CommandAllocator[0][0].Get(),
			nullptr,
			IID_PPV_ARGS(m_CommandListEpir.ReleaseAndGetAddressOf())));
		m_CommandListEpir->Close();
	}
}

void cDrawCommand::CreateFence()
{
	CheckHR(cDirectX12::GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_Fence.ReleaseAndGetAddressOf())));

	m_FenceEveneHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void cDrawCommand::CommandListBuild()
{
	int cmdIndex = GetFrameIndex();	// ���Ԃ̃R�}���h�A���P�[�^�[�𗘗p���邩�H
	auto* cmdQueue = m_CommandQueue.Get();
	auto* cmdListProl = m_CommandListProl.Get();
	auto* cmdListEpir = m_CommandListEpir.Get();

	// �R�}���h�����܂�؂�̂�҂��Ă���
	if (m_FrameCount > g_MaxFrameLatency)
	{
		m_Fence->SetEventOnCompletion(m_FrameCount - g_MaxFrameLatency, m_FenceEveneHandle);
		DWORD wait = WaitForSingleObject(m_FenceEveneHandle, 10000);	//�V�O�i����ԂɂȂ�̂������ő҂��Ă���Ă�����ۂ�

		//�V�O�i����ԂɂȂ�Ȃ�����
		if (wait != WAIT_OBJECT_0)
			throw std::runtime_error("Failed WaitForSingleObject().");

		CheckHR(m_CommandAllocator[cmdIndex][0]->Reset());
	}

	// �`��̑O����
	PreDrawingProcess(cmdListProl, cmdIndex);

	// TODO ��������������
	auto* cmdList = m_CommandList[0].Get();
	CheckHR(cmdList->Reset(m_CommandAllocator[cmdIndex][0].Get(), nullptr));
	cmdList->OMSetRenderTargets(1, &m_DescHandleRtv, true, &cMainWindow::GetDescHeapDsv()->GetCPUDescriptorHandleForHeapStart());
	m_RootSig->Draw(cmdList);
	// Fix draw command
	CheckHR(cmdList->Close());

	// ImGUI �̏���
	auto DescriptorHandleForHeap = cMainWindow::GetDescHeapRtv()->GetCPUDescriptorHandleForHeapStart();
	SIZE_T rtvDescriptorSize = cDirectX12::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	DescriptorHandleForHeap.ptr += rtvDescriptorSize * (m_FrameCount % WindowOptions::g_FrameBuuferNum);

	// �㏈��
	CheckHR(cmdListEpir->Reset(m_CommandAllocator[cmdIndex][0].Get(), nullptr));
	SetResourceBarrier(cmdListEpir, cMainWindow::GetBuffer((m_FrameCount - 1) % WindowOptions::g_FrameBuuferNum).Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	CheckHR(cmdListEpir->Close());

	// �R�}���h���s���Ɏg�p����e�A�h���X���O���[�o���ŕێ����Ă���
	g_CmdListsProlTmp = cmdListProl;
	g_CmdListsEpirTmp = cmdListEpir;
	g_CommandQueueTmp = cmdQueue;
}

void cDrawCommand::CommandQueueExe()
{
	// Exec
	ID3D12CommandList* const cmdListsProl = g_CmdListsProlTmp;
	ID3D12CommandList* const cmdListsEpir = g_CmdListsEpirTmp;
	auto* cmdQueue = g_CommandQueueTmp;

	// �O����
	cmdQueue->ExecuteCommandLists(1, &cmdListsProl);
	// ���C������
	ID3D12CommandList* data = m_CommandList[0].Get();
	cmdQueue->ExecuteCommandLists(1, &data);
	// �㏈��
	cmdQueue->ExecuteCommandLists(1, &cmdListsEpir);
	// ImGUI����


	CheckHR(cmdQueue->Signal(m_Fence.Get(), m_FrameCount));

	// Present
	CheckHR(m_SwapChain->Present(1, 0));
}

void cDrawCommand::PreDrawingProcess(ID3D12GraphicsCommandList*& cmdListProl, const int& cmdIndex)
{
	// �O�����X�^�[�g
	CheckHR(cmdListProl->Reset(m_CommandAllocator[cmdIndex][0].Get(), nullptr));

																											// ���݂̕`��o�b�t�@�̎擾
	ID3D12Resource* d3dBuffer = cMainWindow::GetBuffer((m_FrameCount - 1) % WindowOptions::g_FrameBuuferNum).Get();

	// �f�v�X�X�e���V���̎擾
	auto descHandleDsv = cMainWindow::GetDescHeapDsv()->GetCPUDescriptorHandleForHeapStart();

	// �o���A�������_�[�^�[�Q�b�g�ɐݒ肷��
	SetResourceBarrier(cmdListProl, d3dBuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	// �f�v�X�X�e���V�����N���A
	cmdListProl->ClearDepthStencilView(descHandleDsv, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// �����_�[�^�[�Q�b�g���N���A
	{
		const float ClearColor[4] = { 0.1f, 0.2f, 0.6f, 1.0f };
		cmdListProl->ClearRenderTargetView(m_DescHandleRtv, ClearColor, 0, nullptr);
	}

	// �`��O�����̏I��
	CheckHR(cmdListProl->Close());
}

void cDrawCommand::SetResourceBarrier(ID3D12GraphicsCommandList* commandList,	// �R�}���h���X�g
	ID3D12Resource* res,		// ���݂̃o�b�t�@
	D3D12_RESOURCE_STATES before,		// �ݒ肵�Ȃ����ۂɈȑO�̏�Ԃ������K�v�����邽��
	D3D12_RESOURCE_STATES after)
{
	D3D12_RESOURCE_BARRIER desc = {};
	desc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	desc.Transition.pResource = res;
	desc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	desc.Transition.StateBefore = before;
	desc.Transition.StateAfter = after;
	desc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	commandList->ResourceBarrier(1, &desc);
}

void cDrawCommand::Destroy()
{
	CloseHandle(m_FenceEveneHandle);
}
