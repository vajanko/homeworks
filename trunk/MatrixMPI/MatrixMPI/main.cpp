#include<fstream>
#include<iostream>
#include<memory>
#include "matrix.hpp"

#define ROOT 0

using namespace std;

void mpi_pack_and_send(const matrix m1, const matrix m2, size_t dim1, size_t dim2, size_t dim3)
{
	size_t buff_size = 3 * sizeof(size_t)+(dim1 * dim2 + dim2 * dim3) * sizeof(float);
	char *buff = new char[buff_size];
	int pos = 0;

	/*MPI_Pack(&dim1, 1, MPI_INT, buff, buff_size, &pos, MPI_COMM_WORLD);
	MPI_Pack(&dim2, 1, MPI_INT, buff, buff_size, &pos, MPI_COMM_WORLD);
	MPI_Pack(&dim3, 1, MPI_INT, buff, buff_size, &pos, MPI_COMM_WORLD);
	MPI_Pack(m1, dim1 * dim2, MPI_FLOAT, buff, buff_size, &pos, MPI_COMM_WORLD);
	MPI_Pack(m2, dim2 * dim3, MPI_FLOAT, buff, buff_size, &pos, MPI_COMM_WORLD);*/

}
void mpi_send_work(const matrix m1, const matrix m2, size_t dim1, size_t dim2, size_t dim3, int worker_id)
{
	//MPI_Isend(&low_bound, 1, MPI_INT, i, MASTER_TO_SLAVE_TAG, MPI_COMM_WORLD, &request);
}
bool mpi_check_work(matrix res, size_t &rows, size_t &cols)
{
	return false;
}
void mpi_divide_work(const char *filename1, const char *filename2, const char *output, size_t group_size)
{
	// open matrix files ..
	ifstream file1(filename1, ios::binary);
	ifstream file2(filename2, ios::binary);
	// .. and read their sizes
	size_t dim1, dim2, dim3;
	matrix_read_size(file1, dim1, dim2);
	matrix_read_size(file2, dim2, dim3);

	// TODO: decide
	size_t chunk_dim1 = 2;
	size_t chunk_dim2 = 2;
	size_t chunk_dim3 = 2;

	// allocate matrices for chunks and reuse them
	matrix a = matrix_alloc(chunk_dim1, chunk_dim2);
	matrix b = matrix_alloc(chunk_dim2, chunk_dim3);

	// complete result of matrix multiplication
	matrix result = matrix_alloc(dim1, dim3);
	matrix_init(result, dim1, dim3, 0.0);

	// worker data of the master
	size_t my_dim1, my_dim2, my_dim3;
	matrix my_a, my_b;

	int worker_id = 1;		// ID of current worker
	bool * working = new bool[group_size];
	for (size_t i = 0; i < group_size; ++i)
		working[i] = false;


	for (size_t i = 0; i < dim1; i += chunk_dim1)
	{
		for (size_t j = 0; j < dim2; j += chunk_dim2)
		{
			matrix_load(a, file1, i, j, chunk_dim1, chunk_dim2, dim2);

			for (size_t k = 0; k < dim3; k += chunk_dim3)
			{
				matrix_load(b, file2, j, k, chunk_dim2, chunk_dim3, dim3);
				// at this point there is a new task generated

				for (size_t w = 0; w < group_size; ++w, worker_id = (worker_id + 1) % group_size)
					if (!working[worker_id] && worker_id != ROOT)
						break;
				// worker_id is now ID of an idle worker or if working[worker_id] == true none of them is idle

				if (!working[worker_id])
				{	// assign task to current worker
					working[worker_id] = true;
					mpi_send_work(a, b, chunk_dim1, chunk_dim2, chunk_dim3, worker_id);
				}
				else
				{	// all worker are working
					// check whether any of them is finished

					// if none of the workers is finished do your job
				}
		

				// check for received work - if any, add it to result matrix
				while (mpi_check_work(a, chunk_dim1, chunk_dim3))
				{
					// TODO: left and top position in the result matrix
					matrix_add(result, a, chunk_dim1, chunk_dim3);
				}
			}
		}
	}

	// TODO: calculate the rest chunks

	matrix_save(output, result, dim1, dim3);

	delete[] working;
}
#ifndef MPI
int main(int argc, char **argv) 
{
	int err = 0;

	size_t rows, cols;
	ifstream input("C:\\tmp\\m_3_3.dat", ios::binary);
	matrix_read_size(input, rows, cols);
	matrix m1 = matrix_load(input, 0, 0, 3, 3, cols);
	matrix_print(m1, 3, 3);
	cout << endl;

	/*matrix m2 = matrix_multiply(m1, m1, 2, 2, 2);
	matrix_print(m2, 2, 2);*/

	//matrix_free(m1, 2);
	//matrix_free(m, rows);

	system("pause");
	return err;
}

#else
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
void test()
{
	MPI_Type_vector(count, blocklength, stride, oldtype, &newtype);
	MPI_Type_commit (&newtype);
	MPI_Send(buffer, 1, newtype, dest, tag, comm);
	int MPI_Type_free (&newtype)
}

int main(int argc, char **argv) 
{
	catch_error(MPI_Init(&argc, &argv));

	return catch_error(MPI_Finalize());
}
#endif