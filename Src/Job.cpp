#include "Job.h"

void Job::Execute(uint64_t delta_time)
{
	m_Function(delta_time);
}
