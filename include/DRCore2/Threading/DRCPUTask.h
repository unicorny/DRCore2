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
 * \author Dario Rekowski
 * 
 * \date 2015-09-27
 *
 * \brief One Task for the CPU, only calculation
 */

#ifndef __DR_UNIVERSUM_LIB_CONTROLLER_CPU_TASK_H__
#define __DR_UNIVERSUM_LIB_CONTROLLER_CPU_TASK_H__

#include "DRTask.h"
#include "DRCPUScheduler.h"
#include "DRCore2/export.h"

#include <memory>



class DRCPUTask;
typedef std::shared_ptr<DRCPUTask> DRCPUTaskPtr;

class DRCPUScheduler;

class DRCORE2_EXPORT DRCPUTask : public DRTask
{
public: 
	DRCPUTask(DRCPUScheduler* cpuSheduler, size_t taskDependenceCount);
	DRCPUTask(DRCPUScheduler* cpuScheduler);
    virtual ~DRCPUTask();

	virtual const char* getResourceType() const {return "CPUTask";};
	//! \brief return true if task has finished, else false
	//! automatic scheduling of task if he isn't finished and sheduled yet
			

	virtual void scheduleTask(DRTaskPtr own);
protected:
	void triggerSheduler() { mScheduler->checkPendingTasks(); }
			
private: 
	DRCPUScheduler* mScheduler;
};

#endif //__DR_UNIVERSUM_LIB_CONTROLLER_CPU_TASK_H__
        