#ifndef du4levenstein_hpp_
#define du4levenstein_hpp_

#include <iostream>
#include <assert.h>

template<typename T> T smaller(T x, T y) { return (x<y) ? x : y; }
template<typename T> T smaller(T x, T y, T z) { return smaller<T>(smaller<T>(x, y), z); }

class levenstein 
{
public:
	typedef int data_element;

private:
	data_element *str1;
	std::size_t size1;

	data_element *str2;
	std::size_t size2;

	data_element * dst1;
	data_element * dst2;
	data_element * dst3;

	data_element *left;
	data_element *top;
	data_element *diag1;
	data_element *diag2;
	data_element *diag3;

	data_element *mat;
	std::size_t width;
	std::size_t height;

public:
	template< typename I1, typename I2>
	levenstein(I1 i1b, I1 i1e, I2 i2b, I2 i2e) :
		str1(new data_element[std::distance(i1b, i1e)]), size1(std::distance(i1b, i1e)),
		str2(new data_element[std::distance(i2b, i2e)]), size2(std::distance(i2b, i2e))
	{
		std::copy(i1b, i1e, str1);
		std::copy(i2b, i2e, str2);

		if (size2 > size1)
		{
			std::swap(size1, size2);
			std::swap(str1, str2);
		}

		dst1 = new data_element[size2 + 1];
		dst2 = new data_element[size2 + 1];
		dst3 = new data_element[size2 + 1];

		for (int i = 0; i <= size2; ++i)
		{
			dst2[i] = i;
			dst1[i] = i;
		}

		/*left = new data_element[size1 + 1];
		left1 = new data_element[size1 + 1];
		for (std::size_t i = 0; i <= size1; ++i)
		{
			left[i] = i;
			left1[i] = i;
		}
		top = new data_element[size2 + 1];
		top1 = new data_element[size2 + 1];
		for (std::size_t i = 0; i <= size2; ++i)
		{
			top[i] = i;
			top1[i] = i;
		}*/

		width = size2 + 1;
		height = size1 + 1;
		mat = new data_element[width * height];
		for (std::size_t i = 0; i <= size1; ++i)
			set(i, 0, i);
		for (std::size_t j = 0; j <= size2; ++j)
			set(0, j, j);
	}
	~levenstein()
	{
		delete[] mat;
	}

	data_element compute_two_rows()
	{
		if (size1 == 0) return size2;
		if (size2 == 0) return size1;

		for (int i = 0; i <= size2; ++i)
		{
			dst2[i] = i;
			dst1[i] = i;
		}

		for (int i = 1; i <= size1; ++i)
		{
			dst1[0] = i;

			// use formula to fill in the rest of the row
			for (int j = 1; j <= size2; ++j)
			{
				char cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
				dst1[j] = smaller<data_element>(dst2[j] + 1, dst1[j - 1] + 1, dst2[j - 1] + cost);
			}

			std::swap(dst1, dst2);
		}

		return dst2[size2];
	}

	data_element compute_two_rows_skip()
	{
		if (size1 == 0) return size2;
		if (size2 == 0) return size1;

		for (int i = 0; i <= size2; ++i)
		{
			dst2[i] = i;
			dst1[i] = i;
		}

		int row = 0;
		int col = 0;

		while (true)	// TODO: break at the end of matrix
		{
			for (int i = col; i < size2; ++i)
				if (str1[row] == str2[i])
				{
					goto inc_col;
				}

			for (int i = row; i < size1; ++i)
				if (str1[i] == str2[col])
				{
					goto inc_row;
				}

			++col;	// no match on the row
			++row;	// no match in the column
		}

	inc_row:
		while (row < size1)
		{
			for (int i = col; i < size1; ++i)
				if (str1[i] == str2[col])
				{
					goto start;
				}
			++row;
		}
	inc_col:
		while (col < size2)
		{
			for (int i = row; i < size2; ++i)
				if (str1[row] == str2[i])
				{
					goto start;
				}
			++col;
		}

	start:
		// arrays are already initialized with 0, 1, 2, 3 ... - just start at given position

		++row;
		++col;
		for (int i = row; i <= size1; ++i)
		{
			dst1[col - 1] = i;
			dst3[col - 1] = i;

			// use formula to fill in the rest of the row
			for (int j = col; j <= size2; ++j)
			{
				char cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
				int total = smaller<data_element>(dst2[j] + 1, dst1[j - 1] + 1, dst2[j - 1] + cost);
				dst1[j] = total;

				//dst1[j] = smaller<data_element>(dst2[j] + 1, dst1[j - 1] + 1, dst2[j - 1] + cost);
			}

			/*data_element *tmp = dst1;
			dst1 = dst2;
			dst2 = dst3;
			dst3 = tmp;*/
			std::swap(dst1, dst2);
		}

		return dst2[size2];
	}

