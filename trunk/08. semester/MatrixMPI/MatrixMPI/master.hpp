#ifndef master_hdr
#define master_hdr

#include<mpi.h>

#include<string>
#include<fstream>
#include<vector>
#include<memory>
#include "matrix.hpp"
#include "worker.hpp"

#ifdef WIN32
#include<future>
void sleep(int seconds) {
	std::this_thread::sleep_for(std::chrono::seconds(seconds));
}
#else
#include <unistd.h>
#endif

// hold by master
class worker_task
{
private:
	// worker mpi ID
	int worker_id;
	int master_id;
	// true if worker is currently working
	bool working;
	// sizes of matrix the worker is currently working on
	size_t my_dim1, my_dim2, my_dim3;
	// position of the chunk matrix in the result
	size_t top, left;
	MPI_Request send_req;
	MPI_Request recv_req;
	void *req_buff;
	size_t req_buff_size;

public:
	bool is_idle() { return !working; }
	bool is_working() { return working; }
	void send(matrix a, matrix b, size_t top, size_t left, size_t chunk_dim1, size_t chunk_dim2, size_t chunk_dim3)
	{
		working = true;
		this->top = top;
		this->left = left;
		my_dim1 = chunk_dim1;
		my_dim2 = chunk_dim2;
		my_dim3 = chunk_dim3;

		int pos = 0;
		MPI_Pack(&my_dim1, 1, MPI_UNSIGNED_LONG, req_buff, req_buff_size, &pos, MPI_COMM_WORLD);
		MPI_Pack(&my_dim2, 1, MPI_UNSIGNED_LONG, req_buff, req_buff_size, &pos, MPI_COMM_WORLD);
		MPI_Pack(&my_dim3, 1, MPI_UNSIGNED_LONG, req_buff, req_buff_size, &pos, MPI_COMM_WORLD);
		MPI_Pack(a, my_dim1 * my_dim2, MPI_FLOAT, req_buff, req_buff_size, &pos, MPI_COMM_WORLD);
		MPI_Pack(b, my_dim2 * my_dim3, MPI_FLOAT, req_buff, req_buff_size, &pos, MPI_COMM_WORLD);

		MPI_Isend(req_buff, pos, MPI_PACKED, worker_id, TAG, MPI_COMM_WORLD, &send_req);

		// start receiving imediatelly
		MPI_Irecv(req_buff, my_dim1 * my_dim3, MPI_FLOAT, worker_id, TAG, MPI_COMM_WORLD, &recv_req);
	}
	bool receive_ready()
	{
		MPI_Status status;
		int flag;
		MPI_Test(&recv_req, &flag, &status);

		return flag != 0;
	}
	void receive(matrix &res, size_t &top, size_t &left, size_t &rows, size_t &cols)
	{
		res = (matrix)req_buff;
		top = this->top;
		left = this->left;
		rows = this->my_dim1;
		cols = this->my_dim3;

		working = false;
	}
	void kill()
	{
		//std::cout << "sending poison pill" << std::endl;

		size_t zero = 0;

		int pos = 0;
		MPI_Pack(&zero, 1, MPI_UNSIGNED_LONG, req_buff, req_buff_size, &pos, MPI_COMM_WORLD);
		MPI_Pack(&zero, 1, MPI_UNSIGNED_LONG, req_buff, req_buff_size, &pos, MPI_COMM_WORLD);
		MPI_Pack(&zero, 1, MPI_UNSIGNED_LONG, req_buff, req_buff_size, &pos, MPI_COMM_WORLD);

		MPI_Send(req_buff, pos, MPI_PACKED, worker_id, TAG, MPI_COMM_WORLD);
		working = false;

		//std::cout << "poison pill sent" << std::endl;
	}
	worker_task(int worker_id, size_t max_dim1, size_t max_dim2, size_t max_dim3) :
		worker_id(worker_id),
		working(false),
		req_buff_size(sizeof(unsigned long)* 3 + sizeof(float)* (max_dim1 * max_dim2 + max_dim2 * max_dim3)),
		req_buff(new char[sizeof(unsigned long)* 3 + sizeof(float)* (max_dim1 * max_dim2 + max_dim2 * max_dim3)])
	{}
	~worker_task()
	{
		if (req_buff != NULL)
			delete[] (char *)req_buff;
	}
};

class master
{
private:
	// master MPI id
	int master_id;
	// sizes of the multiplied matrices
	size_t dim1, dim2, dim3;
	// sizes of matrices multiplied by workers (max possible can be smaller)
	size_t chunk_dim1, chunk_dim2, chunk_dim3;
	// buffers - currently procesing matrices - loaded from files
	matrix a, b;

	// master working data
	matrix my_res;

	// result matrix contains collected data from all workers
	matrix result;
	// total number of workers
	size_t group_size;
	// ID of the current worker
	int worker_id;
	std::vector<worker_task *> workers;

