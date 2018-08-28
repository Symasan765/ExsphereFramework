#pragma once
#include <array>
#include "JobContainer.h"
#include "ComponentType.h"

class JobScheduler final
{
public:
	static JobScheduler* Instance(){
		return m_Instance;
	}

	JobScheduler();
	~JobScheduler();

	bool Create();
	void Destroy();

	void Register(Job * job, ComponentID type);
	void Unregister(Job* job);

	void Execute(uint64_t delta_time);

private:
	static JobScheduler* m_Instance;

	JobContainer m_Containers[ComponentID::kEnd];
	JobScheduler(const JobScheduler &) = delete;
	JobScheduler &operator=(const JobScheduler &) = delete;
};