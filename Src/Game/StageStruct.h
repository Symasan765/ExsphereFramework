#pragma once

// �ړ��\�����@�r�b�g���Z���s���Ďg�p����
enum StageMoveDir {
	UP = 1,
	DOWN = 2,
	RIGHT = 4,
	LEFT = 8
};

// �X�e�[�W�̃}�X���Ƃɍs��������Ȃǂ�ێ�����
struct StageCell
{
	int MoveDir;
	int Obj;		// ���̃I�u�W�F�N�g�������ɂ��邩�H
};

class StageData {
public:
	StageData();
	~StageData();

	bool MoveJudge(const int width, const int height, StageMoveDir dir);
	bool LightJudge(const int width, const int height);		// �F����
	void PacmanPos(const int width, const int height);
	void Clear();
private:
};