	std::ifstream file1, file2;
	const char *output;

public:
	void init()
	{
		matrix_read_size(file1, dim1, dim2);
		matrix_read_size(file2, dim2, dim3);

		/*std::cout << "total dimensions" << std::endl;
		std::cout << "m1 = [" << dim1 << ", " << dim2 << "]" << std::endl;
		std::cout << "m2 = [" << dim2 << ", " << dim3 << "]" << std::endl;*/

		chunk_dim1 = 1;
		chunk_dim2 = dim1; // or bigger if dim1 is too small
		chunk_dim3 = dim3;

		/*std::cout << "chunk dimensions" << std::endl;
		std::cout << "m1 = [" << chunk_dim1 << ", " << chunk_dim2 << "]" << std::endl;
		std::cout << "m2 = [" << chunk_dim2 << ", " << chunk_dim3 << "]" << std::endl;*/

		a = matrix_alloc(chunk_dim1, chunk_dim2);
		b = matrix_alloc(chunk_dim2, chunk_dim3);
		result = matrix_alloc(dim1, dim3, 0.0);
		my_res = matrix_alloc(chunk_dim1, chunk_dim3);

		for (size_t i = 0; i < group_size; ++i)
			workers.push_back(new worker_task(i, chunk_dim1, chunk_dim2, chunk_dim3));

		// broadcast max size of chunk
		int max_chunk[3];
		/*max_chunk[0] = chunk_dim1;
		max_chunk[1] = chunk_dim2;
		max_chunk[2] = chunk_dim3;*/
		max_chunk[0] = dim1;
		max_chunk[1] = dim2;
		max_chunk[2] = dim3;
		MPI_Bcast(max_chunk, 3, MPI_INT, master_id, MPI_COMM_WORLD);
	}
	int get_idle_worker()
	{
		for (size_t w = 0; w < group_size; ++w, worker_id = (worker_id + 1) % group_size)
			if (workers[worker_id]->is_idle() && worker_id != master_id)
				break;
		// return either ID of the next idle worker or ROOT if none of them is idle
		return workers[worker_id]->is_idle() ? worker_id : master_id;
	}
	void receive_result(int worker_id)
	{
		matrix chunk_res;
		size_t top, left, rows, cols;
		workers[worker_id]->receive(chunk_res, top, left, rows, cols);

		// dim3 == width of the result matrix
		matrix_add(result, top, left, dim3, chunk_res, rows, cols);
	}
	void check_workers_status()
	{
		for (size_t w = 0; w < group_size; ++w, worker_id = (worker_id + 1) % group_size)
		{
			if (workers[worker_id]->is_working() && workers[worker_id]->receive_ready())
			{
				receive_result(worker_id);
			}
		}
	}
	void process_task(size_t top, size_t left, size_t ch_dim1, size_t ch_dim2, size_t ch_dim3)
	{
		int id = get_idle_worker();
		if (id != master_id)
		{	// we have found an idle worker
			workers[id]->send(a, b, top, left, ch_dim1, ch_dim2, ch_dim3);
		}
		else
		{	// all workers are working currently
			check_workers_status();
			id = get_idle_worker();

			//if (id == master_id)
			//{
			//	for (size_t i = 0; i < chunk_dim1; ++i)
			//	{
			//		for (size_t j = 0; j < chunk_dim3; ++j)
			//		{
			//			int sum = 0;

			//			for (size_t k = 0; k < chunk_dim2; ++k)
			//			{
			//				sum += (int)a[i * chunk_dim2 + k] * (int)b[k * chunk_dim3 + j];
			//			}
			//			result[(top + i) * dim3 + left + j] += sum;
			//			//res[i * chunk_dim2 + j] = sum;
			//		}
			//	}
			//}
			//check_workers_status();

			while (id == master_id)
			{
				check_workers_status();
				id = get_idle_worker();
			}

			workers[id]->send(a, b, top, left, ch_dim1, ch_dim2, ch_dim3);
		}
	}
	void finish_tasks()
	{
		bool all_done = false;
		while (!all_done)
		{
			all_done = true;
			for (size_t i = 0; i < group_size; ++i)
			{
				if (workers[i]->is_working())
				{
					all_done = false;
					if (workers[i]->receive_ready())
					{
						receive_result(i);
					}
				}
			}
		}		
	}
	void kill_workers()
	{
		for (size_t i = 0; i < group_size; ++i)
			if (i != master_id)
				workers[i]->kill();
	}
	void generate_tasks()
	{
		size_t backup2 = chunk_dim2;

		for (size_t i = 0; i < dim1; i += chunk_dim1)
		{
			if (i + chunk_dim1 > dim1)
				chunk_dim1 = dim1 - i;

			file2.seekg(2 * MATRIX_SIZE_TYPE_SIZE, file2.beg);
			chunk_dim2 = backup2;

			for (size_t j = 0; j < dim2; j += chunk_dim2)
			{
				if (j + chunk_dim2 > dim2)
					chunk_dim2 = dim2 - j;

				file1.read((char *)a, chunk_dim1 * chunk_dim2 * sizeof(float));

				file2.read((char *)b, chunk_dim2 * chunk_dim3 * sizeof(float));

				process_task(i, 0, chunk_dim1, chunk_dim2, chunk_dim3);
			}
		}
		
		// wait for all tasks to be finished
		finish_tasks();
	}

	void work() 
	{
		init();

		generate_tasks();
		
		kill_workers();
		
		finish();
	}
	void finish()
	{
		matrix_save(output, result, dim1, dim3);

		//matrix_print(result, dim1, dim3);

		matrix_free(a);
		matrix_free(b);
		matrix_free(result);
	}

	master(int id, const char *filename1, const char *filename2, const char *output, size_t group_size) :
		master_id(id),
		worker_id(0),
		group_size(group_size),
		file1(filename1, std::ios::binary),
		file2(filename2, std::ios::binary),
		output(output),
		workers()
	{
		
	}
	~master()
	{
		try {
			if (file1.is_open())
				file1.close();
		}
		catch (const std::exception) { }
		try {
			if (file2.is_open())
				file2.close();
		}
		catch (const std::exception) { }
	}
};

#endif