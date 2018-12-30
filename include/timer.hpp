//
//	Basic Timer classes
//
//
#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

class Timer
{
public:
	typedef std::chrono::high_resolution_clock clock;

	Timer() : _ts(clock::now())
	{}

	void start()
	{
		_ts = clock::now();
	}

	long long get_elapsed_millis() const
	{
		return std::chrono::duration_cast<std::chrono::milliseconds> (clock::now() - _ts).count();
	}

	clock::time_point _ts;

};

#ifdef WIN32

#include <Windows.h>

class TimerHigh
{
public:

	LARGE_INTEGER _ts;
	LARGE_INTEGER _freq;

	TimerHigh() 
	{}

	void start()
	{
		QueryPerformanceFrequency(&_freq);
		QueryPerformanceCounter(&_ts);
	}

	float get_elapsed_millis() const
	{
		LARGE_INTEGER endts;
		QueryPerformanceCounter(&endts);
		LARGE_INTEGER elapsed;
		elapsed.QuadPart = endts.QuadPart - _ts.QuadPart;

		elapsed.QuadPart *= 1000000;
		elapsed.QuadPart /= _freq.QuadPart;


		return 0.001f*(float)elapsed.QuadPart;
	}


};






#endif

#endif