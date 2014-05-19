#ifndef master_hdr
#define master_hdr

#include<mpi.h>

#include<string>
#include<fstream>
#include<vector>
#include<memory>
#include "matrix.hpp"

#define TAG 0

// hold by master
class worker_task
{
private:
	// worker mpi ID
	int worker_id;
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
	void send(matrix a, matrix b, size_t chunk_dim1, size_t chunk_dim2, size_t chunk_dim3)
	{
		working = true;
		my_dim1 = chunk_dim1;
		my_dim2 = chunk_dim2;
		my_dim3 = chunk_dim3;

		int pos = 0;
		MPI_Pack(&chunk_dim1, 1, MPI_INT, req_buff, req_buff_size, &pos, MPI_COMM_WORLD);
		MPI_Pack(&chunk_dim2, 1, MPI_INT, req_buff, req_buff_size, &pos, MPI_COMM_WORLD);
		MPI_Pack(&chunk_dim3, 1, MPI_INT, req_buff, req_buff_size, &pos, MPI_COMM_WORLD);
		// TODO: add data
		MPI_Pack(a, chunk_dim1 * chunk_dim2, MPI_FLOAT, req_buff, req_buff_size, &pos, MPI_COMM_WORLD);
		MPI_Pack(b, chunk_dim2 * chunk_dim3, MPI_FLOAT, req_buff, req_buff_size, &pos, MPI_COMM_WORLD);
		
		MPI_Isend(req_buff, pos, MPI_PACKED, worker_id, TAG, MPI_COMM_WORLD, &send_req);

		// start receiving imediatelly
		MPI_Irecv(req_buff, chunk_dim2 * chunk_dim2, MPI_FLOAT, worker_id, TAG, MPI_COMM_WORLD, &recv_req);
	}
	bool receive_ready()
	{
		MPI_Status status;
		int flag;
		MPI_Test(&recv_req, &flag, &status);

		return flag != 0;
	}
	void receive(matrix &res, size_t &top, size_t &left, size_t &dim)
	{
		res = (matrix)req_buff;
		top = this->top;
		left = this->left;
		dim = this->my_dim2;

		working = false;
	}
	worker_task(int worker_id, size_t max_dim1, size_t max_dim2, size_t max_dim3) :
		worker_id(worker_id),
		req_buff_size(sizeof(int)* 3 + sizeof(float)* (max_dim1 * max_dim2 + max_dim2 * max_dim3)),
		req_buff(new char[sizeof(int)* 3 + sizeof(float)* (max_dim1 * max_dim2 + max_dim2 * max_dim3)])
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

		// TODO: decide
		chunk_dim1 = 2;
		chunk_dim2 = 2;
		chunk_dim3 = 2;

		for (size_t i = 0; i < group_size; ++i)
			workers.push_back(new worker_task(i, chunk_dim1, chunk_dim2, chunk_dim3));

		// broadcast max size of chunk
		int max_chunk[3];
		max_chunk[0] = chunk_dim1;
		max_chunk[1] = chunk_dim2;
		max_chunk[2] = chunk_dim3;
		MPI_Bcast(max_chunk, 3, MPI_INT, master_id, MPI_COMM_WORLD);
	}
	int get_idle_worker()
	{
		for (int w = 0; w < group_size; ++w, worker_id = (worker_id + 1) % group_size)
			if (workers[worker_id]->is_idle() && worker_id != master_id)
				break;
		// return either ID of the next idle worker or ROOT if none of them is idle
		return workers[worker_id]->is_idle() ? worker_id : master_id;
	}
	void check_workers_status()
	{
		matrix chunk_a, chunk_b;

		for (int w = 0; w < group_size; ++w, worker_id = (worker_id + 1) % group_size)
		{
			if (workers[worker_id]->is_working() && workers[worker_id]->receive_ready())
			{
				matrix res;
				size_t top, left, dim;
				workers[worker_id]->receive(res, top, left, dim);
			}
		}
	}
	void process_task(size_t ch_dim1, size_t ch_dim2, size_t ch_dim3)
	{
		int id = get_idle_worker();
		if (id != master_id)
		{	// we have found an idle worker
			workers[id]->send(a, b, ch_dim1, ch_dim2, ch_dim3);
		}
		else
		{	// all workers are working currently
			check_workers_status();
			id = get_idle_worker();

			while (id == master_id)
			{
				// do peace of your own job
				// ..

				check_workers_status();
				id = get_idle_worker();
			}
		}
	}
	void generate_tasks()
	{
		// TODO:
		for (size_t i = 0; i < dim1; i += chunk_dim1)
		{
			for (size_t j = 0; j < dim2; j += chunk_dim2)
			{
				matrix_load(a, file1, i, j, chunk_dim1, chunk_dim2, dim2);

				for (size_t k = 0; k < dim3; k += chunk_dim3)
				{
					matrix_load(b, file2, j, k, chunk_dim2, chunk_dim3, dim3);
					// at this point there is a new task generated

					process_task(chunk_dim1, chunk_dim2, chunk_dim3);
				}
			}
		}
	}
	void work() 
	{
		init();

		generate_tasks();

		finish();
	}
	void finish()
	{
		matrix_save(output, result, dim1, dim3);
	}

	master(int id, const char *filename1, const char *filename2, const char *output, size_t group_size) :
		master_id(id),
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