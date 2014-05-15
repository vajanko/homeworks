#include<fstream>
#include<iostream>

using namespace std;

// TODO: second matrix represent as an array of columns
// size of matrix size type is 4 by definition
#define MATRIX_SIZE_TYPE_SIZE 4

typedef float * matrix;


matrix matrix_alloc(size_t rows, size_t cols)
{
	matrix data = new float[rows * cols];
	return data;
}
void matrix_free(matrix m)
{
	delete[] m;
}

void matrix_init(matrix m, size_t rows, size_t cols, float value)
{
	for (size_t i = 0; i < rows * cols; ++i)
		m[i] = value;
}
matrix matrix_multiply(const matrix m1, const matrix m2, size_t dim1, size_t dim2, size_t dim3)
{
	// return the result in a newly allocated matrix
	matrix res = matrix_alloc(dim1, dim2);

	for (size_t i = 0; i < dim1; ++i)
	{
		for (size_t j = 0; j < dim3; ++j)
		{
			// [i,j] is position in the result matrix
			float sum = 0;
			for (size_t k = 0; k < dim2; ++k)
				// m1 = (dim1 x dim2)
				// m2 = (dim2 x dim3)
				sum += m1[i * dim2 + k] * m2[k * dim3 + j];

			// res = (dim1 x dim2)
			res[i * dim2 + j] = sum;
		}
	}

	return res;
}
void matrix_add(matrix m1, const matrix m2, size_t rows, size_t cols)
{
	for (size_t i = 0; i < rows * cols; ++i)
		m1[i] += m2[i];
}

void matrix_read_size(ifstream &file, size_t &rows, size_t &cols)
{
	file.read((char *)&rows, MATRIX_SIZE_TYPE_SIZE);
	file.read((char *)&cols, MATRIX_SIZE_TYPE_SIZE);
}
matrix matrix_load(ifstream &file, size_t top, size_t left, size_t rows, size_t cols, size_t width)
{
	matrix res = matrix_alloc(rows, cols);

	matrix_load(res, file, top, left, rows, cols, width);

	return res;
}
void matrix_load(matrix res, ifstream &file, size_t top, size_t left, size_t rows, size_t cols, size_t width)
{
	const size_t base = 2 * MATRIX_SIZE_TYPE_SIZE;

	for (size_t i = 0; i < rows; ++i)
	{
		size_t pos = base + ((top + i) * width + left) * sizeof(float);
		file.seekg(pos, file.beg);
		file.read((char *)(res + i * cols), sizeof(float)* cols);
	}
}
void matrix_save(const char *filename, const matrix m, size_t rows, size_t cols)
{
	ofstream output(filename, ios::binary);

	output.write((char *)&rows, MATRIX_SIZE_TYPE_SIZE);
	output.write((char *)&cols, MATRIX_SIZE_TYPE_SIZE);

	output.write((char *)m, rows * cols * sizeof(float));

	output.close();
}
void matrix_copy(const matrix src, size_t rows, size_t cols, matrix dst, size_t top, size_t left)
{
	for (size_t i = 0; i < rows; ++i)
	{	// more effencitve copy
		for (size_t j = 0; j < cols; ++j)
		{
			dst[(top + i ) * cols + left + j] = src[i * cols + j];
		}
	}
}

void matrix_print(matrix m, size_t rows, size_t cols)
{
	for (size_t i = 0; i < rows; ++i)
	{
		for (size_t j = 0; j < cols; ++j)
			cout << m[i * cols + j] << " ";
		cout << endl;
	}
}

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
void mpi_send_work(const matrix m1, const matrix m2, size_t dim1, size_t dim2, size_t dim3)
{
	//MPI_Isend(&low_bound, 1, MPI_INT, i, MASTER_TO_SLAVE_TAG, MPI_COMM_WORLD, &request);
}
bool mpi_check_work(matrix res, size_t &rows, size_t &cols)
{
	return false;
}
void mpi_divide_work(const char *filename1, const char *filename2, const char *output, size_t group_size)
{
	ifstream file1(filename1, ios::binary);
	ifstream file2(filename2, ios::binary);

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

	for (size_t i = 0; i < dim1; i += chunk_dim1)
	{
		for (size_t j = 0; j < dim2; j += chunk_dim2)
		{
			matrix_load(a, file1, i, j, chunk_dim1, chunk_dim2, dim2);

			for (size_t k = 0; k < dim3; k += chunk_dim3)
			{
				matrix_load(b, file2, j, k, chunk_dim2, chunk_dim3, dim3);
				mpi_send_work(a, b, chunk_dim1, chunk_dim2, chunk_dim3);

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
}

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

#ifdef MPI
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