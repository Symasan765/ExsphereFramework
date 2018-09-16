#pragma once

#include "DXMath.h"

class Wave
{
public:
	Wave();
	~Wave() = default;

	void Update();
	static DirectX::XMFLOAT3 m_WavePos[8][6];
	static DirectX::XMFLOAT3 m_WaveOldPos[8][6];
};