#pragma once

#include <vector>

#define MaximumNumberOfExpectedJobs 128

class Job;

class JobContainer final
{
public:
	JobContainer();
	~JobContainer() = default;

	void MakeReady();		// ��ԕϐ����N���A����

	void Register(Job* job);		// �W���u��o�^����

	void Unregister(Job* job);		// �W���u�̓o�^����������

	Job* Pop();		// ���s����W���u���擾����
private:
	// �R���e�i���̃W���u�̎��s�Ǘ��p
	uint32_t m_CurrentIndex = 0;

	// �W���u�Ǘ��R���e�i
	std::vector<Job*> m_JobList;

	JobContainer(const JobContainer &) = delete;
	JobContainer&operator=(const JobContainer&) = delete;
};