#include "DRCore2/Threading/DRFuzzyTimer.h"
#include "DRCore2/Foundation/DRLogger.h"
#include "DRCore2/Foundation/DRBaseExceptions.h"

using namespace std::chrono_literals;

DRFuzzyTimer::DRFuzzyTimer()
	: DRThread("FuzzyTimer"), mExit(false)
{
}

DRFuzzyTimer::~DRFuzzyTimer()
{
	std::lock_guard _lock(mMutex);
	mExit = true;
	mRegisteredAtTimer.clear();
}

void DRFuzzyTimer::init() 
{
	DRThread::init();
}
// -----------------------------------------------------------------------------------------------------

DRReturn DRFuzzyTimer::addTimer(std::string name, DRTimerCallback* callbackObject, std::chrono::duration<u64, std::milli> timeIntervall, int loopCount/* = -1*/)
{
	std::lock_guard _lock(mMutex);
	if (mExit) return DR_ERROR;

	// calculate point in the future on that the timer callback will be called at the earliest
	auto targetTimePoint = std::chrono::steady_clock::now();
	targetTimePoint += timeIntervall;
	mRegisteredAtTimer.insert(TIMER_TIMER_ENTRY(targetTimePoint, DRTimerEntry(callbackObject, timeIntervall, loopCount, name)));

	return DR_OK;
}

int DRFuzzyTimer::removeTimer(std::string name)
{
	if (!mMutex.try_lock_for(100ms)) {
		return -2;
	}
	// Assumes the calling thread already holds a non-shared lock (i.e., a lock acquired by lock, try_lock, try_lock_for, or try_lock_until) on m. 
	// The behavior is undefined if not so.
	std::unique_lock _lock(mMutex, std::adopt_lock);
	if (mExit) return -1;

	size_t eraseCount = 0;
				
	for (auto it = mRegisteredAtTimer.begin(); it != mRegisteredAtTimer.end();)
	{
		if (name == it->second.name)
		{
			it->second.callback = nullptr;
			it = mRegisteredAtTimer.erase(it);
			eraseCount++;
		}
		else {
			//printf("not removed: %s\n", it->second.name.data());
			it++;
		}
	}
	//printf("removed %d timer with name: %s\n", eraseCount, name.data());
	//printf("mRegisteredAtTimer size: %d\n", mRegisteredAtTimer.size());
	return eraseCount;
}

DRReturn DRFuzzyTimer::move()
{
	std::lock_guard _lock(mMutex);
	if (mExit) return DR_ERROR;
			
	auto it = mRegisteredAtTimer.begin();
	if (it == mRegisteredAtTimer.end()) return DR_OK;

	auto now = std::chrono::steady_clock::now();

	if (it->first <= now) {
		if (!it->second.callback) {
			DRLog.writeToLog("empty callback, name: %s", it->second.name.data());
		}
		else {
			//printf("[FuzzyTimer::move] about to call: %s\n", it->second.name.data());
			TimerReturn ret = TimerReturn::NOT_SET;
			try {
				ret = it->second.callback->callFromTimer();
			}
			catch (DRCoreBaseException& ex) {
				DRLog.writeToLog("DRCore2 Exception: %s", ex.toString());
				ret = TimerReturn::EXCEPTION;
			}
			catch (std::runtime_error& ex) {
				DRLog.writeToLog("std::runtime_error: %s", ex.what());
				ret = TimerReturn::EXCEPTION;
			}
			if (it->second.nextLoop() && ret == TimerReturn::GO_ON) {
				mRegisteredAtTimer.insert(TIMER_TIMER_ENTRY(now + it->second.timeIntervall, it->second));
			}

			if (ret == TimerReturn::REPORT_ERROR) {
				DRLog.writeToLog(
					"timer run report error: timer type: %s, timer name: %s",
					std::string(it->second.callback->getResourceType()), it->second.name);				
			}
			else if (ret == TimerReturn::EXCEPTION) {
				DRLog.writeToLog(
					"timer run throw a exception: timer type: %s, timer name: %s",
					std::string(it->second.callback->getResourceType()), it->second.name);
			}
		}
		mRegisteredAtTimer.erase(it);					
	}

	return DR_OK;
}

void DRFuzzyTimer::stop()
{
	std::lock_guard _lock(mMutex);
	mExit = true;
}
int DRFuzzyTimer::ThreadFunction()
{
	while (true) 
	{
		if (!move()) return 0;
		std::this_thread::sleep_for(MAGIC_NUMBER_TIMER_THREAD_SLEEP_BETWEEN_MOVE_CALLS_MILLISECONDS);
	}
	return 0;
}