	data_element compute_two_rows_cache()
	{
		if (size1 == 0) return size2;
		if (size2 == 0) return size1;

		const int bsize = 4;

		for (int bi = 0; bi < size1; bi += bsize)
		{
			for (int i = bi; i < bi + bsize; ++i)
			{
				//dst1[0] = i;

				// use formula to fill in the rest of the row
				for (int j = 1; j <= size2; ++j)
				{
					char cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
					dst1[j] = smaller<data_element>(dst2[j] + 1, dst1[j - 1] + 1, dst2[j - 1] + cost);
				}

				std::swap(dst1, dst2);
			}
		}

		return dst2[size2];
	}
	void print_rows()
	{
		std::cout << std::endl << "dst1: ";
		for (int i = 0; i <= size2; ++i)
			std::cout << dst1[i] << " ";
		std::cout << std::endl << "dst2: ";
		for (int i = 0; i <= size2; ++i)
			std::cout << dst2[i] << " ";
		std::cout << std::endl;
	}
	data_element compute_two_rows_skew()
	{
		if (size1 == 0) return size2;
		if (size2 == 0) return size1;

		for (int i = 1; i <= size1; ++i)
		{
			dst1[0] = i;

			int count = smaller<int>(i, size2);
			// use formula to fill in the rest of the row
			for (int j = 1; j <= count; ++j)
			{
				char cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
				dst1[j] = smaller<data_element>(dst2[j] + 1, dst1[j - 1] + 1, dst2[j - 1] + cost);
			}

			std::swap(dst1, dst2);
			//print_rows();
		}
		for (int i = size1; i >= 0; --i)
		{
			for (int j = size1 - i + 1; j <= size2; ++j)
			{
				char cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
				dst1[j] = smaller<data_element>(dst2[j] + 1, dst1[j - 1] + 1, dst2[j - 1] + cost);
			}

			std::swap(dst1, dst2);
			//print_rows();
		}

		return dst2[size2];
	}
	data_element compute_matrix()
	{
		int diag_len;

		// upper-left matrix triangle - diagonal size is growing
		for (diag_len = 1; diag_len < size2; ++diag_len)
		{
			int j = 1;
			for (int i = diag_len; i > 0; --i, ++j)
			{
				data_element upper = get(i - 1, j);
				data_element left = get(i, j - 1);
				data_element upperleft = get(i - 1, j - 1);
				char cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;

				set(i, j, smaller<data_element>(upper + 1, left + 1, upperleft + cost));
			}
		}
		// middle of the mattrix - diagonal is constant
		diag_len = size2;
		for (int x = size2; x < size1; ++x)
		{
			int i = x;
			for (int j = 1; j <= diag_len; ++j, --i)
			{
				data_element upper = get(i - 1, j);
				data_element left = get(i, j - 1);
				data_element upperleft = get(i - 1, j - 1);
				char cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;

				set(i, j, smaller<data_element>(upper + 1, left + 1, upperleft + cost));
			}
		}
		// bottom-right matrix triangle - diagonal size is 
		for (diag_len = size2 - 1; diag_len >= 0; --diag_len)
		{
			int i = size1;
			for (int j = size2 - diag_len; j <= size2; ++j, --i)
			{
				data_element upper = get(i - 1, j);
				data_element left = get(i, j - 1);
				data_element upperleft = get(i - 1, j - 1);
				char cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;

				set(i, j, smaller<data_element>(upper + 1, left + 1, upperleft + cost));
			}
		}

		return get(size1, size2);
	}

