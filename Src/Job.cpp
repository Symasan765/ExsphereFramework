#include "Job.h"

void Job::Execute(float delta_time)
{
	m_Function(delta_time);
}
