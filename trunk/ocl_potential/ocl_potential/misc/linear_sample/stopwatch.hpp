#ifndef _STOPWATCH_HPP
#define _STOPWATCH_HPP

#include <tbb/tick_count.h>

/*
 * Implementation of wall stopwatch based on TBB tick_count timers.
 */
class Stopwatch
{
private:
	tbb::tick_count mStartTime;
	tbb::tick_count::interval_t mLastInterval;
	bool mTiming;

public:
	/*
	 * Create new stopwatch. The stopwatch are not running when created.
	 */
	Stopwatch() : mTiming(false) { }

	/*
	 * Create new stopwatch. If start is true, the stapwatch are started immediatelly.
	 */
	Stopwatch(bool start)
	{
		if (start) this->start();
	}


	/*
	 * Start the stopwatch. If the stopwatch are already timing, they are reset.
	 */
	void start()
	{
		mTiming = true;
		mStartTime = tbb::tick_count::now();
	}


	/*
	 * Stop the stopwatch. Multiple invocation has no effect.
	 */
	void stop()
	{
		if (mTiming == false) return;
		mTiming = false;
		mLastInterval = tbb::tick_count::now() - mStartTime;
	}


	/*
	 * Return elapsed time in seconds since start method has been called.
	 * If the stopwatch is not timing, last measured interval is returned.
	 */
	double getSeconds()
	{
		if (mTiming)
			return (tbb::tick_count::now() - mStartTime).seconds();
		else
			return mLastInterval.seconds();
	}

	/*
	 * Return elapsed time in miliseconds since start method has been called.
	 * If the stopwatch is not timing, last measured interval is returned.
	 */
	double getMiliseconds()
	{
		return getSeconds() * 1000.0;
	}
};

#endif
