#include "ShaderByte.h"
#include "Utility.h"

cShaderByte::~cShaderByte()
{
	SAFE_RELEASE(m_ShaderData.vs);
	SAFE_RELEASE(m_ShaderData.ps);
	SAFE_RELEASE(m_ShaderData.ds);
	SAFE_RELEASE(m_ShaderData.hs);
	SAFE_RELEASE(m_ShaderData.gs);
}

void cShaderByte::CompileFromFile(std::string fileName, const char * vs, const char * ps, const char * ds, const char * hs, const char * gs)
{
	std::wstring wStr = std::wstring(fileName.begin(), fileName.end());

	ID3D10Blob *info;
	UINT flag = 0;
#if _DEBUG
	flag |= D3DCOMPILE_DEBUG;
#endif /* _DEBUG */

	if (vs != nullptr)
		CheckHR(D3DCompileFromFile(wStr.c_str(), nullptr, nullptr, vs, "vs_5_0", flag, 0, &m_ShaderData.vs, &info));

	if (ps != nullptr)
		CheckHR(D3DCompileFromFile(wStr.c_str(), nullptr, nullptr, ps, "ps_5_0", flag, 0, &m_ShaderData.ps, &info));

	if (ds != nullptr)
		CheckHR(D3DCompileFromFile(wStr.c_str(), nullptr, nullptr, ds, "ds_5_0", flag, 0, &m_ShaderData.ds, &info));

	if (hs != nullptr)
		CheckHR(D3DCompileFromFile(wStr.c_str(), nullptr, nullptr, hs, "hs_5_0", flag, 0, &m_ShaderData.hs, &info));

	if (gs != nullptr)
		CheckHR(D3DCompileFromFile(wStr.c_str(), nullptr, nullptr, gs, "gs_5_0", flag, 0, &m_ShaderData.gs, &info));
}