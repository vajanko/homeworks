#include<fstream>
#include<iostream>
#include<memory>

#include "master.hpp"
#include "worker.hpp"

using namespace std;

#define ROOT 0

void abort(int err)
{
	MPI_Finalize();
	exit(err);
}
int catch_error(int err)
{
	if (err)
		abort(err);
	return err;
}

int main(int argc, char **argv) 
{
	catch_error(MPI_Init(&argc, &argv));

	int id;
	catch_error(MPI_Comm_rank(MPI_COMM_WORLD, &id));

	if (id == ROOT)
	{
		int group_size;
		catch_error(MPI_Comm_size(MPI_COMM_WORLD, &group_size));

		master master(id, argv[1], argv[2], argv[3], group_size);
		master.work();
	}
	else
	{
		worker worker(id, ROOT);
		worker.work();
	}

	return catch_error(MPI_Finalize());
}
