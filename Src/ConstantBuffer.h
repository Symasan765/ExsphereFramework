/*=================================================
//								FILE : ConstantBuffer.h
//		�t�@�C������ :
//		�萔�o�b�t�@���̏������L�q�B
//		�v���O�������ŋ쓮����o�b�t�@�����̒萔�o�b�t�@��
//		�^�w�肷�邱�Ƃō쐬�o����悤�ɂ���
//									���{ �Y�V��
=================================================*/
#pragma once
#include <wrl/client.h>
#include "DirectX12.h"
#include <d3dx12.h>
#include "DrawParam.h"
#include "Utility.h"
#include "FrameCnt.h"

/// <summary>
/// �萔�o�b�t�@����舵���e���v���[�g�N���X
/// </summary>
template <typename T> class cConstBuf {
public:
	cConstBuf();				//�R���X�g���N�^�Œ萔�o�b�t�@���
	~cConstBuf();				//�f�X�g���N�^�ŉ��
	void Upload();			// �\���̂̏����A�b�v���[�h����

	T data;				//�����Ƀf�[�^���i�[����
	ID3D12DescriptorHeap* GetDescriptorHeap();
private:
	void CreateDescriptorHeapCB();
	void CreateConstantBuffer();
	// TODO �萔�o�b�t�@�{�̂̕ێ�
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DescHeapCbvSrvUav[DrawParam::g_MaxFrameLatency];
	Microsoft::WRL::ComPtr<ID3D12Resource> m_ConstantBuffer;
	unsigned int m_BufferSize;
	void* mCBUploadPtr;
};

template<typename T>
inline cConstBuf<T>::cConstBuf()
{
	m_BufferSize = 0;
	mCBUploadPtr = nullptr;

	CreateDescriptorHeapCB();
	CreateConstantBuffer();
}

template<typename T>
inline cConstBuf<T>::~cConstBuf()
{
	m_ConstantBuffer->Unmap(0, nullptr);
}

template<typename T>
inline void cConstBuf<T>::Upload()
{
	char* ptr = reinterpret_cast<char*>(mCBUploadPtr) + m_BufferSize * cFrameCnt::GetNowIndex();
	memcpy_s(ptr, sizeof(T), &data, sizeof(T));
}

template<typename T>
inline ID3D12DescriptorHeap* cConstBuf<T>::GetDescriptorHeap()
{
	return m_DescHeapCbvSrvUav[cFrameCnt::GetNowIndex()].Get();
}

template<typename T>
inline void cConstBuf<T>::CreateDescriptorHeapCB()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;		// �V�F�[�_����A�N�Z�X�\
	desc.NodeMask = 0;
	// �x���t���[�������͒萔�o�b�t�@��ێ����Ȃ���Ό��ʂ��s����ɂȂ�
	for (auto& cb : m_DescHeapCbvSrvUav)
	{
		CheckHR(cDirectX12::GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(cb.ReleaseAndGetAddressOf())));
	}
}

template<typename T>
inline void cConstBuf<T>::CreateConstantBuffer()
{
	// �萔�o�b�t�@�T�C�Y��256�o�C�g�ŃA���C�����g����K�v������B
	m_BufferSize = (sizeof(T) + D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1) & ~(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1);

	// �萔�o�b�t�@���\�[�X���쐬
	CheckHR(cDirectX12::GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),		// �萔�o�b�t�@�̓A�b�v���[�h�q�[�v
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(m_BufferSize * DrawParam::g_MaxFrameLatency),		// �`��x�����������쐬����
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_ConstantBuffer.ReleaseAndGetAddressOf())));
	m_ConstantBuffer->SetName(L"ConstantBuffer");

	// �萔�o�b�t�@�̃f�B�X�N���v�^�[���q�[�v�փZ�b�g����
	auto cbvDescHeapIncSize = cDirectX12::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	for (auto i = 0u; i < DrawParam::g_MaxFrameLatency; ++i)
	{
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBuffer->GetGPUVirtualAddress() + (i * m_BufferSize);
		cbvDesc.SizeInBytes = m_BufferSize;
		auto cbvSrvUavDescHeap = m_DescHeapCbvSrvUav[i]->GetCPUDescriptorHandleForHeapStart();
		cDirectX12::GetDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvUavDescHeap);
	}
	CheckHR(m_ConstantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mCBUploadPtr)));
}
