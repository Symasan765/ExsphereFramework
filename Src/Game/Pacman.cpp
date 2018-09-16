#include "Pacman.h"
#include "../Input.h"

using namespace DirectX;
void cPacman::Update()
{
	MoveInversion();

	// 次のマスに移動が終わっている
	if (m_MoveRate >= 1.0f) {
		m_NowPos = m_NextPos;

		StageData stage;
		stage.PacmanPos(m_NowPos.x,m_NowPos.y);
		auto moveDir = MoveDirJudg();
		bool moveFlag = stage.MoveJudge(m_NowPos.x,m_NowPos.y, moveDir);

		// キー入力を元に移動判定
		if (moveFlag) {
			m_MoveDir = moveDir;
			NextPosUpdate();
			m_MoveRate = 0.0f;
		}
		// キーがダメならそのままの移動方向で判定
		else if (stage.MoveJudge(m_NowPos.x, m_NowPos.y, m_MoveDir)) {
			NextPosUpdate();
			m_MoveRate = 0.0f;
		}
	}
	else {
		m_MoveRate += 1.0f / TravelTimeFrame;
		if (m_MoveRate > 1.0f) m_MoveRate = 1.0f;
	}

}

DirectX::XMFLOAT3 cPacman::GetWorldPos()
{
	const float troutSize = 1.8f;		// マス自体のワールド上のサイズ
	const float startX = -4.5f;
	const float startY = 12.6f + (0.6);
	DirectX::XMFLOAT3 wPos = { 0.0f,0.0f,0.0f };

	XMFLOAT2 now = { startX + ((float)m_NowPos.x * troutSize),startY - ((float)m_NowPos.y * troutSize) };
	XMFLOAT2 next = { startX + ((float)m_NextPos.x * troutSize),startY - ((float)m_NextPos.y * troutSize) };

	wPos.x = (now.x * (1.0f - m_MoveRate)) + (next.x * m_MoveRate);
	wPos.y = (now.y * (1.0f - m_MoveRate)) + (next.y * m_MoveRate);

	return wPos;
}

DirectX::XMFLOAT3 cPacman::GetRotate()
{
	switch (m_MoveDir)
	{
	case UP:
		return { 0.0f,0.0f,XMConvertToRadians(270.0f) };
		break;
	case DOWN:
		return { 0.0f,0.0f ,XMConvertToRadians(90.0f) };
		break;
	case LEFT:
		return { 0.0f,0.0f,0.0f };
		break;
	case RIGHT:
		return { 0.0f,0.0f,XMConvertToRadians(180.0f) };
		break;
	default:
		break;
	}

	return { 0.0f,0.0f,0.0f };
}

StageMoveDir cPacman::MoveDirJudg()
{
	// 入力方向を判定していく
	if (Input::cKeyboard::getInstance()->Press(DIK_W)) {
		return StageMoveDir::UP;
	}

	if (Input::cKeyboard::getInstance()->Press(DIK_S)) {
		return StageMoveDir::DOWN;
	}

	if (Input::cKeyboard::getInstance()->Press(DIK_A)) {
		return StageMoveDir::LEFT;
	}

	if (Input::cKeyboard::getInstance()->Press(DIK_D)) {
		return StageMoveDir::RIGHT;
	}

	return m_MoveDir;		// 押されてなければそのままの移動方向に
}

void cPacman::NextPosUpdate()
{
	switch (m_MoveDir)
	{
	case UP:
		m_NextPos.y -= 1;
		break;
	case DOWN:
		m_NextPos.y += 1;
		break;
	case RIGHT:
		m_NextPos.x += 1;
		break;
	case LEFT:
		m_NextPos.x -= 1;
		break;
	default:
		break;
	}
}

bool cPacman::MoveInversion()
{
	bool flag = false;
	switch (m_MoveDir)
	{
	case UP:
		if (Input::cKeyboard::getInstance()->Press(DIK_S)) {
			flag = true;
			m_MoveDir = DOWN;
		}
		break;
	case DOWN:
		if (Input::cKeyboard::getInstance()->Press(DIK_W)) {
			flag = true;
			m_MoveDir = UP;
		}
		break;
	case RIGHT:
		if (Input::cKeyboard::getInstance()->Press(DIK_A)) {
			flag = true;
			m_MoveDir = LEFT;
		}
		break;
	case LEFT:
		if (Input::cKeyboard::getInstance()->Press(DIK_D)) {
			flag = true;
			m_MoveDir = RIGHT;
		}
		break;
	default:
		break;
	}

	if (flag) {
		XMINT2 tmp = m_NextPos;
		m_NextPos = m_NowPos;
		m_NowPos = tmp;
		m_MoveRate = 1.0f - m_MoveRate;
	}

	return flag;
}
