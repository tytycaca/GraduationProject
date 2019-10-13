////////////////////////////////////////////////////////////////////////////////
// Filename: deltatimeclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "deltatimeclass.h"


DeltaTimeClass::DeltaTimeClass()
{
	countsPerSecond = 0.0;
	CounterStart = 0;

	frameTimeOld = 0;
}


DeltaTimeClass::DeltaTimeClass(const DeltaTimeClass& other)
{
}


DeltaTimeClass::~DeltaTimeClass()
{
}


void DeltaTimeClass::StartTimer()
{
	LARGE_INTEGER frequencyCount;
	QueryPerformanceFrequency(&frequencyCount);

	countsPerSecond = double(frequencyCount.QuadPart);

	QueryPerformanceCounter(&frequencyCount);
	CounterStart = frequencyCount.QuadPart;
}


double DeltaTimeClass::GetTime()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	return double(currentTime.QuadPart - CounterStart) / countsPerSecond;
}


double DeltaTimeClass::GetFrameTime()
{
	LARGE_INTEGER currentTime;
	__int64 tickCount;
	QueryPerformanceCounter(&currentTime);

	tickCount = currentTime.QuadPart - frameTimeOld;
	frameTimeOld = currentTime.QuadPart;

	if (tickCount < 0.0f)
		tickCount = 0.0f;

	return float(tickCount) / countsPerSecond;
}