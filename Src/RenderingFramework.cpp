#include "RenderingFramework.h"
#include "ModelManager.h"

std::unordered_map<std::string, UINT> cRenderingFramework::m_IDMap;


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
