// DU3sch.hpp
// Ondrej Kov�� NPRG051 2013/2014

#pragma once

#include <memory>
#include <future>
#include <list>
#include <vector>
#include <atomic>

enum class task_state
{
	// task waiting to be executed
	waiting,
	// task is currently running
	running,
	// task result is ready
	ready
};

// Wrapper class for task, its result and current state. Manages access to task from 
// multiple threads.
template<typename T, typename TASK> class task_info
{
private:
	// current task state
	std::atomic<task_state> state_;
	// task operation (implements operator()(void))
	TASK task_;
	// task result as a value
	T result_;

public:
	// Gets value indicating whether task result is already ready (execution finished)
	bool is_task_ready() { return state_ == task_state::ready; }
	// Starts task if not running yet (thread safe)
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
			// here and in the previous condition are the only places where state_ variable is assigned
			state_ = task_state::ready;
		}
	}
	// Gets task result (thread safe). This method blocks if task is currently running
	T get_task_result()
	{
		// run task if not running yet
		run_task();

		// block until task becomes ready (blocking only if already running on worker thread)
		// we are waiting actively because requested task is currently being executed
		while (!is_task_ready())
			;

		return result_;
	}
	
	task_info(TASK &&task) : task_(std::forward<TASK>(task)), state_(task_state::waiting), result_() { }
};

template<typename T, typename TASK> class Scheduler;		// forward declaration

template<typename T, typename TASK>
class worker
{
public:
	typedef task_info<T, TASK> job_type;				// type of task wrapper
	typedef std::shared_ptr<job_type> job_ptr;			// pointer to task wrapper
private:
	typedef std::unique_ptr<std::thread> thread_ptr;	// pointer to worker thread

	// thread on which current worker is running
	thread_ptr thread_;
	// true if worker should exit
	bool exit_;

	// usually parallel access to queue would be implemented in a separate class, but
	// this one also need to access private fields

	// a list of jobs to be executed
	std::list<job_ptr> jobs_;
	// protects the jobs_ queue
	std::mutex jobs_mutex_;
	// notify about new jobs
	std::condition_variable jobs_pending_;

	Scheduler<T, TASK> &scheduler_;

	// retrieves next job from the queue or returns nullptr if exit is needed (thread safe)
	job_ptr pop()
	{
		// can be blocked if no job are present
		job_ptr job;
		{
			std::unique_lock<std::mutex> lock(jobs_mutex_);
			// waits until there are some jobs in the queue of exit signal was sent
			jobs_pending_.wait(lock, [&]() { return exit_ || !jobs_.empty(); });

			if (exit_)
				return nullptr;

			// get next job
			job = jobs_.front();
			jobs_.pop_front();
		}
		return job;
	}
	// inserts a new job into the queue and notifies about it (thread safe)
	void push(job_ptr job)
	{
		std::lock_guard<std::mutex> lock(jobs_mutex_);
		jobs_.push_back(job);
		jobs_pending_.notify_one();
	}

	// worker main function
	void run()
	{
		while (!exit_)
		{
			// take a job from queue
			job_ptr job = pop();
			if (exit_) break;

			// run the job (if there is not an exit request)
			if (job != nullptr)
				job->run_task();

			// notify scheduler if current worker is idle - load balancing
			if (jobs_.size() == 0)
				scheduler_.idle_notify(*this);
		}
	}
	
public:
	// Adds given job to worker queue to be executed
	void add_job(job_ptr job) { push(job); }
	// Gets number of jobs waiting to be executed
	std::size_t waiting_jobs() { return jobs_.size(); }

	// Divide your work with the other one. Give him half of the jobs
	void divide_work(worker &other)
	{
		// lock both queues at once
		std::unique_lock<std::mutex> lock1(jobs_mutex_, std::defer_lock);
		std::unique_lock<std::mutex> lock2(other.jobs_mutex_, std::defer_lock);
		std::lock(lock1, lock2);

		// take a half of one queue and put it to the other
		std::size_t count = (jobs_.size() + 1) / 2;
		for (std::size_t i = 0; i < count; ++i)
		{
			other.jobs_.push_back(jobs_.front());
			jobs_.pop_front();
		}

		lock1.unlock();
		lock2.unlock();
	}

	// Start worker thread
	void start()
	{
		thread_ = thread_ptr(new std::thread(std::bind(&worker::run, this)));
	}

