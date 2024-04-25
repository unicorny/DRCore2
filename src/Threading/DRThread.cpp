//#include "lib/Thread.h"
//#include "UniversumLib.h"
#include "DRCore2/DRTypes.h"
#include "DRCore2/Threading/DRThread.h"
#include "DRCore2/Foundation/DRBaseExceptions.h"
#include "DRCore2/Threading/DRMultithreadLogger.h"

DRThread::DRThread(const char* threadName)
    : mThread(nullptr), mExitCalled(false), mThreadName(threadName)
{
} 

void DRThread::init()
{
	mThread = new std::thread(&DRThread::run, this);	
}

void DRThread::exit()
{
	{
		std::unique_lock _lock(mMutex);
		//Post Exit to Thread
		mExitCalled = true;
		condSignal();
	}
	if (mThread)
	{
		mThread->join();
		DR_SAVE_DELETE(mThread);
	}
}

DRThread::~DRThread()
{
	exit();
}

void DRThread::condSignal()
{
	mCondition.notify_one();
}

void DRThread::run()
{
	while (true) {
		try {
			if (mExitCalled) return;
			// Lock work mutex
			std::unique_lock _lock(mMutex);
			try {
				mCondition.wait(_lock);
				if (mExitCalled) {
					return;
				}
				int ret = ThreadFunction();
				if (ret) {
					DRLog.writeToLog("%s Thread returned with: %d", mThreadName.data(), ret);
					LOG_ERROR_VOID("error running thread function, exit thread");
				}
			}
			catch (DRCoreBaseException& e) {
				DRLog.writeToLog("%s Thread throw a DRCoreException: %s", mThreadName.data(), e.toString().data());
				LOG_ERROR_VOID("thread throw an uncatched DRCoreException exception, exit thread");
			}
			catch (std::exception& e) {
				DRLog.writeToLog("%s Thread throw a exception: %s", mThreadName.data(), e.what());
				LOG_ERROR_VOID("thread throw an uncatched exception, exit thread");
			}

		} catch (std::system_error& e) {
			DRLog.writeToLog("%s Thread locking throw exception: %s", mThreadName.data(), e.what());
			LOG_ERROR_VOID("thread locking throw an uncatched exception, exit thread");
		}
	}
     
}