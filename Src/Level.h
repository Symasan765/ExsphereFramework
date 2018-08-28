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
	Job m_Job;		// 更新処理用ジョブ
	AsyncJob m_AsyncJob;		// 非同期処理用ジョブ

	Actor* m_pActor;

	// 読み込むファイル名
	std::string m_FileName;
	// ファイルから読み込んだレベルの名前
	std::string m_Data;

	// データにより初期化終了フラグ
	bool m_Initialized = false;

	Level(const Level &) = delete;
	Level &operator=(const Level &) = delete;
};
