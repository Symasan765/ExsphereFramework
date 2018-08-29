#include "RenderingFramework.h"
#include "ModelManager.h"
#include "RenderComponent.h"

std::unordered_map<std::string, UINT> cRenderingFramework::m_IDMap;
std::unordered_map<UINT, std::vector<cRenderComponent*>> cRenderingFramework::m_DrawObjMap[DrawParam::g_ThreadNum];

UINT cRenderingFramework::GetID(std::string fileName)
{
	auto itr = m_IDMap.find(fileName);

	if (itr != m_IDMap.end()) {
		// ���Ƀ��\�[�X�����݂���
		return m_IDMap[fileName];
	}

	// �܂����\�[�X�����݂��Ȃ�
	auto ID = cModelManager::Load(fileName);	// ���[�h���ă��\�[�XID���擾����
	m_IDMap[fileName] = ID;
	return ID;
}

void cRenderingFramework::RnederingRegister(UINT ResourceID, cRenderComponent * pRenderPtr)
{
	// �}���`�X���b�h�ł̐݌v���s���̂ŃV���v���Ƀ��\�[�X�ԍ������ɂ��ăX���b�h��U�蕪����
	// TODO ����A�������I�Ƀ��\�[�X�̐U�蕪�����s���ꍇ�ύX�̕K�v����
	m_DrawObjMap[ResourceID][ResourceID % DrawParam::g_ThreadNum].push_back(pRenderPtr);
}

