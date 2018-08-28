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

	static UINT Load(std::string fileName);
private:
	static std::unordered_map<UINT, std::unique_ptr<cModelResource>> m_ResourceMap;
};