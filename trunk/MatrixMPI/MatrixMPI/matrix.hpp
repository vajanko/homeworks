#ifndef matrix_hdr
#define matrix_hdr

#include<fstream>
#include<iostream>

#define MATRIX_SIZE_TYPE_SIZE 4

// size of matrix size type is 4 by definition
typedef float * matrix;

matrix matrix_alloc(size_t rows, size_t cols)
{
	matrix data = new float[rows * cols];
	return data;
}
void matrix_init(matrix m, size_t rows, size_t cols, float value)
{
	for (size_t i = 0; i < rows * cols; ++i)
		m[i] = value;
}
matrix matrix_alloc(size_t rows, size_t cols, float value)
{
	matrix data = matrix_alloc(rows, cols);
	matrix_init(data, rows, cols, value);
	return data;
}
void matrix_free(matrix m)
{
	delete[] m;
}


matrix matrix_multiply(const matrix m1, const matrix m2, size_t dim1, size_t dim2, size_t dim3)
{
	// return the result in a newly allocated matrix
	matrix res = matrix_alloc(dim1, dim3);

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
void matrix_add(matrix m1, size_t top, size_t left, size_t width, const matrix m2, size_t rows, size_t cols)
{
	for (size_t i = 0; i < rows; ++i)
	{
		for (size_t j = 0; j < cols; ++j)
		{
			m1[(top + i) * width + left + j] += m2[i * cols + j];
		}
	}
}

void matrix_read_size(std::ifstream &file, size_t &rows, size_t &cols)
{
	unsigned int r, c;
	file.read((char *)&r, MATRIX_SIZE_TYPE_SIZE);
	file.read((char *)&c, MATRIX_SIZE_TYPE_SIZE);
	rows = r;
	cols = c;
}
void matrix_load(matrix res, std::ifstream &file, size_t top, size_t left, size_t rows, size_t cols, size_t width)
{
	const size_t base = 2 * MATRIX_SIZE_TYPE_SIZE;

	for (size_t i = 0; i < rows; ++i)
	{
		size_t pos = base + ((top + i) * width + left) * sizeof(float);
		file.seekg(pos, file.beg);
		file.read((char *)(res + i * cols), sizeof(float)* cols);
	}
}
matrix matrix_load(std::ifstream &file, size_t top, size_t left, size_t rows, size_t cols, size_t width)
{
	matrix res = matrix_alloc(rows, cols);

	matrix_load(res, file, top, left, rows, cols, width);

	return res;
}
void matrix_save(const char *filename, const matrix m, size_t rows, size_t cols)
{
	std::ofstream output(filename, std::ios::binary);

	unsigned int r = rows, c = cols;
	output.write((char *)&r, MATRIX_SIZE_TYPE_SIZE);
	output.write((char *)&c, MATRIX_SIZE_TYPE_SIZE);

	output.write((char *)m, rows * cols * sizeof(float));

	output.close();
}
void matrix_copy(const matrix src, size_t rows, size_t cols, matrix dst, size_t top, size_t left)
{
	for (size_t i = 0; i < rows; ++i)
	{	// more effencitve copy
		for (size_t j = 0; j < cols; ++j)
		{
			dst[(top + i) * cols + left + j] = src[i * cols + j];
		}
	}
}

void matrix_print(matrix m, size_t rows, size_t cols)
{
	for (size_t i = 0; i < rows; ++i)
	{
		for (size_t j = 0; j < cols; ++j)
			std::cout << m[i * cols + j] << " ";
		std::cout << std::endl;
	}
}


#endif