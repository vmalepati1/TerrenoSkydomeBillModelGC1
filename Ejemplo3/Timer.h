#pragma once

#include <stdint.h>
#include <Windows.h>

struct Members;

class Timer
{
private:
	LARGE_INTEGER m_Start;
	double m_Frequency;
public:
	// Creates and starts timer
	Timer();
	// Resets and restarts timer
	virtual void Reset();
	// Returns time in seconds
	virtual float Elapsed();
	// Returns time in milliseconds
	virtual float ElapsedMillis();
};