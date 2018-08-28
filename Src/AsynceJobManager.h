#pragma once

#include <thread>
#include <mutex>
#include <map>

class AsyncJob;

// ゲームループに対して非同期で実行される処理を管理する
class AsynceJobManager
{
public:
	static constexpr unsigned kNumThreads = 3;

	static AsynceJobManager* Instance() {
		return m_Instance;
	}

	AsynceJobManager();
	~AsynceJobManager();

	bool Create();
	void Destroy();

	// 実行する非同期処理をキューに追加する
	void Push(AsyncJob* job, uint8_t priority = 128);
	// 実行する非同期処理をキューから取り出す
	AsyncJob* Pop();

private:
	// スレッド実行される関数
	static void Run();
	static AsynceJobManager * m_Instance;
	// 非同期処理を実行するスレッド
	std::thread *m_Thread[kNumThreads];
	// ジョブキューのスレッド間排他処理用
	std::mutex m_Mutex;
	// ジョブキュー用のスレッド間排他処理用
	std::condition_variable m_Condition;
	// 非同期処理されるジョブキュー
	std::map<uint8_t, AsyncJob*> m_JobQueue;

	bool m_Terminate;

	AsynceJobManager(const AsynceJobManager &) = delete;
	AsynceJobManager &operator=(const AsynceJobManager &) = delete;
};