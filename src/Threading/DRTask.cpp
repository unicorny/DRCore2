#include "DRCore2/Threading/DRTask.h"
#include "DRCore2/DRTypes.h"

DRTask::DRTask()
: mTaskScheduled(false), mFinishCommand(nullptr),
	mDeleted(false), mFinished(false)
{
}

DRTask::DRTask(size_t taskPointerArraySize)
    : mTaskScheduled(false), mFinishCommand(nullptr), mParentTaskPtrs(taskPointerArraySize),
        mDeleted(false), mFinished(false)
{
}
		
DRTask::~DRTask()
{
	std::lock_guard _lock(mWorkingMutex);
    mDeleted = true;
}

bool DRTask::isAllParentsReady()
{
    bool allFinished = true;
	std::lock_guard _lock(mWorkingMutex);

	for (auto parentTaskPtr : mParentTaskPtrs) {
		if (parentTaskPtr && !parentTaskPtr->isTaskFinished()) {
			allFinished = false;
			if (!parentTaskPtr->isTaskSheduled()) {
				parentTaskPtr->scheduleTask(parentTaskPtr);
			}
		}
	}
    return allFinished;
}

bool DRTask::isTaskFinished() {
	std::lock_guard _lock(mWorkingMutex);
	return mFinished;
}

DRTaskPtr DRTask::getParent(int index)
{
	if (index < 0 || index >= mParentTaskPtrs.size()) {
		return nullptr;
	}
	return mParentTaskPtrs[index];
}


void DRTask::setTaskFinished() {
	std::lock_guard _lock(mWorkingMutex);
	mFinished = true; 
	if (mFinishCommand) {
		mFinishCommand->taskFinished(this);
	}
}
