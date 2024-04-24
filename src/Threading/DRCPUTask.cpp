#include "DRCore2/Threading/DRCPUTask.h"
#include "DRCore2/Threading/DRCPUScheduler.h"

	
DRCPUTask::DRCPUTask(DRCPUScheduler* cpuScheduler, size_t taskDependenceCount)
	: DRTask(taskDependenceCount), mScheduler(cpuScheduler)
{
	assert(cpuScheduler);
}

DRCPUTask::DRCPUTask(DRCPUScheduler* cpuScheduler)
	: DRTask(), mScheduler(cpuScheduler)
{
	assert(cpuScheduler);
}


DRCPUTask::~DRCPUTask()
{
}

void DRCPUTask::scheduleTask(DRTaskPtr own)
{
	assert(mScheduler);
	if(!isTaskSheduled()) {
		mScheduler->sheduleTask(own);
		taskScheduled();
	}
}