	void print_diagonal(data_element *diag, int size)
	{
		std::cout << std::endl;
		for (int i = 0; i <= size; ++i)
			std::cout << diag[i] << " ";
		std::cout << std::endl;
	}
	data_element compute_matrix2()
	{
		size_t diag_size = size1;
		diag1 = new data_element[diag_size + 1];
		diag2 = new data_element[diag_size + 1];
		diag3 = new data_element[diag_size + 1];

		// init
		diag2[0] = 0;
		diag1[0] = 1;
		diag1[1] = 1;

		int diag_len;

		// upper-left matrix triangle - diagonal size is growing
		for (diag_len = 1; diag_len < size2; ++diag_len)
		{
			int j = 1;
			diag3[diag_len + 1] = diag_len;
			for (int i = diag_len; i > 0; --i, ++j)
			{
				char cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
				diag3[i] = smaller<data_element>(diag1[i - 1] + 1, diag1[i] + 1, diag2[i - 1] + cost);
			}
			diag3[0] = diag_len;

			print_diagonal(diag3, diag_len);

			rotate(diag2, diag1, diag3);
		}

		// middle of the matrix - diagonal is constant
		diag_len = size2;
		for (int x = size2; x < size1; ++x)
		{
			int i = x;
			for (int j = 1; j <= diag_len; ++j, --i)
			{
				data_element upper = get(i - 1, j);
				data_element left = get(i, j - 1);
				data_element upperleft = get(i - 1, j - 1);
				char cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;

				set(i, j, smaller<data_element>(upper + 1, left + 1, upperleft + cost));
			}
		}
		// bottom-right matrix triangle - diagonal size is 
		for (diag_len = size2 - 1; diag_len >= 0; --diag_len)
		{
			int i = size1;
			for (int j = size2 - diag_len; j <= size2; ++j, --i)
			{
				data_element upper = get(i - 1, j);
				data_element left = get(i, j - 1);
				data_element upperleft = get(i - 1, j - 1);
				char cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;

				set(i, j, smaller<data_element>(upper + 1, left + 1, upperleft + cost));
			}
		}

		return get(size1, size2);
	}

	data_element get(std::size_t i, std::size_t j)
	{
		return mat[i * width + j];
	}
	void set(std::size_t i, std::size_t j, data_element val)
	{
		mat[i * width + j] = val;
	}
	void print_matrix()
	{
		std::cout << std::endl;
		for (std::size_t i = 0; i <= size1; ++i)
		{
			for (std::size_t j = 0; j <= size2; ++j)
			{
				std::cout << get(i, j) << " ";
			}
			std::cout << std::endl;
		}
	}

