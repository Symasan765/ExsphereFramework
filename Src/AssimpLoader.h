/*=================================================
//								FILE :	AssimpLoader.h
//		�t�@�C������ :
//		Assimp��p���ă��f���f�[�^��ǂ݂��ރ��[�_�[
//
//									���{ �Y�V��
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
	void GetBoneNode(int parentNo, aiNode* node);			// �{�[���̐e�q�֌W���\�z����
	void processNode(aiNode* node, const aiScene* scene);
	cGraphic3D processMesh(aiMesh * mesh, const aiScene * scene);
	std::vector<cTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
	void GetBoneData(aiMesh * mesh, std::vector<DefaultVertex>& vertices);

	DirectX::XMFLOAT4X4 MatrixTransformation(const aiMatrix4x4& mat);		// Assimp�̍s��͗�D��A�s�D��ɕύX

	std::unique_ptr<cModelResource> m_ModelResource;					// �����Ƀf�[�^���쐬���Ă����čŌ��std::move�ŌĂяo�����ɏ��L�����ڂ�
	std::unordered_map<std::string, int> m_BoneMap;						// string�c�m�[�h(�{�[��)���@int�c�z��ԍ�
	std::vector<cTexture> textures_loaded;									// �ǂݍ��񂾃e�N�X�`����ێ�
};