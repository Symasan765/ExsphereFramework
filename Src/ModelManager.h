/*=================================================
//								FILE :	ModelManager.h
//		�t�@�C������ :
//		���f���̃��[�h���Ǘ�����}�l�[�W���[�N���X
//		���f����ێ����Ă����A�匳�ɂ̓��\�[�XID��Ԃ�
//									���{ �Y�V��
=================================================*/
#pragma once
#include <string>
#include <unordered_map>
#include "DirectX12.h"
#include "ModelResource.h"

class cModelManager
{
public:
	cModelManager() = default;
	~cModelManager() = default;

	/// <summary>
	/// �t�@�C���������Ƀ��\�[�XID���擾����
	/// </summary>
	/// <param name="fileName">�ǂݍ��ރt�@�C�����B�p�X�͎����I�ɕt�^����</param>
	UINT GetID(std::string fileName);
private:
	std::unordered_map<std::string, UINT> m_IDMap;
	std::unordered_map<UINT, std::unique_ptr<cModelResource>> m_ResourceMap;
};