#include "RenderingFramework.h"
#include "ModelManager.h"
#include "RenderComponent.h"

std::unordered_map<std::string, UINT> cRenderingFramework::m_IDMap;
std::unordered_map<UINT, std::vector<cRenderComponent*>> cRenderingFramework::m_DrawObjMap[DrawParam::g_ThreadNum];

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
	m_DrawObjMap[ResourceID][ResourceID % DrawParam::g_ThreadNum].push_back(pRenderPtr);
}

