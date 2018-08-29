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
	// 全体のスレッドを並列処理
	for (auto tid = 0u; tid < DrawParam::g_ThreadNum; tid++) {
		// スレッド毎に登録されているリソースをすべて処理
		for (auto& itr : m_DrawObjMap[tid].m_Map) {
			// 一つのリソースの合計インスタンス数を取得
			UINT InstanceCount = itr.second.size();

			// 一つのリソースの処理を行う
			for (auto& vect : itr.second) {
				cModelManager manager;
				manager.Draw(itr.first, cmdLists[tid].Get(), true, 2, InstanceCount);		// TODO 仮実装
			}
		}
	}

	// TODO 上述のテストがうまくいったら並列処理に対応させること
	//for (auto tid = 0u; tid < MaxThreadCount; tid++)
	/*parallel_for(0u, DrawParam::g_ThreadNum, [&](auto tid) {

	});*/

	// 描画コマンドを消す
	for (int i = 0; i < DrawParam::g_ThreadNum; i++)
		m_DrawObjMap[i].m_Map.clear();
}