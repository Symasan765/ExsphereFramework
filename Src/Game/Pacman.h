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
	DirectX::XMFLOAT3 GetWorldPos();	// 現在のキャラのワールド座標を計算して取得
	DirectX::XMFLOAT3 GetRotate();		// 現在のキャラの回転を取得
private:
	StageMoveDir MoveDirJudg();
	void NextPosUpdate();
	bool MoveInversion();

	DirectX::XMINT2 m_NowPos = {1,7};		// 現在のステージ配列番号
	DirectX::XMINT2 m_NextPos = {2,7};	// 次のステージ配列番号
	float m_MoveRate = 1.0f;					// 移動率
	StageMoveDir m_MoveDir = StageMoveDir::RIGHT;
};