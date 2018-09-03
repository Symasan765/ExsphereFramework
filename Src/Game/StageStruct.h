#pragma once

// 移動可能方向　ビット演算を行って使用する
enum StageMoveDir {
	UP = 1,
	DOWN = 2,
	RIGHT = 4,
	LEFT = 8
};

// ステージのマスごとに行ける方向などを保持する
struct StageCell
{
	int MoveDir;
	int Obj;		// 何のオブジェクトがそこにいるか？
};

class StageData {
public:
	StageData();
	~StageData();

	bool MoveJudge(const int width, const int height, StageMoveDir dir);
private:
};