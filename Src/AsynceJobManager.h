#pragma once

#include <thread>
#include <mutex>
#include <map>

class AsyncJob;

// �Q�[�����[�v�ɑ΂��Ĕ񓯊��Ŏ��s����鏈�����Ǘ�����
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

	// ���s����񓯊��������L���[�ɒǉ�����
	void Push(AsyncJob* job, uint8_t priority = 128);
	// ���s����񓯊��������L���[������o��
	AsyncJob* Pop();

private:
	// �X���b�h���s�����֐�
	static void Run();
	static AsynceJobManager * m_Instance;
	// �񓯊����������s����X���b�h
	std::thread *m_Thread[kNumThreads];
	// �W���u�L���[�̃X���b�h�Ԕr�������p
	std::mutex m_Mutex;
	// �W���u�L���[�p�̃X���b�h�Ԕr�������p
	std::condition_variable m_Condition;
	// �񓯊����������W���u�L���[
	std::map<uint8_t, AsyncJob*> m_JobQueue;

	bool m_Terminate;

	AsynceJobManager(const AsynceJobManager &) = delete;
	AsynceJobManager &operator=(const AsynceJobManager &) = delete;
};