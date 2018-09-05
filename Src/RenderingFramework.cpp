#include "RenderingFramework.h"
#include "ModelManager.h"
#include "RenderComponent.h"

using namespace concurrency;

std::unordered_map<std::string, UINT> cRenderingFramework::m_IDMap;
DrawObjMap cRenderingFramework::m_DrawObjMap[DrawParam::g_ThreadNum];

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
	m_DrawObjMap[ResourceID % DrawParam::g_ThreadNum].m_Map[ResourceID].push_back(pRenderPtr);
}

void cRenderingFramework::CommandIssue(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>* cmdLists, int listNum)
{
	// �R�}���h�̔��s��parallel_for���g�p���ĕ��񉻂�����
	//for (auto tid = 0u; tid < MaxThreadCount; tid++)
	parallel_for(0u, DrawParam::g_ThreadNum, [&](auto tid) {
		for (auto& itr : m_DrawObjMap[tid].m_Map) {
			UINT InstanceCount = itr.second.size();
			DirectX::XMFLOAT4X4 mats[DrawParam::g_MaxInstNum];

			auto& vector = itr.second;

			for (int i = 0; i < vector.size(); i++) {
				mats[i] = vector[i]->GetWorldMatrix();
			}
			cModelManager manager;
			manager.Draw(mats, itr.first, cmdLists[tid].Get(), true, 4, InstanceCount);		// TODO ������
		}
	});

	// �`��R�}���h������
	for (int i = 0; i < DrawParam::g_ThreadNum; i++)
		m_DrawObjMap[i].m_Map.clear();
}