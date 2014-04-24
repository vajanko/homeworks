#pragma once

#include<vector>

// optional design

template<typename T, typename TASK>
class core
{
private:
	// TODO:
	//TASK[] queue;

public:
	void add_task(TASK &&task, std::size_t index)
	{

	}
	T get_task_result(std::size_t index)
	{
		return T();
	}

	core() { }
};

template<typename T, typename TASK>
class Scheduler
{
public:
	typedef core<T, TASK> core_type;
private:
	std::size_t last_index_;
	std::vector<core_type> cores_;
	typedef typename std::vector<core_type>::const_iterator core_ptr;

	core_ptr get_task_core(std::size_t index)
	{
		return cores_.end();
	}

public:
	std::size_t add_task(TASK &&task)
	{
		++last_index_;
		// add task to a core
		cores_.front().add_task(std::forward<TASK>(task), last_index_);

		return last_index_;
	}

	bool is_task_ready(std::size_t index)
	{
		return false;
	}

	T get_task_result(std::size_t index)
	{
		return (get_task_core(index))->get_task_result(index);
	}

	Scheduler(std::size_t core_count) : last_index_(0), cores_(core_count)
	{
	}

	~Scheduler()
	{
	}
};

