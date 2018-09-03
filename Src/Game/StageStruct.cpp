#include "StageStruct.h"

namespace {
	constexpr int g_Width = 6;
	constexpr int g_Height = 8;

	/*StageMoveDir g_Stage[height][width] = {
		{}
	}*/
}

StageData::StageData()
{
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

	// çsÇØÇÈ
	if (data[height][width] & dir)
		return true;

	// çsÇØÇ»Ç©Ç¡ÇΩ
	return false;
}
