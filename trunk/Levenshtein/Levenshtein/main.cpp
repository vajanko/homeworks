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
			char cost = (s[diag - 2 - i] == t[minus1]) ? 0 : 1;

			size_type upper = diag1[minus1];
			size_type left = diag1[i];
			size_type upperleft = diag2[minus1];

			diag3[i] = smaller<size_type>(upper + 1, left + 1, upperleft + cost);
		}
	}
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

	size_type dist2 = levenshtein_parallel(s, s_size, t, t_size);
	std::cout << dist2 << std::endl;
	//std::cout << "parallel: " << dist2 << std::endl;
}
int main(int argc, char* argv[])
{
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
