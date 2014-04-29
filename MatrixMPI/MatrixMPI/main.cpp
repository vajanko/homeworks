#include<fstream>
#include<iostream>

using namespace std;

typedef float ** matrix;
// TODO: make from the matrix type a complex object

matrix matrix_alloc(size_t rows, size_t cols)
{
	matrix data = new float*[rows];
	for (size_t i = 0; i < rows; ++i)
	{
		data[i] = new float[cols];
		for (size_t j = 0; j < cols; j++)
			data[i][j] = 0.0;
	}

	return data;
}
// return the result in a newly allocated matrix
matrix matrix_multiply(const matrix m1, const matrix m2, size_t dim1, size_t dim2, size_t dim3)
{
	matrix res = matrix_alloc(dim1, dim2);

	for (size_t i = 0; i < dim1; ++i)
	{
		for (size_t j = 0; j < dim3; ++j)
		{
			// [i,j] is position in the result matrix
			float sum = 0;
			for (size_t k = 0; k < dim2; ++k)
				sum += m1[i][k] * m2[k][j];
			res[i][j] = sum;
		}
	}

	return res;
}
matrix matrix_load(const char *filename, size_t &rows, size_t &cols)
{
	ifstream input(filename, ios::binary);

	//size_t rows, cols;
	input.read((char *)&rows, 4);		// size of this number is 4 bytes by definition
	input.read((char *)&cols, 4);

	float val;

	matrix res = matrix_alloc(rows, cols);
	for (size_t i = 0; i < rows; ++i)
	{
		for (size_t j = 0; j < cols; ++j)
		{
			input.read((char *)&val, sizeof(val));
			res[i][j] = val;
			//input.read((char *)&res[i][j], sizeof(float));
			//input >> res[i][j];
		}
	}

	input.close();

	return res;
}
void matrix_copy(const matrix src, size_t rows, size_t cols, matrix dst, size_t top, size_t left)
{
	for (size_t i = 0; i < rows; ++i)
	{
		for (size_t j = 0; j < cols; ++j)
		{
			dst[top + i][left + j] = src[i][j];
		}
	}
}

void matrix_print(matrix m, size_t rows, size_t cols)
{
	for (size_t i = 0; i < rows; ++i)
	{
		for (size_t j = 0; j < cols; ++j)
			cout << m[i][j] << " ";
		cout << endl;
	}
}

int main(int argc, char **argv) 
{
	int err = 0;

	size_t rows, cols;
	matrix m = matrix_load("C:\\tmp\\m_3_3.dat", rows, cols);
	matrix_print(m, rows, cols);

	matrix mult = matrix_multiply(m, m, rows, cols, cols);
	matrix_print(mult, rows, cols);

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

	size_t rows, cols;
	matrix m = matrix_load("C:\\tmp\\m_3_3.dat", rows, cols);
	matrix_print(m, rows, cols);

	matrix mult = matrix_multiply(m, m, rows, cols, cols);
	matrix_print(mult, rows, cols);

	return catch_error(MPI_Finalize());
}
#endif