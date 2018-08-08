/*=================================================
//								FILE :	RenderBufferStruct.h
//		�t�@�C������ :
//		�����_�����O�o�b�t�@�̕ێ��ɗ��p����\����
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include "DirectX12.h"

struct RenderBufferStruct {
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
	D3D12_CPU_DESCRIPTOR_HANDLE descHandleRtv;		// ���݂̃t���[���Ŏg�p����RTV�n���h����ێ�
	D3D12_CPU_DESCRIPTOR_HANDLE descHandleDsv;		// ���݂̃t���[���Ŏg�p����DSV�n���h����ێ�
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> HeapRtv;	// �n���h���擾�Ɏg�p
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> HeapDsv;	// �n���h���擾�Ɏg�p
};