	void rotate(data_element *&ptr1, data_element *&ptr2, data_element *&ptr3)
	{
		data_element *tmp = ptr1;
		ptr1 = ptr2;
		ptr2 = ptr3;
		ptr3 = tmp;
	}
	data_element levenshtein_parallel(const data_element *s, size_t s_size, const data_element *t, size_t t_size)
	{
		if (s_size < t_size)
		{
			std::swap(s, t);
			std::swap(s_size, t_size);
		}

		size_t diag_size = t_size;
		data_element *diag1 = new data_element[diag_size + 1];
		data_element *diag2 = new data_element[diag_size + 1];
		data_element *diag3 = new data_element[diag_size + 1];

		// total number of diagonals
		size_t total_diags = s_size + t_size + 1;

		// init
		diag2[0] = 0;
		diag1[0] = 1;
		diag1[1] = 1;		// step=1

		for (size_t step = 2; step <= diag_size; ++step)
		{
			for (data_element i = 1; i < step; ++i)
			{
				data_element i1 = i - 1;
				char cost = (s[step - 1 - i] == t[i1]) ? 0 : 1;

				data_element upper = diag1[i1];
				data_element left = diag1[i];
				data_element upperleft = diag2[i1];

				diag3[i] = smaller<data_element>(upper + 1, left + 1, upperleft + cost);
			}

			diag3[0] = step;
			diag3[step] = step;

			rotate(diag2, diag1, diag3);
		}


		size_t full_diag_count = total_diags - diag_size;

		for (size_t step = diag_size + 1; step <= full_diag_count; ++step)
		{
			for (data_element i = 1; i <= diag_size; ++i)
			{
				data_element i1 = i - 1;
				char cost = (s[step - i - 1] == t[i1]) ? 0 : 1;

				data_element upper = diag1[i];
				data_element left = diag1[i1];
				data_element upperleft = diag2[i1];

				diag3[i] = smaller<data_element>(upper + 1, left + 1, upperleft + cost);
			}

			diag3[0] = step;

			rotate(diag2, diag1, diag3);
		}

		size_t err = 0;
		for (size_t step = full_diag_count + 1; step < total_diags; ++step)
		{
			for (data_element i = 1; i <= total_diags - step; ++i)
			{
				char cost = (s[s_size - i] == t[step - full_diag_count - 1 + i]) ? 0 : 1;

				data_element upper = diag1[i + 1];
				data_element left = diag1[i];
				data_element upperleft = diag2[i + err];

				diag3[i] = smaller<data_element>(upper + 1, left + 1, upperleft + cost);
			}

			diag3[0] = step;
			err = 1;

			// WARNNING: do not rotate if nothing has been done
			if (total_diags - step > 1)
				rotate(diag2, diag1, diag3);
		}

		data_element res = diag3[1];

		delete diag1;
		delete diag2;
		delete diag3;

		return res;
	}
	data_element compute_diagonal()
	{
		const int diag_count = 6;
		int diag_len = 1;
		int step = 1;

		size_t diag_size = size1;
		diag1 = new data_element[diag_size + 1];
		diag2 = new data_element[diag_size + 1];
		diag3 = new data_element[diag_size + 1];

		// total number of diagonals
		int total_diags = size1 + size2 + 1;

		// init
		diag2[0] = 0;
		diag1[0] = 1;
		diag1[1] = 1;

		for (int d = 2; d < total_diags; ++d)
		{	// process each diagonal

			diag3[0] = diag_len;	// ??
			for (int i = 1; i < diag_len; ++i)
			{	// process one diagonal
				int i1 = i - 1;
				char cost = (str1[step - 1 - i] == str2[i1]) ? 0 : 1;

				data_element upper = diag1[i1];
				data_element left = diag1[i];
				data_element upperleft = diag2[i1];

				diag3[i] = smaller<data_element>(upper + 1, left + 1, upperleft + cost);
			}
			diag3[diag_len] = diag_len;	// ??

			// rotate diagonals
			auto tmp = diag2;
			diag2 = diag1;
			diag1 = diag3;
			diag3 = tmp;
		}

		/*for (int step = 2; step <= diag_size; ++step)
		{
			for (int i = 1; i < step; ++i)
			{
				int i1 = i - 1;
				char cost = (str1[step - 1 - i] == str2[i1]) ? 0 : 1;

				data_element upper = diag1[i1];
				data_element left = diag1[i];
				data_element upperleft = diag2[i1];

				diag3[i] = smaller<data_element>(upper + 1, left + 1, upperleft + cost);
			}

			diag3[0] = step;
			diag3[step] = step;

			for (int i = 0; i <= step; ++i)
				std::cout << diag3[i] << " ";
			std::cout << std::endl;

			auto tmp = diag2;
			diag2 = diag1;
			diag1 = diag3;
			diag3 = tmp;
		}*/

		return diag1[1];
	}

	data_element compute_chunks()
	{
		int chunk1 = 2;
		int chunk2 = 2;

		left = new data_element[size1 + 1];
		for (int i = 0; i <= size1; ++i)
			left[i] = i;
		top = new data_element[size2 + 1];
		for (int i = 0; i <= size2; ++i)
			top[i] = i;




		return 0;
	}

	data_element compute()
	{
		/*int count = 0;
		for (int i = 0; i < size1; ++i)
		{
			for (int j = 0; j < size2; ++j)
			{
				count += str1[i] == str2[j] ? 1 : 0;
			}
		}
		return count;*/

		//return compute_two_rows_skip();
		//return levenshtein_parallel(str1, size1, str2, size2);
		//return compute_matrix2();

		data_element res1 = compute_two_rows();
		data_element res2 = compute_two_rows_skip();
		if (res1 != res2)
			std::cout << "error" << std::endl;
		//assert(res1 == res2);

		return res2;
	}
};

#endif
