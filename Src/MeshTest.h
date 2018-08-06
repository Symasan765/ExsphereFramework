// TODO ��X����
#pragma once
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <string>
#include <vector>
#include <unordered_map>

#include "VertexStruct.h"
#include <wrl/client.h>
#include "DirectX12.h"
#include "TextureLoader.h"

struct BoneData {
	DirectX::XMFLOAT4X4 Offset;
	DirectX::XMFLOAT4X4 Transpose;
	UINT parent;
	std::string name;

	BoneData() {
		DirectX::XMStoreFloat4x4(&Offset, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&Transpose, DirectX::XMMatrixIdentity());
		parent = -1;
	}
};

class Mesh {
public:
	std::vector<DefaultVertex> vertices;
	std::vector<UINT> indices;
	std::vector<cTexture> textures;

	Mesh() = default;
	Mesh(std::vector<DefaultVertex> vertices, std::vector<UINT> indices,std::vector<cTexture> tex)
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

	void GetBoneNode(int parentNo, aiNode* node);			// �{�[���̐e�q�֌W���\�z����
	void GetBoneData(aiMesh * mesh, std::vector<DefaultVertex>& vertices);

	std::vector<cTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
	int getTextureIndex(aiString * str);

	DirectX::XMFLOAT4X4 MatrixTransformation(const aiMatrix4x4& mat);		// Assimp�̍s��͗�D��A�s�D��ɕύX

	std::vector<Mesh> meshes;
	std::vector<cTexture> textures_loaded;	// �ǂݍ��񂾃e�N�X�`����ێ�
	std::unordered_map<std::string, int> m_Map;	// string�c�m�[�h���@int�c�z��ԍ�
	std::vector<BoneData> m_Bone;
};