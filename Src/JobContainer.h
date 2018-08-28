#pragma once

#include <vector>

#define MaximumNumberOfExpectedJobs 128

class Job;

class JobContainer final
{
public:
	JobContainer();
	~JobContainer() = default;

	void MakeReady();		// 状態変数をクリアする

	void Register(Job* job);		// ジョブを登録する

	void Unregister(Job* job);		// ジョブの登録を解除する

	Job* Pop();		// 実行するジョブを取得する
private:
	// コンテナ内のジョブの実行管理用
	uint32_t m_CurrentIndex = 0;

	// ジョブ管理コンテナ
	std::vector<Job*> m_JobList;

	JobContainer(const JobContainer &) = delete;
	JobContainer&operator=(const JobContainer&) = delete;
};