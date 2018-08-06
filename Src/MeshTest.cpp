
#include "MeshTest.h"
#include "Utility.h"
#include <d3dx12.h>
using namespace std;

void AssimpTest::GetBoneNode(int parentNo, aiNode * node)
{
	int myNo = m_Bone.size();		// 自分が収まる配列番号を取得する
	BoneData data;
	data.name = node->mName.C_Str();	// ここでは名前だけを格納する
	data.parent = parentNo;
	data.Transpose = MatrixTransformation(node->mTransformation);
	m_Bone.push_back(data);

	// Mapにデータ登録
	m_Map[data.name] = myNo;

	for (int i = 0; i < node->mNumChildren; i++) {
		GetBoneNode(myNo,node->mChildren[i]);
	}
}

void AssimpTest::GetBoneData(aiMesh * mesh, std::vector<DefaultVertex>& vertices)
{
	for (int i = 0; i < mesh->mNumBones; i++) {
		// まずはマップからボーンのデータを取得してくる
		int myBoneNo = m_Map[mesh->mBones[i]->mName.C_Str()];
		m_Bone[myBoneNo].Offset = MatrixTransformation(mesh->mBones[i]->mOffsetMatrix);
		for (int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
			int verID = mesh->mBones[i]->mWeights[j].mVertexId;
			// 最小のウェイト値を見つける
			float* weight[4] = { &vertices[verID].bweight.x,&vertices[verID].bweight.y,&vertices[verID].bweight.z,&vertices[verID].bweight.w };
			int32_t* bonesNo[4] = { &vertices[verID].bindex.x,&vertices[verID].bindex.y,&vertices[verID].bindex.z,&vertices[verID].bindex.w };
			int min = 0;
			for (int k = 0; k < 4; k++) {
				if (*weight[k] < *weight[min]) {
					min = k;
				}
			}
			// 現在保有する最小のウェイトよりも大きいウェイトを見つけた
			if (*weight[min] < mesh->mBones[i]->mWeights[j].mWeight) {
				*bonesNo[min] = myBoneNo;
				*weight[min] = mesh->mBones[i]->mWeights[j].mWeight;
			}
		}
	}
}

void AssimpTest::Load(std::string fileName)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(fileName,
		aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded);

	if (pScene == NULL)
		return;

	// ここでボーンの親子関係を作っている
	GetBoneNode(-1, pScene->mRootNode);

	// 実データ取得
	processNode(pScene->mRootNode, pScene);
}

Mesh AssimpTest::processMesh(aiMesh * mesh, const aiScene * scene)
{
	vector<DefaultVertex> vertices;
	vector<UINT> indices;
	vector<cTexture> textures;

	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		DefaultVertex vertex;

		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;		// 左手座標系へ

		if (mesh->mTextureCoords[0])
		{
			vertex.texcoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texcoord.y = (float)mesh->mTextureCoords[0][i].y;	// 左手座標系に変更
		}

		if (mesh->mNormals) {
			vertex.normal.x = (float)mesh->mNormals[i].x;
			vertex.normal.y = (float)mesh->mNormals[i].y;
			vertex.normal.z = (float)mesh->mNormals[i].z;		//	左手座標系へ
		}

		vertices.push_back(vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++) {
			UINT no = face.mIndices[j];
			indices.push_back(no);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		vector<cTexture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}

	int add = 0;
	for (int i = 0; i < scene->mNumAnimations; i++) {
		for (int j = 0; j < scene->mAnimations[i]->mNumChannels; j++) {
			scene->mAnimations[i]->mChannels[j]->mNodeName;
			
		}
	}


	GetBoneData(mesh, vertices);

	int check = 0;
	for (int i = 0; i < vertices.size(); i++) {
		if (vertices[i].bweight.x + vertices[i].bweight.y + vertices[i].bweight.z + vertices[i].bweight.w < 0.9f)
			check++;
	}

	return Mesh(vertices, indices, textures);
}

void AssimpTest::processNode(aiNode * node, const aiScene * scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(this->processMesh(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}

vector<cTexture> AssimpTest::loadMaterialTextures(aiMaterial * mat, aiTextureType type, string typeName, const aiScene * scene)
{
	vector<cTexture> textures;
	for (UINT i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (UINT j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].GetPath().c_str(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // If texture hasn't been loaded already, load it
			HRESULT hr;
			cTexture texture;
			{
				string filename = string(str.C_Str());
				filename = std::string("Private") + std::string("/") + filename;
				cTextureLoader::LoadTextureFromFile(filename, &texture);
			}
			texture.SetFilePath(str.C_Str());
			textures.push_back(texture);
			this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

int AssimpTest::getTextureIndex(aiString * str)
{
	string tistr;
	tistr = str->C_Str();
	tistr = tistr.substr(1);
	return stoi(tistr);
}

DirectX::XMFLOAT4X4 AssimpTest::MatrixTransformation(const aiMatrix4x4& mat)
{
	DirectX::XMFLOAT4X4 ret;
	ret._11 = mat.a1;
	ret._12 = mat.b1;
	ret._13 = mat.c1;
	ret._14 = mat.d1;

	ret._21 = mat.a2;
	ret._22 = mat.b2;
	ret._23 = mat.c2;
	ret._24 = mat.d2;

	ret._31 = mat.a3;
	ret._32 = mat.b3;
	ret._33 = mat.c3;
	ret._34 = mat.d3;

	ret._41 = mat.a4;
	ret._42 = mat.b4;
	ret._43 = mat.c4;
	ret._44 = mat.d4;

	return ret;
}

bool Mesh::setupMesh()
{
	int size = sizeof(indices[0]);
	mIndexCount = static_cast<UINT>(indices.size());
	mVBIndexOffset = static_cast<UINT>(sizeof(DefaultVertex) * vertices.size());
	UINT IBSize = static_cast<UINT>(sizeof(indices[0]) * mIndexCount);

	void* vbData = (void*)&vertices[0];
	void* ibData = (void*)&indices[0];
	CheckHR(cDirectX12::GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(mVBIndexOffset + IBSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(mVB.ReleaseAndGetAddressOf())));

	cDirectX12::GetDevice()->SetName(L"VertexBuffer");
	char* vbUploadPtr = nullptr;
	CheckHR(mVB->Map(0, nullptr, reinterpret_cast<void**>(&vbUploadPtr)));
	memcpy_s(vbUploadPtr, mVBIndexOffset, vbData, mVBIndexOffset);
	memcpy_s(vbUploadPtr + mVBIndexOffset, IBSize, ibData, IBSize);
	mVB->Unmap(0, nullptr);

	mVBView.BufferLocation = mVB->GetGPUVirtualAddress();
	mVBView.StrideInBytes = sizeof(DefaultVertex);
	mVBView.SizeInBytes = mVBIndexOffset;
	mIBView.BufferLocation = mVB->GetGPUVirtualAddress() + mVBIndexOffset;
	mIBView.Format = DXGI_FORMAT_R32_UINT;
	mIBView.SizeInBytes = IBSize;

	return true;
}
