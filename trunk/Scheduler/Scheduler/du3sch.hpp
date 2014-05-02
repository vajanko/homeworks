// DU3sch.hpp
// Ondrej Kov·Ë NPRG051 2013/2014

#pragma once

#include <memory>
#include <future>
#include <list>
#include <vector>
#include <atomic>

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
	std::atomic<task_state> state_;		// use atomics here
	TASK task_;
	T result_;

	//std::once_flag once_flag_;
	//std::condition_variable result_cond_;
	//std::mutex result_mutex_;

	// not thread safe
	void run_internal()
	{
		// There are two possibilities of running a task: either it is normally executed by a worker
		// thread or the client application requests its result synchronously. In the second case
		// task is executed on the application main thread.
		// This method should be called only once - from worker thread or from main thread.
		task_state ex = task_state::waiting;
		if (state_.compare_exchange_strong(ex, task_state::running))
		{
			result_ = task_();
			std::lock_guard<std::mutex> lock(result_mutex_);
			result_cond_.notify_one();
			state_.store(task_state::ready);
		}

		/*state_ = task_state::running;
		result_ = task_();
		state_ = task_state::ready;
		
		std::lock_guard<std::mutex> lock(result_mutex_);
		result_cond_.notify_one();*/
		//std::call_once(once_flag_, std::bind(&task_info::run_internal, this));
	}

public:
	bool is_task_ready() { return state_ == task_state::ready; }
	void run_task()
	{
		// There are two possibilities of running a task: either it is normally executed by a worker
		// thread or the client application requests its result synchronously. In the second case
		// task is executed on the application main thread.
		// This method should be called only once - from worker thread or from main thread.
		task_state ex = task_state::waiting;
		if (state_.compare_exchange_strong(ex, task_state::running))
		{	// run task only if not running yet
			result_ = task_();
			state_ = task_state::ready;
			//std::lock_guard<std::mutex> lock(result_mutex_);
			//result_cond_.notify_one();
		}
	}
	T get_task_result()
	{
		run_task();
		// block until task becomes ready (blocking only if already running on worker thread)
		while (!is_task_ready())
			;

		//std::unique_lock<std::mutex> lock(result_mutex_);
		//result_cond_.wait(lock, std::bind(&task_info::is_task_ready, this));
		
		return result_;
	}
	
	task_info(TASK &&task) : task_(std::forward<TASK>(task)), state_(task_state::waiting), result_() 
		/*, once_flag_(), result_cond_(), result_mutex_()*/ { }
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
	std::condition_variable jobs_pending_;

	bool idle_;

	// thread safe
	job_ptr pop()
	{
		// can be blocked if no job are present
		job_ptr job;
		{
			std::unique_lock<std::mutex> lock(jobs_mutex_);
			idle_ = jobs_.empty();
			jobs_pending_.wait(lock, [&]() { return exit_ || !jobs_.empty(); });
			idle_ = false;

			if (exit_)
				return nullptr;

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
		jobs_pending_.notify_one();
	}

	void run()
	{
		while (!exit_)
		{
			// take a job from queue
			job_ptr job = pop();

			// run the job (if there is not an exit request)
			if (job != nullptr)
				job->run_task();
		}
	}
	
public:
	void add_job(job_ptr job) { push(job); }
	bool idle() { return idle_; }
	void work()
	{
		thread_ = thread_ptr(new std::thread(std::bind(&worker::run, this)));
	}

	worker() : exit_(false), idle_(true), thread_(nullptr), jobs_(), jobs_mutex_(), jobs_pending_() { }

	// no copying
	worker &operator=(const worker &w) = delete;
	worker(const worker &w) = delete;

	~worker()
	{	// exit the thread
		{
			std::lock_guard<std::mutex> lock(jobs_mutex_);
			exit_ = true;
			jobs_pending_.notify_one();
		}
		// wait for the last task to be finished
		thread_->join();
	}
};

template<typename T, typename TASK>
class Scheduler
{
public:
	typedef worker<T, TASK> worker_type;
	typedef std::unique_ptr<worker_type> worker_ptr;

	typedef typename worker_type::job_type job_type;
	typedef typename worker_type::job_ptr job_ptr;

private:
	std::vector<worker_ptr> workers_;
	std::vector<job_ptr> jobs_;

public:
	std::size_t add_task(TASK &&task)
	{
		// create job
		job_ptr job(new job_type(std::forward<TASK>(task)));
		jobs_.push_back(job);

		// assign to a worker (round robin)
		workers_[jobs_.size() % workers_.size()]->add_job(job);

		// index of currently added job
		return jobs_.size() - 1;
	}

	bool is_task_ready(std::size_t index)
	{
		return jobs_[index]->is_task_ready();
	}

	T get_task_result(std::size_t index)
	{
		return jobs_[index]->get_task_result();
	}

	Scheduler(std::size_t core_count)
	{
		for (std::size_t i = 0; i < core_count; ++i)
		{
			workers_.emplace_back(worker_ptr(new worker_type()));
			workers_[i]->work();
		}
	}

	~Scheduler() {}
};

