#include "RenderingFramework.h"
#include "ModelManager.h"

std::unordered_map<std::string, UINT> cRenderingFramework::m_IDMap;


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
