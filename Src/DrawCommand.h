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
	constexpr int g_ThreadNum = 4;		// ���񏈗��X���b�h��
	constexpr const unsigned MaxFrameLatency = 2;	// �����t���[����
}

class cDrawCommand {
public:
	cDrawCommand();
	~cDrawCommand();
private:
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_CommandAllocator[DrawParam::MaxFrameLatency][DrawParam::g_ThreadNum];
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CommandQueue;
};