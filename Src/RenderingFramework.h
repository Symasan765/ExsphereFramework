/*=================================================
//								FILE :	RenderingFramework.h
//		�t�@�C������ :
//		�����_�����O�̐�����s���t���[�����[�N�B
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include "DirectX12.h"
#include "DrawParam.h"
#include <string>
#include <unordered_map>
#include <vector>

class cRenderComponent;

struct DrawObjMap {
	std::unordered_map<UINT, std::vector<cRenderComponent*>> m_Map;		//���\�[�XID�Ƀ����_�����O�Ώۂ�z��ŕR�Â���
};

// �A�N�Z�X����static�̗L���ŃR���g���[������
class cRenderingFramework
{
public:
	cRenderingFramework() = default;
	~cRenderingFramework() = default;

	/// <summary>
	/// �t�@�C���������Ƀ��\�[�XID���擾����
	/// </summary>
	/// <param name="fileName">�ǂݍ��ރt�@�C�����B�p�X�͎����I�ɕt�^����</param>
	static UINT GetID(std::string fileName);

	/// <summary>
	/// �V�[������`��Ώۂ̃I�u�W�F�N�g��o�^����֐�
	/// </summary>
	/// <param name="ResourceID">ResourceID</param>
	/// <param name="pRenderPtr">�����_�����O�R���|�[�l���g�̃A�h���X</param>
	static void RnederingRegister(UINT ResourceID, cRenderComponent* pRenderPtr);

	/// <summary>
	/// �R�}���h���X�g�փR�}���h�̔��s���s���֐��B
	/// </summary>
	void CommandIssue(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>* cmdLists,int listNum);
private:
	static std::unordered_map<std::string, UINT> m_IDMap;
	static DrawObjMap m_DrawObjMap[DrawParam::g_ThreadNum];		//���\�[�XID�Ƀ����_�����O�Ώۂ�z��ŕR�Â���
};