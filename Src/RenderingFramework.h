/*=================================================
//								FILE :	RenderingFramework.h
//		�t�@�C������ :
//		�����_�����O�̐�����s���t���[�����[�N�B
//
//									���{ �Y�V��
=================================================*/
#pragma once
#include "DirectX12.h"
#include <string>
#include <unordered_map>

class cRenderingFramework
{
public:
	cRenderingFramework() = default;
	~cRenderingFramework() = default;

	/// <summary>
	/// �t�@�C���������Ƀ��\�[�XID���擾����
	/// </summary>
	/// <param name="fileName">�ǂݍ��ރt�@�C�����B�p�X�͎����I�ɕt�^����</param>
	static UINT GetID(std::string fileName);
private:
	static std::unordered_map<std::string, UINT> m_IDMap;
};