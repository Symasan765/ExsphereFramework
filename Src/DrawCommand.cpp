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

// ���̊֐����Ăяo�����Ƃŏ��ɏ��������s��
void cDrawCommand::Init()
{
	CreateCommandAllocators();
	CreateCommandQueue();
	CreateSwapChain();
	CreateCommandLists();
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

void cDrawCommand::Destroy()
{
	CloseHandle(m_FenceEveneHandle);
}
