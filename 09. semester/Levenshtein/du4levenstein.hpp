#ifndef du4levenstein_hpp_
#define du4levenstein_hpp_

#include <iostream>
#include <assert.h>
#include <bitset>

typedef unsigned int uint32;
typedef unsigned long long uint64;

template<typename T> inline T higher(T x, T y) { return (x>y) ? x : y; }
template<typename T> inline T smaller(T x, T y) { return (x<y) ? x : y; }
template<typename T> inline T smaller(T x, T y, T z) { return smaller<T>(smaller<T>(x, y), z); }

//template<std::size_t len>
//class big_int
//{
//private:
//	uint64_t data[len];
//
//public:
//	/*big_int operator&(const big_int &other) { return big_int(data & other.data); }
//	big_int operator|(const big_int &other) { return big_int(data | other.data); }
//	big_int operator^(const big_int &other) { return big_int(data ^ other.data); }
//	big_int operator~() { return big_int(~data); }*/
//
//	big_int(uint64_t d[len]) : data(d) { }
//	big_int() : data() { }
//};

class levenstein 
{
public:
	typedef int data_element;

private:
	data_element *str1;
	std::size_t size1;

	data_element *str2;
	std::size_t size2;

	data_element *__restrict dst1;
	data_element *__restrict dst2;
	data_element *__restrict dst3;

