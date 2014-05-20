#ifndef worker_hdr
#define worker_hdr

#include<mpi.h>

#include<fstream>
#include<string>


class worker
{
private:
	int master_id;
	int worker_id;
	MPI_Request send_req;
	MPI_Request recv_req;
	void *req_buff;
	size_t req_buff_size;
	// sizes of matrix the worker is currently working on
	size_t my_dim1, my_dim2, my_dim3;
	matrix a, b;

	std::ofstream log_file;

public:
	void init()
	{
		int max_chunk[3];
		MPI_Bcast(max_chunk, 3, MPI_INT, master_id, MPI_COMM_WORLD);

		// allocate buffers
		req_buff_size = sizeof(unsigned long)* 3 + sizeof(float)* (max_chunk[0] * max_chunk[1] + max_chunk[1] * max_chunk[2]);
		req_buff = new char[req_buff_size];
		a = matrix_alloc(max_chunk[0], max_chunk[1]);
		b = matrix_alloc(max_chunk[1], max_chunk[2]);
	}
	void receive(matrix a, matrix b, size_t &chunk_dim1, size_t &chunk_dim2, size_t &chunk_dim3)
	{
		MPI_Status status;
		MPI_Recv(req_buff, req_buff_size, MPI_PACKED, master_id, TAG, MPI_COMM_WORLD, &status);

		log_file << "data received" << std::endl;

		int pos = 0;
		MPI_Unpack(req_buff, req_buff_size, &pos, &chunk_dim1, 1, MPI_UNSIGNED_LONG, MPI_COMM_WORLD);
		MPI_Unpack(req_buff, req_buff_size, &pos, &chunk_dim2, 1, MPI_UNSIGNED_LONG, MPI_COMM_WORLD);
		MPI_Unpack(req_buff, req_buff_size, &pos, &chunk_dim3, 1, MPI_UNSIGNED_LONG, MPI_COMM_WORLD);
		MPI_Unpack(req_buff, req_buff_size, &pos, a, chunk_dim1 * chunk_dim2, MPI_FLOAT, MPI_COMM_WORLD);
		MPI_Unpack(req_buff, req_buff_size, &pos, b, chunk_dim2 * chunk_dim3, MPI_FLOAT, MPI_COMM_WORLD);

		log_file << "data unpacked" << std::endl;
	}
	void send(matrix res, size_t rows, size_t cols)
	{
		MPI_Send(res, rows * cols, MPI_FLOAT, master_id, TAG, MPI_COMM_WORLD);
		log_file << "data sent" << std::endl;
	}
	void work()
	{
		init();

		while (true)
		{
			// receive request - if poisonous break
			receive(a, b, my_dim1, my_dim2, my_dim3);

			if (my_dim1 == 0 && my_dim2 == 0 && my_dim3 == 0)
			{
				log_file << "poison pill" << std::endl;
				break;
			}

			log_file << "before multiply: " << my_dim1 << " " << my_dim2 << " " << my_dim3 << std::endl;
			// calculation (TODO: reused result matrix buffer)
			matrix res = matrix_multiply(a, b, my_dim1, my_dim2, my_dim3);

			log_file << "before send" << std::endl;
			// send result
			send(res, my_dim1, my_dim3);

			matrix_free(res);
		}

		log_file << "finilizing" << std::endl;
		finish();
	}
	void finish()
	{
		delete[] (char *)req_buff;
		log_file.close();
	}

	worker(int worker_id, int master_id) : worker_id(worker_id), master_id(master_id), log_file("tmp.dat")
	{ }
	~worker()
	{
		
	}
};

#endif