#include "DRCore2/Foundation/DRBaseExceptions.h"
#include "DRCore2/Threading/DRCPUSchedulerThread.h"
#include "DRCore2/Threading/DRCPUScheduler.h"
#include "DRCore2/Threading/DRTask.h"
#include "DRCore2/Threading/DRCPUSchedulerTasksLog.h"

#ifdef DEBUG
#include "DRCore2/Utils/DRProfiler.h"
#endif //DEBUG

#ifdef DEBUG
DRCPUSchedulerThread::DRCPUSchedulerThread(DRCPUScheduler& parent, const char* name, DRLogger& speedLog)
  : DRThread(name), mParent(parent), mSpeedLog(speedLog)
{
	mWaitingTask = mParent.getNextUndoneTask(this);
}
#else 
DRCPUSchedulerThread::DRCPUSchedulerThread(DRCPUScheduler& parent, const char* name)
	: DRThread(name), mParent(parent)
{
	mWaitingTask = mParent.getNextUndoneTask(this);
}
#endif // DEBUG
DRCPUSchedulerThread::~DRCPUSchedulerThread()
{
}

int DRCPUSchedulerThread::ThreadFunction()
{
	while(mWaitingTask)
	{
				
#ifdef DEBUG
		DRProfiler counter;
		DRCPUSchedulerTasksLog* l = DRCPUSchedulerTasksLog::getInstance();
		auto name = mWaitingTask->getName();
		auto threadName = getName().data();
		l->addTaskLogEntry((u64)mWaitingTask.get(), mWaitingTask->getResourceType(), threadName, name);
#endif
		try {
			int returnValue = mWaitingTask->run();
			if (!returnValue) {
				mWaitingTask->setTaskFinished();
			}
#ifdef DEBUG
			l->removeTaskLogEntry((u64)mWaitingTask.get());
			mSpeedLog.writeToLog("%s used on thread: %s by Task: %s of: %s (returned: %d)",
				counter.string().data(), threadName, mWaitingTask->getResourceType(), name, returnValue);
#endif
		}
		catch (DRCoreBaseException& ex) {
			DRLog.writeToLog("[DRCPUSchedulerThread] DRCore2 exception %s for task type: %s\n", ex.toString().data(), mWaitingTask->getResourceType());
#ifdef DEBUG
			DRLog.writeToLog("task name: %s\n", threadName);
#endif
		}
		catch (std::exception& ex) {
			DRLog.writeToLog("[DRCPUSchedulerThread] exception %s for task type: %s\n", ex.what(), mWaitingTask->getResourceType());
#ifdef DEBUG
			DRLog.writeToLog("task name: %s\n", threadName);
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