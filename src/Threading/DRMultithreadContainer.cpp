#include "DRCore2/Threading/DRMultithreadContainer.h"

#include <thread>
#include <chrono>

std::unique_lock<std::recursive_mutex> DRMultithreadContainer::getUniqueLock(const char* stackDetails/* = nullptr*/)
{
	using namespace std::chrono_literals;
	const static char* functionName = "MultithreadContainer::lock";
			
	uint8_t exitCounter = 5;
	while (exitCounter > 0) {
		if (mWorkMutex.try_lock()) break;
		std::this_thread::sleep_for(100ms);
		exitCounter--;
	}
	if (exitCounter > 0) {
		if (stackDetails) {
			mLastSucceededLock = stackDetails;
		}
		return std::unique_lock(mWorkMutex, std::adopt_lock);
	}
	else {
		throw DRMultithreadContainerLockTimeoutException(mLastSucceededLock.data(), stackDetails);
	}			
}

// ************ Exception **************
DRMultithreadContainerLockTimeoutException::DRMultithreadContainerLockTimeoutException(const char* lastSucceedLock, const char* stackDetails)
	: DRCoreBaseException("DRMultithreadContainer lock timeout")
{
	if (lastSucceedLock) {
		mLastSucceedLock = lastSucceedLock;
	}
	if (stackDetails) {
		mStackDetails = stackDetails;
	}
}

std::string DRMultithreadContainerLockTimeoutException::toString() const
{
	if (mLastSucceedLock.size() || mStackDetails.size()) {
		size_t resultSize = 0;
		if (mLastSucceedLock.size()) {
			resultSize += mLastSucceedLock.size() + 14;
		}
		if (mStackDetails.size()) {
			resultSize += mStackDetails.size() + 10;
		}
		std::string result;
		result.reserve(resultSize);
		if (mLastSucceedLock.size()) {
			result += "last succeed: " + mLastSucceedLock;
		}
		if (mStackDetails.size()) {
			result += ", stack: " + mStackDetails;
		}
		return result;
	}
	else {
		return what();
	}			
}
