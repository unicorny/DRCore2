/*/*************************************************************************
*                                                                         *
* UniversumLib, collection of classes for generating and go through a     *
* whole universe. It is for my Gameproject Spacecraft					   *
* Copyright (C) 2014, 2015, 2016, 2017 Dario Rekowski.					   *
* Email: dario.rekowski@gmx.de   Web: www.spacecrafting.de                *
*                                                                         *
* This program is free software: you can redistribute it and/or modify    *
* it under the terms of the GNU General Public License as published by    *
* the Free Software Foundation, either version 3 of the License, or       *
* any later version.													   *
*																		   *
* This program is distributed in the hope that it will be useful,	       *
* but WITHOUT ANY WARRANTY; without even the implied warranty of	       *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	       *
* GNU General Public License for more details.							   *
*																		   *
* You should have received a copy of the GNU General Public License	   *
* along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
*                                                                         *
***************************************************************************/


#ifndef __UNI_LIB_DEBUG_CPU_SCHEDULER_TASKS_LOG_H
#define __UNI_LIB_DEBUG_CPU_SCHEDULER_TASKS_LOG_H

#include "DRCore2/export.h"
#include "DRCore2/DRTypes.h"

#include "DRMultithreadContainer.h"

#include <map>

#ifdef DEBUG

class DRCORE2_EXPORT DRCPUSchedulerTasksLog : public DRMultithreadContainer
{
public:
	static DRCPUSchedulerTasksLog* const getInstance();
	inline static bool	isInitialized() { return getInstance()->mInitialized; };

	//! \brief init 
	//! \param defaultCPUSheduler CPUSheduler on which texture loading and saving take place
	//! \param updateTimer timer on which to attach for updating texture storage
	//! \param rerunDelay how much ms passes between update calls
	DRReturn init();
	void exit();

	//! \param ressourcePointer use pointer as id because the pointer should be already unique and don't change for task
	void addTaskLogEntry(u64 ressourcePointer, const char* resourcesTypeName, const char* threadName, const char* name = NULL);
	void removeTaskLogEntry(u64 id);

	void printCurrentlyRunningTasks();
	std::string getCurrentlRunningTasksTableString();

protected:
	DRCPUSchedulerTasksLog();
	virtual ~DRCPUSchedulerTasksLog();

	bool mInitialized;

	struct TaskLogEntry {
		TaskLogEntry(u64 id, const char* typeName, const char* threadName, const char* name)
			: id(id), resourceTypeName(typeName), name(name), threadName(threadName) {}
		std::string resourceTypeName;
		std::string name;
		std::string threadName;
		u64 id;
	};
	typedef std::map<u64, TaskLogEntry*> TaskLogEntryMap;
	typedef std::pair<u64, TaskLogEntry*> TaskLogEntryPair;
	TaskLogEntryMap mTaskLogEntrys;

};
#endif // DEBUG

#endif //__UNI_LIB_DEBUG_CPU_SCHEDULER_TASKS_LOG_H