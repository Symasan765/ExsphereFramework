#include "JobScheduler.h"
#include <cassert>
#include "Job.h"

JobScheduler* JobScheduler::m_Instance = new JobScheduler();

JobScheduler::JobScheduler()
{
	assert(m_Instance == nullptr);
	m_Instance = this;
}

JobScheduler::~JobScheduler()
{
	Destroy();
	m_Instance = nullptr;
}

bool JobScheduler::Create()
{
	return true;
}

void JobScheduler::Destroy()
{

}

void JobScheduler::Register(Job * job, ComponentID type)
{
	m_Containers[type].Register(job);
}

void JobScheduler::Unregister(Job * job)
{
}

void JobScheduler::Execute(float delta_time)
{
	for (JobContainer& container : m_Containers) {
		container.MakeReady();
	}

	for (JobContainer& container : m_Containers) {
		Job* job = container.Pop();
		if (!job) continue;

		job->Execute(delta_time);
	}
}

