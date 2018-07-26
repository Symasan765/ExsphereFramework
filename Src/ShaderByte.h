/*=================================================
//								FILE : ShaderByte.h
//		ファイル説明 :
//		シェーダファイルをコンパイルし、バイナリコードを保持するクラス
//
//									松本 雄之介
=================================================*/
#pragma once
#include <string>
#include <d3dcompiler.h>

struct ShaderData {
	ID3D10Blob* vs = nullptr;
	ID3D10Blob* ps = nullptr;
	ID3D10Blob* ds = nullptr;
	ID3D10Blob* hs = nullptr;
	ID3D10Blob* gs = nullptr;
};

class cShaderByte
{
public:
	cShaderByte() = default;
	~cShaderByte();

	void CompileFromFile(std::string fileName,const char* vs,const char* ps, const char* ds = nullptr, const char* hs = nullptr, const char* gs = nullptr);
	inline ShaderData& GetShaderData() { return m_ShaderData; };
private:
	ShaderData m_ShaderData;
};