	// Default constructor
	worker(Scheduler<T, TASK> &scheduler) : scheduler_(scheduler),
		exit_(false), thread_(nullptr), jobs_(), jobs_mutex_(), jobs_pending_() { }
	// no copying
	worker &operator=(const worker &w) = delete;
	worker(const worker &w) = delete;
	// Destructor
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

template<typename T, typename TASK> class scheduler_base
{
public:
	typedef worker<T, TASK> worker_type;
	typedef std::unique_ptr<worker_type> worker_ptr;

	typedef typename worker_type::job_type job_type;
	typedef typename worker_type::job_ptr job_ptr;

private:
	std::vector<worker_ptr> workers_;
	std::vector<job_ptr> jobs_;
	std::size_t current_ = 0;

protected:
	T get_task_result_internal(std::size_t index) 
	{
		return jobs_[index]->get_task_result(); 
	}
	std::size_t add_task_internal(TASK &&task)
	{
		// create job
		job_ptr job(new job_type(std::forward<TASK>(task)));
		jobs_.push_back(job);

		// assign to a worker (round robin)
		workers_[jobs_.size() % workers_.size()]->add_job(job);

		// index of currently added job
		return jobs_.size() - 1;
	}

public:
	// TODO: make friend
	void idle_notify(worker_type &worker)
	{
		std::size_t max_jobs = 0;
		std::size_t max_index = current_;

		// do not search all
		for (std::size_t i = 0; i < workers_.size(); ++i)
		{
			current_ = (current_ + 1) % workers_.size();
			if (workers_[current_]->waiting_jobs() > max_jobs)
			{
				max_jobs = workers_[current_]->waiting_jobs();
				max_index = current_;
			}
		}

		if (workers_[max_index]->waiting_jobs() > 0)
			workers_[max_index]->divide_work(worker);
	}


	bool is_task_ready(std::size_t index)
	{
		return jobs_[index]->is_task_ready();
	}

	scheduler_base(std::size_t core_count)
	{
		for (std::size_t i = 0; i < core_count; ++i)
		{
			workers_.emplace_back(worker_ptr(new worker_type(*this)));
			workers_[i]->start();
		}
	}
	virtual ~scheduler_base() { }
};

template<typename T, typename TASK> class Scheduler : public scheduler_base<T, TASK>
{
private:
	typedef scheduler_base<T, TASK> base_type;

public:
	std::size_t add_task(TASK &&task)
	{
		return add_task_internal(std::forward<TASK>(task));
	}
	T get_task_result(std::size_t index)
	{
		return get_task_result_internal(index);
	}

	explicit Scheduler(std::size_t core_count) : base_type(core_count) { }
	virtual ~Scheduler() { }
};
template<typename TASK> class void_wrapper
{
private:
	TASK task_;
public:
	int operator()()
	{
		task_();
		return 0;
	}
	void_wrapper(TASK &&task) : task_(std::forward(task)) { }
};
template<typename TASK> class Scheduler<void, TASK> : public scheduler_base<int, void_wrapper<TASK>>
{
private:
	typedef scheduler_base<int, void_wrapper<TASK>> base_type;

public:
	std::size_t add_task(TASK &&task)
	{
		return add_task_internal(void_wrapper(std::forward(task)));
	}
	// don't now if this is even necessary ?? could be used for explicitly running specified task
	void get_task_result(std::size_t index)
	{
		get_task_result_internal(index);
	}

	explicit Scheduler(std::size_t core_count) : base_type(core_count) { }
	virtual ~Scheduler() { }
};

//template<typename T, typename TASK>
//class Scheduler
//{
//public:
//	typedef worker<T, TASK> worker_type;
//	typedef std::unique_ptr<worker_type> worker_ptr;
//
//	typedef typename worker_type::job_type job_type;
//	typedef typename worker_type::job_ptr job_ptr;
//
//private:
//	std::vector<worker_ptr> workers_;
//	std::vector<job_ptr> jobs_;
//	std::size_t current_ = 0;
//
//public:
//	// TODO: make friend
//	void idle_notify(worker_type &worker)
//	{
//		std::size_t max_jobs = 0;
//		std::size_t max_index = current_;
//
//		// do not search all
//		for (std::size_t i = 0; i < workers_.size(); ++i)
//		{
//			current_ = (current_ + 1) % workers_.size();
//			if (workers_[current_]->waiting_jobs() > max_jobs)
//			{
//				max_jobs = workers_[current_]->waiting_jobs();
//				max_index = current_;
//			}
//		}
//
//		if (workers_[max_index]->waiting_jobs() > 0)
//			workers_[max_index]->divide_work(worker);
//	}
//
//	std::size_t add_task(TASK &&task)
//	{
//		// create job
//		job_ptr job(new job_type(std::forward<TASK>(task)));
//		jobs_.push_back(job);
//
//		// assign to a worker (round robin)
//		workers_[jobs_.size() % workers_.size()]->add_job(job);
//
//		// index of currently added job
//		return jobs_.size() - 1;
//	}
//	bool is_task_ready(std::size_t index)
//	{
//		return jobs_[index]->is_task_ready();
//	}
//
//	T get_task_result(std::size_t index)
//	{
//		return jobs_[index]->get_task_result();
//	}
//
//	Scheduler(std::size_t core_count)
//	{
//		for (std::size_t i = 0; i < core_count; ++i)
//		{
//			workers_.emplace_back(worker_ptr(new worker_type(*this)));
//			workers_[i]->start();
//		}
//	}
//	virtual ~Scheduler() {}
//};

