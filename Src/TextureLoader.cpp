#include "TextureLoader.h"
#include "Utility.h"
#include <WICTextureLoader.h>
#include <d3dx12.h>
#include <vector>

using namespace Microsoft::WRL;
using namespace DirectX;

cResourceCreateCommand cTextureLoader::m_Command;

void cTextureLoader::LoadTextureFromFile(std::string fileName, cTexture * texture)
{
	ComPtr<ID3D12Resource> textureUploadHeap;		// �ꎞ�ϐ��ł悵
	ID3D12DescriptorHeap* srvHeap = texture->GetDescriptorHeap().Get();
	ID3D12Resource* tex = texture->GetTextureResource().Get();
	auto m_commandList = m_Command.GetList().Get();
	auto m_commandQueue = m_Command.GetQueue().Get();
	// string��wstring�֕ϊ�����
	std::wstring wStr = std::wstring(fileName.begin(), fileName.end());

	{
		// �e�N�X�`�������[�h����
		std::unique_ptr<uint8_t[]> textureData;
		D3D12_SUBRESOURCE_DATA subresouceData;
		CheckHR(LoadWICTextureFromFile(cDirectX12::GetDevice(),
			wStr.c_str(),
			&tex,
			textureData,
			subresouceData));
		D3D12_RESOURCE_DESC textureDesc = tex->GetDesc();

		// �f�[�^�̃A�b�v���[�h�Ɏg�p����o�b�t�@�̕K�v�ȃT�C�Y���擾�B
		const UINT subresoucesize = 1;
		const UINT64 uploadBufferSize
			= GetRequiredIntermediateSize(tex, 0, subresoucesize);

		// �A�b�v���[�h�p�o�b�t�@�̍쐬
		CheckHR(cDirectX12::GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&textureUploadHeap)));

		m_commandList->Reset(m_Command.GetAlloc().Get(), nullptr);

		// �T�u���\�[�X�̍X�V
		UpdateSubresources(m_commandList,
			tex,										// �X�V��A�h���X
			textureUploadHeap.Get(),		// ���ԃA�h���X
			0,											// �I�t�Z�b�g�l
			0,											// ���\�[�X���̍ŏ��̃T�u���\�[�X�̃C���f�b�N�X
			subresoucesize,					// ���\�[�X���̃T�u���\�[�X�̐�
			&subresouceData);					// �X�V�ɗ��p����T�u���\�[�X�f�[�^���܂ޔz��A�h���X
		//tex->SetName(L"TextureResource");

		// �o���A�̐ݒu
		m_commandList->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(tex,
				D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

		// Describe and create a SRV for the texture.
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = textureDesc.Format;	// �e�N�X�`���t�H�[�}�b�g(TK���擾���Ă��ꂽ���́B��{RGBA8)
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = subresoucesize;
		cDirectX12::GetDevice()->CreateShaderResourceView(tex,
			&srvDesc,
			srvHeap->GetCPUDescriptorHandleForHeapStart());

		// �s�v�ɂȂ������ԃf�[�^��j������R�}���h
		m_commandList->DiscardResource(textureUploadHeap.Get(), nullptr);
	}

	// Close the command list and execute it to begin the initial GPU setup.
	CheckHR(m_commandList->Close());
	ID3D12CommandList* ppCommandLists[] = { m_commandList };
	m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// GPU�ɃA�b�v���[�h�����̂�҂�
	cFenceObj FenceObj;
	FenceObj.WaitForPreviousFrame(m_commandQueue);

	texture->SetFilePath(fileName);	// �p�X��o�^���Ă���
}

void cTextureLoader::Init(ID3D12Device * dev)
{
	m_Command.Init(dev);
}

