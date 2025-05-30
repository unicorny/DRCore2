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
/*!
 *
 * \author: Dario Rekowski
 * 
 * \date: 27.08.15
 *
 * \desc: Task save at the same time the result of his task and the way to get to the result
 */

#ifndef __DR_UNIVERSUM_LIB_CONTROLLER_TASK_H__
#define __DR_UNIVERSUM_LIB_CONTROLLER_TASK_H__

#include "DRCore2/export.h"
#include "DRCore2/DRTypes.h"

#include <mutex>
#include <memory>
#include <vector>
#include <assert.h>


class DRTask;
typedef std::shared_ptr<DRTask> DRTaskPtr;

class DRCommand {
public:
	virtual void taskFinished(DRTask* task) = 0;
	virtual ~DRCommand() {}
};

class DRDirtyCommand
{
public:
	virtual void goingDirty() = 0;
	virtual ~DRDirtyCommand() {}
};

class DRMultiCommands : public DRCommand
{
public:
	DRMultiCommands(u8 commandsCount = 4) {
		mCommands.reserve(commandsCount);
	}
	virtual ~DRMultiCommands() {}
	inline void addCommand(std::shared_ptr<DRCommand> command) { 
		assert(mCommands.size() < mCommands.capacity() && command); 
		mCommands.push_back(command); 
	}
	virtual void taskFinished(DRTask* task) {
		for(auto command: mCommands) {
			command->taskFinished(task);
		}
	}
protected:
	std::vector<std::shared_ptr<DRCommand>> mCommands;
};


class DRCORE2_EXPORT DRTask
{
public:
	DRTask();
	DRTask(size_t parentTaskPointerArraySize);
	virtual ~DRTask();

	virtual bool isReady() { return isAllParentsReady(); }
	// called from scheduler
	//! \brief return true if all parent task finished or return false and schedule not already finished parent tasks
	bool isAllParentsReady();
	//! \brief return true if task has finished, else false
	//! automatic scheduling of task if he isn't finished and sheduled yet
	virtual bool isTaskFinished();
	//! \brief called from task scheduler, maybe from another thread
	//! \return if return DR_OK, mark task as finished
	virtual DRReturn run() = 0;

	inline std::unique_lock<std::recursive_mutex> getUniqueLock() { return std::unique_lock(mWorkingMutex); }

	inline void setParentTaskPtrInArray(DRTaskPtr task, size_t index)
	{
		assert(index < mParentTaskPtrs.size());
		mParentTaskPtrs[index] = task;
	}
	/*	inline void setParentTaskPtrInArray(DRResourcePtrHolder* resourceHolder, size_t index) {
			assert(index < mParentTaskPtrArraySize);
			mParentTaskPtrArray[index] = resourceHolder;
		}*/

	inline void setFinishCommand(std::shared_ptr<DRCommand> command) { mFinishCommand = command; }

	// from parent
	virtual const char* getResourceType() const { return "DRTask"; };

	virtual const char* getName() const { return mName.data(); }
	inline void setName(const char* name) { mName = name; }

	virtual void scheduleTask(DRTaskPtr own) = 0;

	void setTaskFinished();
protected:
	// scheduling only once
	inline bool isTaskSheduled() { return mTaskScheduled; }
	inline void taskScheduled() { mTaskScheduled = true; }

	DRTaskPtr getParent(int index);

	bool mTaskScheduled;
	std::shared_ptr<DRCommand> mFinishCommand;
private:
	std::vector<DRTaskPtr> mParentTaskPtrs;
	std::recursive_mutex mWorkingMutex;
	bool     mDeleted;
	bool     mFinished;

	std::string mName;
};

#endif //__DR_UNIVERSUM_LIB_CONTROLLER_TASK_H__