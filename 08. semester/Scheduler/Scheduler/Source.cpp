
#include <iostream>
#include <future>
#include <ctime>
#include <map>
#include <vector>
#include <algorithm>
#include "Stopwatch.hpp"
#include "du3sch.hpp"

//static std::size_t Count = 1000;
static std::size_t Count = 1000;

static std::size_t MaxNum = 10000000;

//static std::size_t CoreCount = 8;
static std::size_t CoreCount = 3;

typedef std::map<std::size_t, bool> PrimeMap;

bool IsPrime(std::size_t num)
{
	for (std::size_t i = 2; i < num; i++)
	{
		if (num % i == 0)
			return false;
	}
	return true;
}

template<typename SCHEDULER>
void TestScheduler(const char* msg, const PrimeMap& value, time_sec serialTime)
{
	ticks_t start = now();
	SCHEDULER scheduler(CoreCount);
	std::map<std::size_t, std::size_t> tasks;
	for (std::size_t i = 0; i < Count; i++)
	{
		tasks[i] = scheduler.add_task([i]() -> bool { return IsPrime(i); });
		tasks[MaxNum - i] = scheduler.add_task([i]() -> bool { return IsPrime(MaxNum - i); });
	}

	for (std::map<std::size_t, std::size_t>::iterator i = tasks.begin(); i != tasks.end(); ++i)
	{
		if (value.at(i->first) != scheduler.get_task_result(i->second))
			throw 0;
	}

	time_sec parallelTime = ticks_to_time(now() - start);
	std::cout << msg << " time = " << parallelTime << " [s]  -  Serial time portion = " << parallelTime / (serialTime / 100.0) << " [%]" << std::endl;
	std::cout << msg << " calculation correct" << std::endl;
}

struct calc
{
	float value_;
	double operator()(void)
	{
		float res = 1;
		for (int i = 1; i < value_; i++)
			res = std::sqrt(res * i) + res * res / 0.33;
		return res;
	}
	calc(float val) : value_(val) { }
};
struct job
{
	void operator()(void)
	{
		std::cout << "void operator()" << std::endl;
	}
};

int main()
{
	//ticks_t start = now();
	PrimeMap value;
	for (std::size_t i = 0; i < Count; i++)
	{
		value[i] = IsPrime(i);
		value[MaxNum - i] = IsPrime(MaxNum - i);
	}
	//time_sec serialTime = ticks_to_time(now() - start);
	//std::cout << "Serial time = " << serialTime << std::endl;

	//TestScheduler<Scheduler<bool, std::function<bool(void)> > >("Scheduler", value, serialTime);

	/*Scheduler<float, calc> sch(2);
	ticks_t start = now();

	size_t ids[4];

	ids[0] = sch.add_task(calc(100));
	ids[1] = sch.add_task(calc(10000000));
	ids[2] = sch.add_task(calc(100));
	ids[3] = sch.add_task(calc(10000000));
	
	while (true)
	{
		bool all_true = true;
			 
		for (size_t i = 0; i < 4; i++)
			all_true &= sch.is_task_ready(ids[i]);
		if (all_true)
			break;
	}
	
	for (size_t i = 0; i < 4; i++)
		sch.get_task_result(ids[i]);

	time_sec total = ticks_to_time(now() - start);
	std::cout << "Total time = " << total << std::endl;

	Scheduler<void, job> sch2(2);
	for (int i = 0; i < 10; i++)
		sch2.add_task(job());*/

	system("pause");
	return 0;
}