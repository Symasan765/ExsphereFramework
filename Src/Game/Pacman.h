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
	DirectX::XMFLOAT3 GetWorldPos();	// ���݂̃L�����̃��[���h���W���v�Z���Ď擾
	DirectX::XMFLOAT3 GetRotate();		// ���݂̃L�����̉�]���擾
private:
	StageMoveDir MoveDirJudg();
	void NextPosUpdate();
	bool MoveInversion();

	DirectX::XMINT2 m_NowPos = {1,7};		// ���݂̃X�e�[�W�z��ԍ�
	DirectX::XMINT2 m_NextPos = {2,7};	// ���̃X�e�[�W�z��ԍ�
	float m_MoveRate = 1.0f;					// �ړ���
	StageMoveDir m_MoveDir = StageMoveDir::RIGHT;
};