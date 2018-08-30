/*=================================================
//								FILE :	cInputLayout.h
//		�t�@�C������ :
//		�C���v�b�g���C�A�E�g�̍쐬���菕������w���p�[�N���X�B
//		�C���v�b�g���C�A�E�g�̌^�ƃZ�}���e�B�N�X����ǉ����Ă����Ɗ�������悤�ɍs��
//									���{ �Y�V��
=================================================*/
#pragma once
#include <vector>
#include "DirectX12.h"

class cInputLayout
{
public:
	cInputLayout() = default;
	~cInputLayout() = default;

	template <typename T>		// �V�F�[�_���Ŏg�p����^�Ɠ��`��Math�^��n���Ƃ��̕��̃A���C�����g���v�Z����
	void AddElement(const char* SemanticName, DXGI_FORMAT Format, D3D12_INPUT_CLASSIFICATION InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,UINT SemanticIndex = 0, UINT InputSlot = 0, UINT InstanceDataStepRate = 0);
	inline D3D12_INPUT_ELEMENT_DESC* GetInputElementDesc() { return &m_InputLayout[0]; };
	inline size_t GetSize() { return m_InputLayout.size(); };
private:
	std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayout;
	UINT m_AlignedByteOffset[15] = { 0 };
};

template<typename T>
inline void cInputLayout::AddElement(const char* SemanticName, DXGI_FORMAT Format, D3D12_INPUT_CLASSIFICATION InputSlotClass, UINT SemanticIndex, UINT InputSlot, UINT InstanceDataStepRate)
{
	m_InputLayout.push_back({ SemanticName, SemanticIndex, Format, InputSlot, m_AlignedByteOffset[InputSlot], InputSlotClass, InstanceDataStepRate });
	m_AlignedByteOffset[InputSlot] += sizeof(T);		// �A���C�����g�T�C�Y�𑝉�����
}
