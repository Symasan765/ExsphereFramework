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
		// 既にリソースが存在する
		return m_IDMap[fileName];
	}

	// まだリソースが存在しない
	auto ID = cModelManager::Load(fileName);	// ロードしてリソースIDを取得する
	m_IDMap[fileName] = ID;
	return ID;
}

void cRenderingFramework::RnederingRegister(UINT ResourceID, cRenderComponent * pRenderPtr)
{
	// マルチスレッドでの設計を行うのでシンプルにリソース番号を元にしてスレッドを振り分ける
	// TODO 今後、より効率的にリソースの振り分けを行う場合変更の必要あり
	m_DrawObjMap[ResourceID % DrawParam::g_ThreadNum].m_Map[ResourceID].push_back(pRenderPtr);
}

void cRenderingFramework::CommandIssue(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>* cmdLists, int listNum)
{
	// コマンドの発行をparallel_forを使用して並列化させる
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
			manager.Draw(mats, itr.first, cmdLists[tid].Get(), true, 4, InstanceCount);		// TODO 仮実装
		}
	});

	// 描画コマンドを消す
	for (int i = 0; i < DrawParam::g_ThreadNum; i++)
		m_DrawObjMap[i].m_Map.clear();
}