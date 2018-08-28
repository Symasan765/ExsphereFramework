#include "AsynceJobManager.h"

#include <cassert>
#include "AsyncJob.h"

AsynceJobManager * AsynceJobManager::m_Instance = nullptr;

AsynceJobManager::AsynceJobManager() : m_Thread{nullptr,nullptr,nullptr},m_Terminate(false)
{
	assert(m_Instance == nullptr);
	m_Instance = this;
}

AsynceJobManager::~AsynceJobManager()
{
	Destroy();

	assert(m_Instance);
	m_Instance = nullptr;
}

bool AsynceJobManager::Create()
{
	for (uint32_t i = 0; i < kNumThreads; i++) {
		m_Thread[i] = new std::thread(AsynceJobManager::Run);
	}
	return true;
}

void AsynceJobManager::Destroy()
{
	m_Terminate = true;
	m_Condition.notify_all();	// �ҋ@���Ă��邷�ׂẴX���b�h���N��������炵��

	for (uint32_t i = 0; i < kNumThreads; i++) {
		// join()���Ăяo�����Ԃ��ǂ����`�F�b�N
		if (m_Thread[i] && m_Thread[i]->joinable()) {
			m_Thread[i]->join();	// m_Thread[i]���I���܂ő҂�
			delete m_Thread[i];
			m_Thread[i] = nullptr;
		}
	}
}

void AsynceJobManager::Push(AsyncJob * job, uint8_t priority)
{
	// unique_lock�́A�~���[�e�b�N�X��lock()�^unlock()�������A�R���X�g���N�^�ƃf�X�g���N�^�Ŋm���Ɏ��s���邽�߂̃N���X�ł���B
	std::unique_lock<std::mutex> lock(m_Mutex);

	m_JobQueue.insert(std::pair<uint8_t, AsyncJob*>(priority, job));
	m_Condition.notify_one();		// �ҋ@���Ă���X���b�h����N��������
}

AsyncJob * AsynceJobManager::Pop()
{
	std::unique_lock<std::mutex> lock(m_Mutex);
	std::map<uint8_t, AsyncJob*>::iterator it = m_JobQueue.begin();

	if (it != m_JobQueue.cend()) {
		AsyncJob* job = it->second;
		m_JobQueue.erase(it);
		return job;
	}

	// ���s����W���u���Ȃ��̂őҋ@����
	m_Condition.wait(lock);
	return nullptr;
}

void AsynceJobManager::Run()
{
	while (AsynceJobManager::Instance()->m_Terminate == false)
	{
		AsyncJob* job = AsynceJobManager::Instance()->Pop();
		if (job) {
			job->Execute();
		}
	}
}

