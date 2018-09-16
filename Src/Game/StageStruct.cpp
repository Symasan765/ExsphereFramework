#include "StageStruct.h"
#include "../Input.h"

namespace {
	constexpr int g_Width = 6;
	constexpr int g_Height = 8;
	bool StageFlagData[g_Height][g_Width] = { true };
	/*StageMoveDir g_Stage[height][width] = {
		{}
	}*/
}

StageData::StageData()
{
	static bool InitFlag = false;
	if (!InitFlag) {
		for (int i = 0; i < g_Height; i++) {
			for (int j = 0; j < g_Width; j++) {
				StageFlagData[i][j] = true;
			}
		}
		InitFlag = true;
	}
}

StageData::~StageData()
{
}

bool StageData::MoveJudge(const int width, const int height, StageMoveDir dir)
{
	static int data[g_Height][g_Width] = {
		{DOWN | RIGHT , LEFT | DOWN | RIGHT,LEFT | RIGHT , LEFT | RIGHT | DOWN,LEFT | RIGHT , LEFT | DOWN},
	{UP | DOWN,UP | RIGHT,LEFT | RIGHT | DOWN,LEFT | UP | DOWN,RIGHT | DOWN,UP | DOWN | LEFT},
	{UP | DOWN | RIGHT,DOWN | RIGHT | LEFT,UP | LEFT,UP | DOWN,UP | DOWN,UP | DOWN},
	{UP | DOWN,UP | RIGHT,LEFT | RIGHT | DOWN,UP | DOWN | LEFT | RIGHT,UP | LEFT,UP | DOWN},
	{UP | DOWN | RIGHT,LEFT | RIGHT,UP | DOWN | LEFT,UP | DOWN | RIGHT,RIGHT | LEFT,UP | DOWN | LEFT},
	{UP | DOWN,DOWN | RIGHT,UP | DOWN | LEFT | RIGHT,UP | LEFT | RIGHT,DOWN | LEFT | RIGHT,UP | DOWN | LEFT},
	{UP | DOWN | RIGHT,UP | LEFT,UP | RIGHT,DOWN | LEFT | RIGHT,UP | LEFT,UP | DOWN},
	{UP | RIGHT,LEFT | RIGHT,LEFT | RIGHT,UP | LEFT | RIGHT,LEFT | RIGHT,UP | LEFT}
	};

	// s‚¯‚é
	if (data[height][width] & dir)
		return true;

	// s‚¯‚È‚©‚Á‚½
	return false;
}

bool StageData::LightJudge(const int width, const int height)
{
	return StageFlagData[height][width];
}

void StageData::PacmanPos(const int width, const int height)
{
	StageFlagData[height][width] = false;
}

void StageData::Clear()
{
	if (Input::cKeyboard::getInstance()->Trigger(DIK_SPACE)) {
		for (int i = 0; i < g_Height; i++) {
			for (int j = 0; j < g_Width; j++) {
				StageFlagData[i][j] = true;
			}
		}
	}
}
