#include "JobContainer.h"
#include <cassert>
#include "Job.h"

JobContainer::JobContainer()
{
	m_JobList.reserve(MaximumNumberOfExpectedJobs);
}

void JobContainer::MakeReady()
{
	m_CurrentIndex = 0;
}

void JobContainer::Register(Job * job)
{
	m_JobList.push_back(job);
	job->SetContainer(this);
}

void JobContainer::Unregister(Job * job)
{
	std::vector<Job*>::const_iterator end = m_JobList.cend();

	for (auto it = m_JobList.begin(); it != end; it++) {
		if (*it == job) {
			job->SetContainer(nullptr);
			m_JobList.erase(it);
			break;
		}
	}
}

Job * JobContainer::Pop()
{
	uint32_t index = m_CurrentIndex++;
	if (index >= m_JobList.size()) {
		return nullptr;
	}
	return m_JobList[index];
}
