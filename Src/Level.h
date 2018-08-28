#pragma once

#include <cstdint>
#include "Job.h"
#include "AsyncJob.h"

class Actor;

class Level final
{
public:
	Level() = default;
	~Level();

	bool Create();
	void Execute(uint64_t delta_time);
	void Destroy();
private:
	void AsycExecute();
	Job m_Job;		// �X�V�����p�W���u
	AsyncJob m_AsyncJob;		// �񓯊������p�W���u

	Actor* m_pActor;

	// �ǂݍ��ރt�@�C����
	std::string m_FileName;
	// �t�@�C������ǂݍ��񂾃��x���̖��O
	std::string m_Data;

	// �f�[�^�ɂ�菉�����I���t���O
	bool m_Initialized = false;

	Level(const Level &) = delete;
	Level &operator=(const Level &) = delete;
};
