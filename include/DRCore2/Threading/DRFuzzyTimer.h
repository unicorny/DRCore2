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
 //#include "UniversumLib.h"

#ifndef __UNIVERSUM_LIB_LIB_TIMER__
#define __UNIVERSUM_LIB_LIB_TIMER__

#include "DRThread.h"
#include "DRCore2/DRTypes.h"

#include <string>
#include <map>
#include <mutex>

// refactored from my game engine for gradido
// @date: 04.12.21
// @author: einhornimmond

// MAGIC NUMBER: The timer has a list of all timer callback, sorted which should be called when
// on every move call it will check if the current callback time is reached and if so, it will be called
// the time between move calls depends on needed accuracy and timer calls per seconds
#define MAGIC_NUMBER_TIMER_THREAD_SLEEP_BETWEEN_MOVE_CALLS_MILLISECONDS 100ms


enum class TimerReturn {
	GO_ON = 0,
	REMOVE_ME = 1,
	REPORT_ERROR = 2,
	EXCEPTION = 3,
	NOT_SET = 4
};
#if (_MSC_VER >= 1200 && _MSC_VER < 1310)
//enum TimerReturn;
#endif

class DRTimerCallback
{
public:
	virtual TimerReturn callFromTimer() = 0;
	virtual const char* getTimerCallbackType() const { return "TimerCallback"; };
};

class DRCORE2_EXPORT DRFuzzyTimer: public DRThread
{
public:
	DRFuzzyTimer();
	~DRFuzzyTimer();

	void init();

	/*!
		add timer callback object
		\param timeIntervall intervall in milliseconds
		\param loopCount 0 for one call, -1 for loop
	*/
	DRReturn addTimer(std::string name, DRTimerCallback* callbackObject, std::chrono::duration<u64, std::milli> timeIntervall, int loopCount = -1);

	/*!
		\brief remove all timer with name
		function is not really fast, because all appended timerCallback will be checked

		\return removed timer count, or -1 if timer was stopped, -2 if try locked timed out
	*/
	int removeTimer(std::string name);

	/*
		\brief update timer map, maybe call timer... (only one per frame)
	*/
	DRReturn move();

	void stop();

	int ThreadFunction();
private:
	struct DRTimerEntry {
		// functions
		DRTimerEntry(DRTimerCallback* _callback, std::chrono::duration<u64, std::milli> _timeIntervall, int _loopCount, std::string _name)
			: callback(_callback), timeIntervall(_timeIntervall), initLoopCount(_loopCount), currentLoopCount(0), name(_name) {}
		~DRTimerEntry() {}
		// \return true if we can run once again
		bool nextLoop() {
			currentLoopCount++;
			if (initLoopCount < 0 || initLoopCount - 1 > currentLoopCount) return true;
			return false;
		}

		// variables
		DRTimerCallback* callback;
		std::chrono::duration<u64, std::milli> timeIntervall;
		int initLoopCount;
		int currentLoopCount;
		std::string name;
	};
	// int key = time since programm start to run
	std::multimap<std::chrono::time_point<std::chrono::steady_clock>, DRTimerEntry> mRegisteredAtTimer;
	typedef std::pair<std::chrono::time_point<std::chrono::steady_clock>, DRTimerEntry> TIMER_TIMER_ENTRY;
	std::timed_mutex mMutex;
	bool		mExit;
};


#endif //__UNIVERSUM_LIB_LIB_TIMER__