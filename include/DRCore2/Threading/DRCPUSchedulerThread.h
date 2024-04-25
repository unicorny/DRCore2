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
 * \date: 27.09.15
 *
 * \desc: CPU Scheduler Thread, one of multiple threads of CPU Scheduler
 */

#ifndef __DR_UNIVERSUM_LIB_CONTROLLER_CPU_SCHEDULER_THREAD_H__
#define __DR_UNIVERSUM_LIB_CONTROLLER_CPU_SCHEDULER_THREAD_H__

#include "DRCore2/export.h"
#include "DRThread.h"
#ifdef DEBUG
#include "DRCore2/Threading/DRMultithreadLogger.h"
#endif

#include <memory>

class  DRTask;
typedef std::shared_ptr<DRTask> DRTaskPtr;
class DRCPUScheduler;

class DRCORE2_EXPORT DRCPUSchedulerThread : public DRThread
{
public: 
#ifdef DEBUG
	DRCPUSchedulerThread(DRCPUScheduler& parent, const char* name, DRMultithreadLogger& speedLog);
#else 
	DRCPUSchedulerThread(DRCPUScheduler& parent, const char* name);
#endif

    virtual ~DRCPUSchedulerThread();
			
	//! \brief will be called every time from thread, when condSignal was called
	//! will be called from thread with locked working mutex,<br>
	//! mutex will be unlock after calling this function
	//! \return if return isn't 0, thread will exit
	virtual int ThreadFunction();

	void setNewTask(DRTaskPtr cpuTask);

protected:
#ifdef DEBUG
	DRMultithreadLogger& mSpeedLog;
#endif
			
private: 
	DRTaskPtr mWaitingTask;
	DRCPUScheduler& mParent;

};

#endif //__DR_UNIVERSUM_LIB_CONTROLLER_CPU_SCHEDULER_THREAD_H__
        