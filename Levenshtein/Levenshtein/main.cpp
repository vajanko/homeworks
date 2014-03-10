#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <omp.h>

typedef int size_type;

void generate(const char* filename, size_type count, int seed)
{
	FILE* f = fopen(filename, "wb");
	if (!f) return;
	srand(seed);
	while (count--)
	{
		char ch = (char)((rand() >> 7) & 0xff);
		fwrite(&ch, 1, 1, f);
	}
	fclose(f);
}
// convert from string to given type
template<typename T> T lexical_cast(const char* str)
{
	std::istringstream buf(str);
	T res;
	buf >> res;
	return res;
}

template<typename T> T smaller(T x, T y)
{
	return (x<y) ? x : y;
}

template<typename T> T smaller(T x, T y, T z)
{
	return smaller<T>(smaller<T>(x, y), z);
}

#define N 1024

int generate_files(char* argv[])
{
	// second option - generate random strings
	int count = lexical_cast<size_type>(argv[2]);
	size_type seed = lexical_cast<size_type>(argv[3]);
	generate(argv[1], count, seed);
	return 0;
}
bool read_files(const char *file1, const char *file2, char *&s, size_t &s_size, char *&t, size_t &t_size)
{
	FILE* f1 = fopen(file1, "rb");
	FILE* f2 = fopen(file2, "rb");
	if (!f1 || !f2)
		return false;

	fseek(f1, 0, SEEK_END);
	s_size = ftell(f1);
	s = new char[s_size];
	fseek(f1, 0, SEEK_SET);
	fread(s, 1, s_size, f1);
	fclose(f1);

	fseek(f2, 0, SEEK_END);
	t_size = ftell(f2);
	t = new char[t_size];
	fseek(f2, 0, SEEK_SET);
	fread(t, 1, t_size, f2);
	fclose(f2);

	return true;
}

size_type levenshtein_serial(const char *s, size_t s_size, const char *t, size_t t_size)
{
	// degenerate cases
	if (s_size == 0) return t_size;
	if (t_size == 0) return s_size;

	// create two work vectors of integer distances
	size_t vec_size = t_size + 1;

	size_type *dst1 = new size_type[vec_size];
	size_type *dst2 = new size_type[vec_size];

	for (int i = 0; i <= t_size; ++i)
		dst2[i] = i;

	for (int i = 1; i <= s_size; ++i)
	{
		dst1[0] = i;

		// use formula to fill in the rest of the row
		for (int j = 1; j <= t_size; ++j)
		{
			size_type upper = dst2[j];
			size_type left = dst1[j - 1];
			size_type upperleft = dst2[j - 1];
			char cost = (s[i - 1] == t[j - 1]) ? 0 : 1;

			size_type total = smaller<size_type>(upper + 1, left + 1, upperleft + cost);
			dst1[j] = total;
		}

		std::swap(dst1, dst2);
	}

	return dst2[t_size];
}

