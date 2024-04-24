#include "DRCore2/Utils/DRProfiler.h"

#include <sstream>

DRProfiler::DRProfiler()
{
	reset();
}

DRProfiler::DRProfiler(const DRProfiler& copy)
{
	mStartTick = copy.mStartTick;
}

DRProfiler::~DRProfiler()
{

}

double DRProfiler::millis() const
{
	auto current = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> fp_ms = current - mStartTick;
	return fp_ms.count();
	
}

double DRProfiler::micros() const
{
	auto current = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::micro> fp_micros = current - mStartTick;
	return fp_micros.count();
	//auto diff = current - mStartTick;
	//return diff.count() / 1000.0;
}

double DRProfiler::nanos() const
{
	auto current = std::chrono::high_resolution_clock::now();
	return static_cast<double>((current - mStartTick).count());
}

double DRProfiler::seconds() const
{
	auto current = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> fp_ms = current - mStartTick;
	return fp_ms.count() / 1000.0;
}

std::string DRProfiler::string() const
{
	auto current = std::chrono::high_resolution_clock::now();
	auto diff = current - mStartTick;
	std::stringstream ss; 
	ss.precision(3);

	if (diff < std::chrono::microseconds{1}) {
		ss << diff.count() << " nano s";
	} else if (diff < std::chrono::milliseconds{ 1 }) {
		ss << (double)diff.count() / 1000.0 << " micro s";
	} else if (diff < std::chrono::seconds{ 1 }) {
		ss << (double)std::chrono::duration_cast<std::chrono::microseconds>(diff).count() / 1000.0 << " ms";
	} else {
		ss << (double)std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() / 1000.0 << " s";
	}

	return ss.str();

}