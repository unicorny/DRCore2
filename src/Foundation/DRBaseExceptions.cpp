#include "DRCore2/Foundation/DRBaseExceptions.h"

DRCoreBaseException::DRCoreBaseException(const char* what) noexcept
	: std::runtime_error(what)
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