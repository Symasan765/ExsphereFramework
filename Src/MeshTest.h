// TODO 後々消す
#pragma once
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <string>
#include <vector>

#include "VertexStruct.h"
#include <wrl/client.h>
#include "DirectX12.h"
#include "TextureLoader.h"

class Mesh {
public:
	std::vector<VERTEX_TEST> vertices;
	std::vector<UINT> indices;
	std::vector<cTexture> textures;

	Mesh() = default;
	Mesh(std::vector<VERTEX_TEST> vertices, std::vector<UINT> indices,std::vector<cTexture> tex)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = tex;

		setupMesh();
	}

	bool setupMesh();
	Microsoft::WRL::ComPtr<ID3D12Resource> mVB;

	UINT mIndexCount = 0;
	UINT mVBIndexOffset = 0;
	D3D12_VERTEX_BUFFER_VIEW mVBView = {};
	D3D12_INDEX_BUFFER_VIEW mIBView = {};
};

class AssimpTest {
public:
	AssimpTest() = default;
	~AssimpTest() = default;

	void Load(std::string fileName);
	void Draw();

	
	Mesh processMesh(aiMesh * mesh, const aiScene * scene);
	void processNode(aiNode* node, const aiScene* scene);
	std::vector<cTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
	int getTextureIndex(aiString * str);

	std::vector<Mesh> meshes;
	std::vector<cTexture> textures_loaded;	// 読み込んだテクスチャを保持
};