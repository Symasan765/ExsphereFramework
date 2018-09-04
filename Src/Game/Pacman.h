#pragma once
#include "../DXMath.h"
#include "StageStruct.h"

#define TravelTimeFrame 20.0f

class cPacman
{
public:
	cPacman() = default;
	~cPacman() = default;

	void Update();
private:
	DirectX::XMINT2 m_NowPos = {1,7};		// 現在のステージ配列番号
	DirectX::XMINT2 m_NextPos = {2,7};	// 次のステージ配列番号
	float m_MoveRate = 1.0f;					// 移動率
	StageMoveDir m_MoveDir = StageMoveDir::RIGHT;
};