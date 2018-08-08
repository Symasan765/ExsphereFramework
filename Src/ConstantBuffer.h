/*=================================================
//								FILE : ConstantBuffer.h
//		ファイル説明 :
//		定数バッファをの処理を記述。
//		プログラム内で駆動するバッファ数分の定数バッファを
//		型指定することで作成出来るようにする
//									松本 雄之介
=================================================*/
#pragma once
#include <wrl/client.h>
#include "DirectX12.h"
#include <d3dx12.h>
#include "DrawParam.h"
#include "Utility.h"
#include "FrameCnt.h"

/// <summary>
/// 定数バッファを取り扱うテンプレートクラス
/// </summary>
template <typename T> class cConstBuf {
public:
	cConstBuf();				//コンストラクタで定数バッファ作る
	~cConstBuf();				//デストラクタで解放
	void Upload();			// 構造体の情報をアップロードする

	T data;				//ここにデータを格納する
	ID3D12DescriptorHeap* GetDescriptorHeap();
private:
	void CreateDescriptorHeapCB();
	void CreateConstantBuffer();
	// TODO 定数バッファ本体の保持
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
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;		// シェーダからアクセス可能
	desc.NodeMask = 0;
	// 遅延フレーム数分は定数バッファを保持しなければ結果が不安定になる
	for (auto& cb : m_DescHeapCbvSrvUav)
	{
		CheckHR(cDirectX12::GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(cb.ReleaseAndGetAddressOf())));
	}
}

template<typename T>
inline void cConstBuf<T>::CreateConstantBuffer()
{
	// 定数バッファサイズは256バイトでアライメントする必要がある。
	m_BufferSize = (sizeof(T) + D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1) & ~(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1);

	// 定数バッファリソースを作成
	CheckHR(cDirectX12::GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),		// 定数バッファはアップロードヒープ
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(m_BufferSize * DrawParam::g_MaxFrameLatency),		// 描画遅延数分だけ作成する
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_ConstantBuffer.ReleaseAndGetAddressOf())));
	m_ConstantBuffer->SetName(L"ConstantBuffer");

	// 定数バッファのディスクリプターをヒープへセットする
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
