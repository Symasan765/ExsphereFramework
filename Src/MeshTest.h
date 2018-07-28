// TODO å„ÅXè¡Ç∑
#pragma once
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <string>
#include <vector>

#include "VertexStruct.h"
#include <wrl/client.h>
#include "DirectX12.h"

class Mesh {
public:
	std::vector<VERTEX_TEST> vertices;
	std::vector<UINT> indices;

	Mesh() = default;
	Mesh(std::vector<VERTEX_TEST> vertices, std::vector<UINT> indices)
	{
		this->vertices = vertices;
		this->indices = indices;

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

	std::vector<Mesh> meshes;
	Mesh processMesh(aiMesh * mesh, const aiScene * scene);
	void processNode(aiNode* node, const aiScene* scene);
};