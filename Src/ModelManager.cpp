#include "ModelManager.h"
#include "AssimpLoader.h"

std::unordered_map<UINT, std::unique_ptr<cModelResource>> cModelManager::m_ResourceMap;

UINT cModelManager::Load(std::string fileName)
{
	// まずリソース番号を設定する
	// リソース番号はロードされた順に設定
	const UINT ID = static_cast<UINT>(m_ResourceMap.size());

	cAssimpLoader Loader;
	m_ResourceMap[ID] = Loader.Load(fileName);

	return ID;
}

void cModelManager::Draw(UINT ResourceID, ID3D12GraphicsCommandList * cmdList, bool texDrawFlag, UINT RootParameterIndex, UINT InstanceCount)
{
	m_ResourceMap[ResourceID]->Draw(cmdList, texDrawFlag, RootParameterIndex, InstanceCount);
}
