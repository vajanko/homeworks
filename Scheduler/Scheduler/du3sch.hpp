// DU3sch.hpp
// Ondrej Kov·Ë NPRG051 2013/2014

#pragma once

#include <memory>
#include <future>
#include <list>
#include <vector>

enum class task_state
{
	waiting,
	running,
	ready
};
template<typename T, typename TASK>
class task_info
{
private:
	typedef std::unique_ptr<std::future<T>> future_ptr;

	task_state state_;
	TASK &task_;
	T result_;
	std::future<T> op_;

public:
	void run()
	{
		// TODO: check and run
		state_ = task_state::running;
		//op_ = std::async(task_);
	}
	T get_result()
	{
		// TODO: check and run
		if (state_ == task_state::waiting)
			result_ = task_();		// synchronous run
		else if (state_ == task_state::running)
			result_ = op_.get();	// asynchronous run (just wait for the thread)
			
		// at this point task must be already ready
		state_ = task_state::ready;
		return result_;
	}

	task_info(TASK &&task) : task_(task), state_(task_state::waiting), 
		result_(), op_() {}
};

template<typename T, typename TASK>
class worker
{
public:
	typedef task_info<T, TASK> job_type;
	typedef std::shared_ptr<job_type> job_ptr;
private:
	typedef std::unique_ptr<std::thread> thread_ptr;

	thread_ptr thread_;		// thread on which current worker is running
	bool exit_;				// true if worker should exit
	
	// this could be a class, but for simplification
	std::list<job_ptr> jobs_;	// a list of jobs to be executed
	std::mutex jobs_mutex_;		// protects the jobs_ queue

	// thread safe
	job_ptr pop()
	{
		job_ptr job;
		{
			std::unique_lock<std::mutex> lock(jobs_mutex_);

			job = jobs_.front();
			jobs_.pop_front();
		}
		return job;
	}
	// thread safe
	void push(job_ptr job)
	{
		std::lock_guard<std::mutex> lock(jobs_mutex_);
		jobs_.push_back(job);

		//queuePending.notify_one();
	}

	void run()
	{
		while (!exit_)
		{
			// take a job from queue
			job_ptr job = pop();

			// execute the job
			//job->operator()();
			//(*job)();
		}
	}
public:
	void add_task()
	{
		
	}

	worker() : thread_(new std::thread(std::bind(&worker::run, this))),
		exit_(false) { }

	// no copying
	worker &operator=(const worker &w) = delete;
	worker(const worker &w) = delete;

};

template<typename T, typename TASK>
class Scheduler
{
public:
	typedef std::shared_ptr<task_info<T, TASK>> task_info_ptr;
	typedef worker<T, TASK> worker_type;
	typedef std::unique_ptr<worker_type> worker_ptr;

private:
	std::vector<worker_ptr> workers_;
	std::vector<task_info_ptr> task_infos_;

	

public:
	std::size_t add_task(TASK &&task)
	{

	}

	bool is_task_ready(std::size_t index)
	{
		return false;
	}

	T get_task_result(std::size_t index)
	{
	}

	Scheduler(std::size_t core_count) //: workers_(core_count)
	{
		// another possibility
		for (std::size_t i = 0; i < core_count; ++i)
		{
			workers_.emplace_back(worker_ptr(new worker_type()));
		}
	}

	~Scheduler()
	{
	}
};

