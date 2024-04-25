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

/**
 * @Author Dario Rekowski
 * 
 * @Date 13.08.12
 * 
 * @Desc Class for easy handling threading
 */
 
#ifndef __DR_UNIVERSUM_LIB_THREAD__
#define __DR_UNIVERSUM_LIB_THREAD__

#include "DRCore2/export.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <stdexcept>

   
class DRCORE2_EXPORT DRThread
{
public:
    DRThread(const char* threadName);
    virtual ~DRThread();

    // signal data chance, will continue thread, if he is paused
    void condSignal();

	void init();
    void exit();

	void run();

    inline void setName(const char* threadName) { mThreadName = threadName; }
    inline const std::string& getName() const { return mThreadName; }
protected:
    //! \brief will be called every time from thread, when condSignal was called
    //! will be called from thread with locked working mutex,<br>
    //! mutex will be unlock after calling this function
    //! \return if return isn't 0, thread will exit
    virtual int ThreadFunction() = 0;

private: 
    std::mutex              mMutex;
    std::thread*            mThread;
    std::condition_variable mCondition;
    bool                    mExitCalled;
    std::string             mThreadName;
};



#endif //__DR_UNIVERSUM_LIB_THREAD__