	data_element *diag1;
	data_element *diag2;
	data_element *diag3;

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
			dst3[i] = i;
			dst2[i] = i;
			dst1[i] = i;
		}
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
	void print_diagonal(data_element *diag, int size)
	{
		std::cout << std::endl;
		for (int i = 0; i <= size; ++i)
			std::cout << diag[i] << " ";
		std::cout << std::endl;
	}
	
	void rotate(data_element *&ptr1, data_element *&ptr2, data_element *&ptr3)
	{
		data_element *tmp = ptr1;
		ptr1 = ptr2;
		ptr2 = ptr3;
		ptr3 = tmp;
	}
	data_element compute_diagonals(const data_element *s, size_t s_size, const data_element *t, size_t t_size)
	{
		/*if (s_size < t_size)
		{
			std::swap(s, t);
			std::swap(s_size, t_size);
		}*/

		size_t diag_size = t_size;
		
		data_element * __restrict diag1 = this->dst1;
		data_element * __restrict diag2 = this->dst2;
		data_element * __restrict diag3 = this->dst3;

		// total number of diagonals
		size_t total_diags = s_size + t_size + 1;

		// init
		diag2[0] = 0;
		diag1[0] = 1;
		diag1[1] = 1;		// step=1

		for (size_t step = 2; step <= diag_size; ++step)
		{
			diag3[0] = step;
			for (data_element i = 1; i < step; ++i)
			{
				data_element i1 = i - 1;
				char cost = (s[step - 1 - i] == t[i1]) ? 0 : 1;

				data_element upper = diag1[i1];
				data_element left = diag1[i];
				data_element upperleft = diag2[i1];

				diag3[i] = smaller<data_element>(upper + 1, left + 1, upperleft + cost);
			}
			diag3[step] = step;

			rotate(diag2, diag1, diag3);
		}


		size_t full_diag_count = total_diags - diag_size;

		for (size_t step = diag_size + 1; step <= full_diag_count; ++step)
		{
			diag3[0] = step;
			for (data_element i = 1; i <= diag_size; ++i)
			{
				data_element i1 = i - 1;
				char cost = (s[step - i - 1] == t[i1]) ? 0 : 1;

				data_element upper = diag1[i];
				data_element left = diag1[i1];
				data_element upperleft = diag2[i1];

				diag3[i] = smaller<data_element>(upper + 1, left + 1, upperleft + cost);
			}

			rotate(diag2, diag1, diag3);
		}

		size_t err = 0;
		for (size_t step = full_diag_count + 1; step < total_diags; ++step)
		{
			diag3[0] = step;
			for (data_element i = 1; i <= total_diags - step; ++i)
			{
				char cost = (s[s_size - i] == t[step - full_diag_count - 1 + i]) ? 0 : 1;

				data_element upper = diag1[i + 1];
				data_element left = diag1[i];
				data_element upperleft = diag2[i + err];

				diag3[i] = smaller<data_element>(upper + 1, left + 1, upperleft + cost);
			}

			err = 1;

			// WARNNING: do not rotate if nothing has been done
			if (total_diags - step > 1)
				rotate(diag2, diag1, diag3);
		}

		data_element res = diag3[1];

		/*delete diag1;
		delete diag2;
		delete diag3;*/

		return res;
	}
	
	int compute_bits(const data_element *str1, int size1, const data_element *str2, int size2)
	{
		// based on this article: http://www.sis.uta.fi/~hh56766/pubs/psc02.pdf
		int res = size1;
		uint64 vp = (uint64)-1;		// see page 4.
		uint64 vn = 0;
		uint64 hp = 0;
		uint64 hn = 0;
		// number of bits used for the matrix
		int bit_len = smaller<int>(size2, sizeof(uint64) * 8);

		for (int j = 0; j < size1; j++)
		{
			uint64 pm = 0;
			for (int i = bit_len - 1; i >= 0; i--) 
			{
				pm <<= 1;
				pm |= str2[i] == str1[j];
			}
			uint64 d = (((pm & vp) + vp)) ^ vp | pm | vn;
			hp = (vn | ~(d | vp));
			uint64 hpw = (hp << 1) | 1;
			hn = d & vp;
			vp = (hn << 1) | ~(d | hpw);
			vn = d & hpw;
		}

		// compute final results
		for (int i = 0; i < bit_len; i++) 
		{
			// sum added and removed
			res += (vp & 1) - (vn & 1);
			vp >>= 1;
			vn >>= 1;
		}
		return res;
	}
	void compute_bits(const data_element *str1, int size1, const data_element *str2, int size2, uint64 &vp, uint64 &vn, uint64 &hp, uint64 &hn)
	{
		// based on this article: http://www.sis.uta.fi/~hh56766/pubs/psc02.pdf
		
		// number of bits used for the matrix
		int bit_len = smaller<int>(size2, sizeof(uint64) * 8);

		for (int j = 0; j < size1; j++)
		{
			uint64 pm = 0;
			for (int i = bit_len - 1; i >= 0; i--)
			{
				pm <<= 1;
				pm |= str2[i] == str1[j];
			}
			uint64 d = (((pm & vp) + vp)) ^ vp | pm | vn;
			hp = (vn | ~(d | vp));
			uint64 hpw = (hp << 1) | 1;
			hn = d & vp;
			vp = (hn << 1) | ~(d | hpw);
			vn = d & hpw;
		}

		// final results are calculated at the end from the whole matrix
	}

	data_element compute()
	{	
		if (size2 <= 64)
		{
			return compute_bits(str1, size1, str2, size2);
			/*int res = compute_bits(str1, size1, str2, size2);
			int check = compute_diagonals(str1, size1, str2, size2);
			if (check != res)
				std::cout << "check: " << check << "result: " << res << std::endl;

			return res;*/
		}
		else
		{
			return compute_diagonals(str1, size1, str2, size2);

			/*int cols = size2 / 64;
			uint64 vp[2024];// = new uint64[2024];
			uint64 vn[2024];// = new uint64[2024];
			for (int i = 0; i < cols; ++i)
			{
				vp[i] = 0xffffffffffffffff;// (uint64)-1;
				vn[i] = 0;
			}
			uint64 hp = 0;
			uint64 hn = 0;

			int res = size1;

			for (int i = 0; i < size1; i += 64)
			{
				for (int j = 0; j < size2; j += 64)
				{
					compute_bits(str1 + i, 64, str2 + j, 64, vp[j / 64], vn[j / 64], hp, hn);
				}
			}

			// compute final results

			for (int j = 0; j < cols; ++j)
			{
				for (int i = 0; i < 64; ++i)
				{
					// sum added and removed
					res += (vp[j] & 1) - (vn[j] & 1);
					vp[j] >>= 1;
					vn[j] >>= 1;
				}
			}*/
		}
	}

	data_element compute_three_rows()
	{
		data_element *__restrict dst1 = this->dst1;
		data_element *__restrict dst2 = this->dst2;
		//data_element *__restrict dst3 = this->dst3;
		data_element dst3_tmp[1024 * 64];
		data_element *__restrict dst3 = dst3_tmp;

		/*for (int i = 0; i <= size2; ++i)
		{
			dst1[i] = i;
			dst2[i] = i;
			dst3[i] = i;
		}*/

		//for (int slice = 0; slice < size1 + size2 - 1; ++slice)
		//{
		//	int z1 = slice < size2 ? 0 : slice - size2 + 1;
		//	int z2 = slice < size1 ? 0 : slice - size1 + 1;

		//	dst3[0] = dst1[0] = smaller<int>(slice + 1, size1);
		//	for (int j = slice - z2; j >= z1; --j)
		//	{
		//		int a = j + 1;
		//		int b = slice - j + 1;

		//		int total = smaller<data_element>(dst2[b - 1] + 1, dst1[b - 1] + 1, dst2[b - 1] + ((str1[b - 1] == str2[a - 1]) ? 0 : 1));
		//		dst3[a] = total;
		//		//printf("%d,%d ", a, b);
		//	}
		//	//printf("\n");

		//	//print_diagonal(dst3, size2);
		//	std::swap(dst1, dst2);
		//	//std::copy_n(dst3, size2, dst1);	
		//}
		//return dst1[size2];

		int ln = 1;
		int count1 = size1 * size2;
		for (int x = 0; x < count1; )
		{
			int i = smaller<int>(ln, size1);

			dst1[0] = i;
			int count2 = smaller<int>(ln, size2);
			for (int j = higher<int>(1, ln - size1 + 1); j <= count2; ++j, --i, ++x)
			{	// these loops generate indices of the matrix passing through the diagonals
				dst1[j] = smaller<data_element>(dst2[j] + 1, dst1[j - 1] + 1, dst2[j - 1] + ((str1[i - 1] == str2[j - 1]) ? 0 : 1));
				//dst1[j] = smaller<data_element>(dst1[j] + 1, dst1[j - 1] + 1, dst2[j - 1] + ((str1[i - 1] == str2[j - 1]) ? 0 : 1));
				//std::cout << i << "," << j << " ";// << " -> " << dst3[j];
			}
			++ln;

			std::swap(dst1, dst2);
			/*auto tmp = dst2;
			dst2 = dst1;
			dst1 = dst3;
			dst3 = tmp;*/

			//print_diagonal(dst1, size2);
		}

		return dst2[size2];
	}
	data_element compute_two_rows_skip()
	{
		if (size1 == 0) return size2;
		if (size2 == 0) return size1;

		/*data_element dst3_tmp[1024];
		data_element *dst3 = dst3_tmp;*/

		int row = 0;
		int col = 0;

		while (true)	// TODO: break at the end of matrix
		{
			for (int i = col; i < size2; ++i)
				if (str1[row] == str2[i])
				{
					goto start;
				}

			//for (int i = row; i < size1; ++i)
			//	if (str1[i] == str2[col])
			//	{
			//		goto inc_row;
			//	}

			//++col;	// no match on the row
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
};

#endif
