#ifndef worker_hdr
#define worker_hdr

#include<mpi.h>

class worker
{
private:
	int master_id;
	int worker_id;
	MPI_Request send_req;
	MPI_Request recv_req;
	void *req_buff;

public:
	void init()
	{
		int max_chunk[3];
		MPI_Bcast(max_chunk, 3, MPI_INT, master_id, MPI_COMM_WORLD);

		// allocate buffers
	}
	void work()
	{
		init();

		while (true)
		{
			// receive request - if poisonous break

			// calculation

			// send result
		}

		finish();
	}
	void finish()
	{

	}

	worker(int worker_id, int master_id) : worker_id(worker_id), master_id(master_id)
	{ }
	~worker()
	{

	}
};

#endif