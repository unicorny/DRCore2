#include "DRCore2/DRCore2Main.h"
#include "DRCore2/Threading/DRCPUScheduler.h"
#include "DRCore2/Threading/DRCPUSchedulerThread.h"
#include "DRCore2/Threading/DRCPUTask.h"

#include <memory.h>


DRCPUScheduler::DRCPUScheduler(uint8_t threadCount, const std::string& name)
	: mThreads(new DRCPUSchedulerThread*[threadCount]), mThreadCount(threadCount), mName(name), mStopped(false)
{
	char nameBuffer[10]; memset(nameBuffer, 0, 10);
	//uint8_t len = std:: min(strlen(name), 7);
	uint8_t len = name.size();
	if(len > 7) len = 7;
	memcpy(nameBuffer, name.data(), len);
		
	for(int i = 0; i < threadCount; i++) {
		sprintf(&nameBuffer[len], "%.2d", i); 
		mThreads[i] = new DRCPUSchedulerThread(*this, nameBuffer);
	}
}

DRCPUScheduler::~DRCPUScheduler()
{
	for(int i = 0; i < mThreadCount; i++) {
		DR_SAVE_DELETE(mThreads[i]);
	}
	DR_SAVE_DELETE_ARRAY(mThreads);
	mThreadCount = 0;
}

int DRCPUScheduler::sheduleTask(DRTaskPtr task)
{
	{ // scoped lock
		std::scoped_lock _lock(mCheckStopMutex);
		if (mStopped) {
			return 0;
		}
	} // scoped lock end
	DRCPUSchedulerThread* t = NULL;
	// look at free worker threads
	if(task->isAllParentsReady() && mFreeWorkerThreads.pop(t)) {
		// gave him the new task
		t->setNewTask(task);
	} else {
		// else put task to pending queue
		DRLog.writeToLog("[DRCPUScheduler::sheduleTask] all %d threads in use", getThreadCount());
		mPendingTasksMutex.lock();
		mPendingTasks.push_back(task);
		mPendingTasksMutex.unlock();
	}
	return 0;
}

void DRCPUScheduler::stop()
{
	{
		std::scoped_lock _lock(mCheckStopMutex);
		mStopped = true;
	}
	{
		std::scoped_lock _lock(mPendingTasksMutex);
		mPendingTasks.clear();
	}
}
DRTaskPtr DRCPUScheduler::getNextUndoneTask(DRCPUSchedulerThread* Me)
{
	// look at pending tasks
	DRTaskPtr task;
	std::scoped_lock _lock(mPendingTasksMutex);
	for (std::list<DRTaskPtr>::iterator it = mPendingTasks.begin(); it != mPendingTasks.end(); it++) {
		if ((*it)->isAllParentsReady()) {
			task = *it;
			mPendingTasks.erase(it);
			return task;
		}
	}
	// push thread to worker queue
	if (Me) {
		mFreeWorkerThreads.push(Me);
	}
			
	return DRTaskPtr();
}
void DRCPUScheduler::checkPendingTasks()
{
	{ // scoped lock
		std::scoped_lock _lock(mCheckStopMutex);
		if (mStopped) {
			return;
		}
	} // scoped lock end
	DRTaskPtr task = getNextUndoneTask(nullptr);
	if (task) {
		sheduleTask(task);
	}
}