#include "Timer.h"

#include <Windows.h>

Timer::Timer()
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	m_Frequency = 1.0 / frequency.QuadPart;

	Reset();
}

void Timer::Reset()
{
	QueryPerformanceCounter(&m_Start);
}

float Timer::Elapsed()
{
	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);
	uint32_t cycles = current.QuadPart - m_Start.QuadPart;
	return (float)(cycles * m_Frequency);
}

float Timer::ElapsedMillis()
{
	return Elapsed() * 1000.0f;
}