void rotate(size_type *&ptr1, size_type *&ptr2, size_type *&ptr3)
{
	size_type *tmp = ptr1;
	ptr1 = ptr2;
	ptr2 = ptr3;
	ptr3 = tmp;
}
size_type levenshtein_parallel(const char *s, size_t s_size, const char *t, size_t t_size)
{
	if (s_size < t_size)
	{
		std::swap(s, t);
		std::swap(s_size, t_size);
	}

	size_t diag_size = t_size;
	size_type *diag1 = new size_type[diag_size + 1];
	size_type *diag2 = new size_type[diag_size + 1];
	size_type *diag3 = new size_type[diag_size + 1];

	// total number of diagonals
	size_t total_diags = s_size + t_size + 1;

	// init
	diag2[0] = 0;
	diag1[0] = 1;
	diag1[1] = 1;		// step=1

	for (size_t step = 2; step <= diag_size; ++step)
	{
#pragma omp parallel for
		for (size_type i = 1; i < step; ++i)
		{
			size_type minus1 = i - 1;
			char cost = (s[step - 1 - i] == t[minus1]) ? 0 : 1;

			size_type upper = diag1[minus1];
			size_type left = diag1[i];
			size_type upperleft = diag2[minus1];

			diag3[i] = smaller<size_type>(upper + 1, left + 1, upperleft + cost);
		}

		diag3[0] = step;
		diag3[step] = step;

		rotate(diag2, diag1, diag3);
	}

	
	size_t full_diag_count = total_diags - diag_size;

	for (size_t step = diag_size + 1; step <= full_diag_count; ++step)
	{
#pragma omp parallel for
		for (size_type i = 1; i <= diag_size; ++i)
		{
			size_type minus1 = i - 1;
			char cost = (s[step - i - 1] == t[minus1]) ? 0 : 1;

			size_type upper = diag1[i];
			size_type left = diag1[minus1];
			size_type upperleft = diag2[minus1];

			diag3[i] = smaller<size_type>(upper + 1, left + 1, upperleft + cost);
		}

		diag3[0] = step;

		rotate(diag2, diag1, diag3);
	}

	size_t err = 0;
	for (size_t step = full_diag_count + 1; step < total_diags; ++step)
	{
#pragma omp parallel for
		for (size_type i = 1; i <= total_diags - step; ++i)
		{
			char cost = (s[s_size - i] == t[step - full_diag_count - 1 + i]) ? 0 : 1;

			size_type upper = diag1[i + 1];
			size_type left = diag1[i];
			size_type upperleft = diag2[i + err];

			diag3[i] = smaller<size_type>(upper + 1, left + 1, upperleft + cost);
		}

		diag3[0] = step;
		err = 1;

		// WARNNING: do not rotate if nothing has been done
		if (total_diags - step > 1)
			rotate(diag2, diag1, diag3);
	}

	size_type res = diag3[1];

	delete diag1;
	delete diag2;
	delete diag3;

	return res;
}

size_type levenshtein_parallel2(const char *s, size_t s_size, const char *t, size_t t_size)
{
	size_type **matrix = new size_type*[s_size];

	matrix[0] = new size_type[t_size];
	for (size_type i = 0; i < s_size; ++i)
		matrix[0][i] = i;
	for (size_type i = 1; i < s_size; ++i)
	{
		matrix[i] = new size_type[t_size];
		matrix[i][0] = i;
	}

	size_t diag_count = s_size + t_size + 1;
	for (size_t diag = 2; diag < diag_count; ++diag)
	{
		for (size_type i = 0; i <= diag; ++i)
		{
			/*char cost = (s[diag - 2 ] == t[minus1]) ? 0 : 1;

			size_type upper = diag1[minus1];
			size_type left = diag1[i];
			size_type upperleft = diag2[minus1];

			diag3[i] = smaller<size_type>(upper + 1, left + 1, upperleft + cost);*/
		}
	}

	return 0;
}

int calculate(char *argv[])
{
	char *s;
	size_t s_size;
	char *t;
	size_t t_size;
	if (!read_files(argv[1], argv[2], s, s_size, t, t_size))
		return -1;


	//size_type dist1 = levenshtein_distance_serial(s, s_size, t, t_size);
	//std::cout << "serial: " << dist1 << std::endl;

	/*size_type dist2 = levenshtein_parallel(s, s_size, t, t_size);
	std::cout << dist2 << std::endl;*/
	//std::cout << "parallel: " << dist2 << std::endl;

	size_type dist2 = levenshtein_blocks(s, s_size, t, t_size);
	std::cout << dist2 << std::endl;
}

