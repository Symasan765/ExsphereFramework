#include "AssimpLoader.h"
#include "BoneStruct.h"

using namespace std;

std::unique_ptr<cModelResource> cAssimpLoader::Load(std::string filePath)
{
	// 実体を作成
	m_ModelResource.reset(new cModelResource());

	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(filePath,
		aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded);

	if (pScene == NULL)
		return nullptr;

	// ここでボーンの親子関係を作っている
	GetBoneNode(-1, pScene->mRootNode);

	// 実データ取得
	processNode(pScene->mRootNode, pScene);

	// 所有権を呼び出し元へ移動
	return std::move(m_ModelResource);
}

void cAssimpLoader::GetBoneNode(int parentNo, aiNode * node)
{
	auto& m_Bone = m_ModelResource->m_Bone;

	int myNo = m_Bone.size();		// 自分が収まる配列番号を取得する
	BoneStruct data;
	data.name = node->mName.C_Str();	// ここでは名前だけを格納する
	data.parent = parentNo;
	data.Transpose = MatrixTransformation(node->mTransformation);
	m_Bone.push_back(data);

	// Mapにデータ登録
	m_BoneMap[data.name] = myNo;

	for (int i = 0; i < node->mNumChildren; i++) {
		GetBoneNode(myNo, node->mChildren[i]);
	}

}

void cAssimpLoader::processNode(aiNode * node, const aiScene * scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_ModelResource->m_Graphic.push_back(this->processMesh(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}

cGraphic3D cAssimpLoader::processMesh(aiMesh * mesh, const aiScene * scene)
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


	GetBoneData(mesh, vertices);

	cMesh3D meshdata;
	meshdata.DataSet(vertices, indices);
	cMaterial materialdata;
	materialdata.DataSet(textures);
	cAnimation3D animeData;		// TODO アニメーション読み込みを実装すること

	return cGraphic3D(meshdata, materialdata, animeData);
}

std::vector<cTexture> cAssimpLoader::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName, const aiScene * scene)
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

void cAssimpLoader::GetBoneData(aiMesh * mesh, std::vector<DefaultVertex>& vertices)
{
	for (int i = 0; i < mesh->mNumBones; i++) {
		// まずはマップからボーンのデータを取得してくる
		int myBoneNo = m_BoneMap[mesh->mBones[i]->mName.C_Str()];
		m_ModelResource->m_Bone[myBoneNo].Offset = MatrixTransformation(mesh->mBones[i]->mOffsetMatrix);
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

DirectX::XMFLOAT4X4 cAssimpLoader::MatrixTransformation(const aiMatrix4x4 & mat)
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
