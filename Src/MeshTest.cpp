
#include "MeshTest.h"
#include "Utility.h"
#include <d3dx12.h>
using namespace std;

void AssimpTest::Load(std::string fileName)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(fileName,
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | 
		aiProcess_ConvertToLeftHanded);

	if (pScene == NULL)
		return;

	processNode(pScene->mRootNode, pScene);
}

Mesh AssimpTest::processMesh(aiMesh * mesh, const aiScene * scene)
{
	vector<VERTEX_TEST> vertices;
	vector<UINT> indices;
	vector<cTexture> textures;

	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		VERTEX_TEST vertex;

		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = -mesh->mVertices[i].z;		// ç∂éËç¿ïWånÇ÷

		if (mesh->mTextureCoords[0])
		{
			vertex.texcoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texcoord.y = (float)mesh->mTextureCoords[0][i].y;
		}

		if (mesh->mNormals) {
			vertex.normal.x = (float)mesh->mNormals[i].x;
			vertex.normal.y = (float)mesh->mNormals[i].y;
			vertex.normal.z = (float)-mesh->mNormals[i].z;		//	ç∂éËç¿ïWånÇ÷
		}

		vertices.push_back(vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++) {
			UINT no = face.mIndices[2 - j];
			indices.push_back(no);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		vector<cTexture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
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

bool Mesh::setupMesh()
{
	int size = sizeof(indices[0]);
	mIndexCount = static_cast<UINT>(indices.size());
	mVBIndexOffset = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());
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
	mVBView.StrideInBytes = sizeof(vertices[0]);
	mVBView.SizeInBytes = mVBIndexOffset;
	mIBView.BufferLocation = mVB->GetGPUVirtualAddress() + mVBIndexOffset;
	mIBView.Format = DXGI_FORMAT_R32_UINT;
	mIBView.SizeInBytes = IBSize;

	return true;
}