void print_matrix(size_type ** matrix, size_t s_size, size_t t_size)
{
	for (size_type r = 0; r <= s_size; ++r)
	{
		for (size_type c = 0; c <= t_size; ++c)
			std::cout << matrix[r][c] << " ";
		std::cout << std::endl;
	}
}
void calculate_diagonal(size_type h_base, size_type v_base, size_type block_count, size_type block_size, const char*s, const char *t, size_type **matrix)
{
	// in this step we have "block_count" block on the diagonale
	// we will start with the block most up and right and follow down and left

	for (size_type block_index = 1; block_index <= block_count; ++block_index)
	{
		// start (up-)right and move (down-)left  -> first part is the block index, multipling by block_size
		// we get the actual position
		size_type h_start = (h_base + block_index - 1) * block_size + 1;
		// just add block_size to get at the end of current block
		size_type h_end = h_start + block_size;

		// vertical start
		size_type v_start = (v_base + block_count - block_index) * block_size + 1;
		//size_type v_start = (base + block_count - (block_index + 1)) * block_size + 1;
		size_type v_end = v_start + block_size;

		// the same algorithm as for serial version, but only on this particular chunk of data
		for (size_type i = h_start; i < h_end; ++i)			// block_size iterations
		{
			// WE DO NOT NEED THIS - INITIALIZATION IS DONE ALREADY
			// only if we are at the left side of the matrix
			if (v_start == 1)
				matrix[i][0] = i;

			for (size_type j = v_start; j < v_end; ++j)		// block_size iterations
			{
				// this is calculation for one block

				char cost = (s[i - 1] == t[j - 1]) ? 0 : 1;

				size_type upper = matrix[i - 1][j];
				size_type left = matrix[i][j - 1];
				size_type upperleft = matrix[i - 1][j - 1];

				size_type total = smaller<size_type>(upper + 1, left + 1, upperleft + cost);
				matrix[i][j] = total;
			}
		}
	}
}
size_type levenshtein_blocks(const char *s, size_t s_size, const char *t, size_t t_size)
{
	// INITIALIZATION - allocate memory
	size_type **matrix = new size_type*[s_size + 1];

	matrix[0] = new size_type[t_size + 1];
	for (size_type i = 0; i <= s_size; ++i)
		matrix[0][i] = i;
	for (size_type i = 1; i <= s_size; ++i)
	{
		matrix[i] = new size_type[t_size + 1];
		matrix[i][0] = i;
	}

	size_type block_size = 2;
	// width of matrix divided into blocks
	size_type max_blocks = t_size / block_size;

	// number of blocks on the diagonal is growing
	for (size_type block_count = 1; block_count < max_blocks; ++block_count)
	{
		calculate_diagonal(0, 0, block_count, block_size, s, t, matrix);
	}

	// number of blocks is constant = total_blocks
	size_type matrix_rows = s_size / block_size;
	size_type matrix_cols = t_size / block_size;
	size_type diag_count = matrix_rows + matrix_cols - 1;
	size_type max_block_diagonals = diag_count - 2 * matrix_cols + 2;
	for (size_type i = 0; i < max_block_diagonals; ++i)
	{
		calculate_diagonal(i, 0, max_blocks, block_size, s, t, matrix);
	}

	// number of blocks is falling
	for (size_type block_count = max_blocks - 1; block_count > 0; --block_count)
	{
		size_type base = matrix_rows - max_blocks + 1;
		calculate_diagonal(base, max_blocks - 1, block_count, block_size, s, t, matrix);
	}

	//print_matrix(matrix, s_size, t_size);

	return matrix[s_size][t_size];
}
void test()
{
	char *s = "abcdffff";
	char *t = "adbc";
	auto dist = levenshtein_blocks(s, 8, t, 4);
	system("pause");
}

int main(int argc, char* argv[])
{
	//test();
	//return 0;

	int ret = 0;
	//double time = omp_get_wtime();
	//std::cout << "BEGIN" << std::endl;
	//// BEGIN

	if (argc == 4)
	{
		ret = generate_files(argv);
	}
	else if (argc == 3)
	{
		ret = calculate(argv);
	}

	//// END
	//std::cout << "END" << std::endl;
	//time = omp_get_wtime() - time;
	//std::cout << "execution time: " << time << " s" << std::endl;

	//system("pause");
	return ret;
}
