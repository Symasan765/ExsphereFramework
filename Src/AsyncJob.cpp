#include "AsyncJob.h"

void AsyncJob::Execute()
{
	m_Function();
	m_Finished = true;
}
