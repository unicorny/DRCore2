#include "DRCore2/Threading/DRCPUSchedulerTasksLog.h"
#include "DRCore2/Foundation/DRLogger.h"
#include <sstream>

#ifdef DEBUG

DRCPUSchedulerTasksLog::DRCPUSchedulerTasksLog()
	: mInitialized(false)
{

}
DRCPUSchedulerTasksLog::~DRCPUSchedulerTasksLog()
{
	if(mInitialized) exit();
}
DRCPUSchedulerTasksLog* const DRCPUSchedulerTasksLog::getInstance()
{
	static DRCPUSchedulerTasksLog theOne;
	return &theOne;
}

DRReturn DRCPUSchedulerTasksLog::init()
{
	mInitialized = true;
	return DR_OK;
}

void DRCPUSchedulerTasksLog::exit()
{			
	for (TaskLogEntryMap::iterator it = mTaskLogEntrys.begin(); it != mTaskLogEntrys.end(); it++) {
		DR_SAVE_DELETE(it->second);
	}
	mTaskLogEntrys.clear();
	mInitialized = false;
}

void DRCPUSchedulerTasksLog::addTaskLogEntry(u64 id, const char* resourcesTypeName, const char* threadName, const char* name/* = NULL*/)
{
	auto _lock = getUniqueLock("DRCPUSchedulerTasksLog::addTaskLogEntry");
	mTaskLogEntrys.insert(TaskLogEntryPair(id, new TaskLogEntry(id, resourcesTypeName, threadName, name)));
	u8 count = mTaskLogEntrys.size();
	//if(id != 1)
		//printCurrentlyRunningTasks();
}
void DRCPUSchedulerTasksLog::removeTaskLogEntry(u64 id)
{
	auto _lock = getUniqueLock("DRCPUSchedulerTasksLog::addTaskLogEntry");
	TaskLogEntryMap::iterator it = mTaskLogEntrys.find(id);
	if (it != mTaskLogEntrys.end()) {
		DR_SAVE_DELETE(it->second);
		mTaskLogEntrys.erase(it);
	}
}
void DRCPUSchedulerTasksLog::printCurrentlyRunningTasks()
{
	DRLog.writeToLogDirect(getCurrentlRunningTasksTableString());
}
std::string DRCPUSchedulerTasksLog::getCurrentlRunningTasksTableString()
{
	std::stringstream ss;
	ss << "<tr><td><table style='color:grey;font-size:10px;border: 1px solid grey'>";
	ss << "<tr><th>Thread Name</th><th>Resource Type</th><th>Task Name</th></tr>";
	{
		auto _lock = getUniqueLock("DRCPUSchedulerTasksLog::addTaskLogEntry");
		for (TaskLogEntryMap::iterator it = mTaskLogEntrys.begin(); it != mTaskLogEntrys.end(); it++) {
			ss << "<tr><td>";
			ss << it->second->threadName;
			ss << "</td><td>";
			ss << it->second->resourceTypeName;
			ss << "</td><td>";
			ss << it->second->name;
			ss << "</td></tr>";
		}
	}
	ss << "</table></td></tr>";
	return ss.str();
}
#endif // DEBUG
