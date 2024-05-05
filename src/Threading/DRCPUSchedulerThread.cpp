#include "DRCore2/DRCore2Main.h"
#include "DRCore2/Foundation/DRBaseExceptions.h"
#include "DRCore2/Threading/DRCPUSchedulerThread.h"
#include "DRCore2/Threading/DRCPUScheduler.h"
#include "DRCore2/Threading/DRTask.h"
#include "DRCore2/Threading/DRCPUSchedulerTasksLog.h"

#ifdef PROFILING
#include "DRCore2/Utils/DRProfiler.h"
#endif //PROFILING

DRCPUSchedulerThread::DRCPUSchedulerThread(DRCPUScheduler& parent, const char* name)
	: DRThread(name), mParent(parent)
{
	mWaitingTask = mParent.getNextUndoneTask(this);
}

DRCPUSchedulerThread::~DRCPUSchedulerThread()
{
}

int DRCPUSchedulerThread::ThreadFunction()
{
	while(mWaitingTask)
	{
#ifdef PROFILING
		DRProfiler counter;
		DRCPUSchedulerTasksLog* l = DRCPUSchedulerTasksLog::getInstance();
		auto name = mWaitingTask->getName();
		
		l->addTaskLogEntry((u64)mWaitingTask.get(), mWaitingTask->getResourceType(), getName().data(), name);
#endif
		try {
			int returnValue = mWaitingTask->run();
			if (!returnValue) {
				mWaitingTask->setTaskFinished();
			}
#ifdef PROFILING
			l->removeTaskLogEntry((u64)mWaitingTask.get());
			DRSpeedLog.writeToLog("%s used on thread: %s by Task: %s of: %s (returned: %d)",
				counter.string().data(), getName().data(), mWaitingTask->getResourceType(), name, returnValue);
#endif
		}
		catch (DRCoreBaseException& ex) {
			DRLog.writeToLog("[DRCPUSchedulerThread] DRCore2 exception %s for task type: %s\n", ex.toString().data(), mWaitingTask->getResourceType());
#ifdef DEBUG
			DRLog.writeToLog("task name: %s\n", getName().data());
#endif
		}
		catch (std::exception& ex) {
			DRLog.writeToLog("[DRCPUSchedulerThread] exception %s for task type: %s\n", ex.what(), mWaitingTask->getResourceType());
#ifdef DEBUG
			DRLog.writeToLog("task name: %s\n", getName().data());
#endif
		}
		mWaitingTask = mParent.getNextUndoneTask(this);
	}
	return 0;
}

void DRCPUSchedulerThread::setNewTask(DRTaskPtr cpuTask)
{
	mWaitingTask = cpuTask;	
	condSignal();
}