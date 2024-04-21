#include "Core2Main.h"

DRCoreBaseException::DRCoreBaseException(const char* what) noexcept
	: std::exception(what)
{

}

DRCoreBaseException::~DRCoreBaseException()
{

}

std::string DRCoreBaseException::toString()
{
	return what();
}

std::string DRMemoryException::toString()
{
	std::string result = what();
	if (mSize) {
		result += ", size: " + std::to_string(mSize);
	}
	return result;
}