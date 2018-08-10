/*=================================================
//								FILE :	AssimpLoader.h
//		ファイル説明 :
//		Assimpを用いてモデルデータを読みこむローダー
//
//									松本 雄之介
=================================================*/
#pragma once
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include<memory>
#include <string>
#include <unordered_map>
#include "ModelResource.h"
#include "TextureLoader.h"

class cAssimpLoader
{
public:
	cAssimpLoader() = default;
	~cAssimpLoader() = default;

	std::unique_ptr<cModelResource> Load(std::string filePath);
private:
	void GetBoneNode(int parentNo, aiNode* node);			// ボーンの親子関係を構築する
	void processNode(aiNode* node, const aiScene* scene);
	cGraphic3D processMesh(aiMesh * mesh, const aiScene * scene);
	std::vector<cTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
	void GetBoneData(aiMesh * mesh, std::vector<DefaultVertex>& vertices);

	DirectX::XMFLOAT4X4 MatrixTransformation(const aiMatrix4x4& mat);		// Assimpの行列は列優先、行優先に変更

	std::unique_ptr<cModelResource> m_ModelResource;					// ここにデータを作成していって最後にstd::moveで呼び出し元に所有権を移す
	std::unordered_map<std::string, int> m_BoneMap;						// string…ノード(ボーン)名　int…配列番号
	std::vector<cTexture> textures_loaded;									// 読み込んだテクスチャを保持
};