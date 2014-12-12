#include "du2matrix.hpp"

template<typename T>
void chunk_mul(T &a, T &bt, T &res) { };
template<typename T>
void matrix_mul(T *a, T *bt, T *res, std::size_t dim1, std::size_t dim2, std::size_t dim3) { };
template<typename T>
bool get_value(T *data, std::size_t cols, std::size_t i, std::size_t j) { return false; }
template<typename T>
void set_value(T *data, std::size_t cols, std::size_t i, std::size_t j, bool e) { }

template<>
void matrix_mul<chunk_1>(chunk_1 *a, chunk_1 *b, chunk_1 *res, std::size_t dim1, std::size_t dim2, std::size_t dim3)
{
	for (std::size_t i = 0; i < dim1; ++i)
	{
		for (std::size_t j = 0; j < dim2; ++j)
		{
			res[i * dim1 + j] = false;
			for (std::size_t k = 0; k < dim3; ++k)
				res[i * dim1 + j] |= a[i * dim1 + k] & b[k * dim2 + j];
		}
	}
}
template<>
bool get_value<chunk_1>(chunk_1 *data, std::size_t cols, std::size_t i, std::size_t j) { return data[i * cols + j]; }
template<>
void set_value<chunk_1>(chunk_1 *data, std::size_t cols, std::size_t i, std::size_t j, bool e) { data[i * cols + j] = e; }


template<>
void chunk_mul<chunk_16>(chunk_16 &a, chunk_16 &b, chunk_16 &res)
{
	// prepare A
	chunk_16 A = 0;
	chunk_16 L = 0xf000;
	chunk_16 R = 0x0000;
	chunk_16 ch_bit = 0x0800;

	for (short i = 0; i < 4; ++i)
	{
		A |= (a & L) << i;
		A |= (a & R) >> (4 - i);

		L = (L >> 4) ^ ch_bit;
		R = (R >> 4) ^ ch_bit;
		ch_bit >>= 5;
	}

	const chunk_16 F = 0x8888;

	for (short i = 0; i < 4; ++i)
	{
		chunk_16 t1 = A & F;
		t1 |= t1 >> 1;
		t1 |= t1 >> 2;

		res |= t1 & b;
		b = _rotl16(b, 4);
		A <<= 1;
	}
}
template<>
void matrix_mul<chunk_16>(chunk_16 *a, chunk_16 *b, chunk_16 *res, std::size_t dim1, std::size_t dim2, std::size_t dim3)
{
	dim1 >>= 2;
	dim2 >>= 2;
	dim3 >>= 2;
	for (std::size_t i = 0; i < dim1; ++i)
	{
		for (std::size_t j = 0; j < dim2; ++j)
		{
			res[i * dim1 + j] = 0;
			for (std::size_t k = 0; k < dim3; ++k)
				chunk_mul(a[i * dim1 + k], b[k * dim2 + j], res[i * dim1 + j]);	
		}
	}
}
template<>
bool get_value<chunk_16>(chunk_16 *data, std::size_t cols, std::size_t i, std::size_t j) 
{
	chunk_16 block = data[i / 4 * cols / 4 + j / 4];
	chunk_16 res = block & (0x8000 >> (i % 4 * 4 + j % 4));
	
	return res != 0;
}
template<>
void set_value<chunk_16>(chunk_16 *data, std::size_t cols, std::size_t i, std::size_t j, bool e) 
{
	chunk_16 block = data[i / 4 * cols / 4 + j / 4];
	chunk_16 res = 0x8000 >> (i % 4 * 4 + j % 4);
	if (!e)
		data[i / 4 * cols / 4 + j / 4] &= ~res;
	else
		data[i / 4 * cols / 4 + j / 4] |= res;
}

template<>
void chunk_mul<chunk_64>(chunk_64 &a, chunk_64 &b, chunk_64 &res)
{
	/*chunk_64 A = 0;
	chunk_64 L = 0xff00000000000000;
	chunk_64 R = 0x0000000000000000;
	chunk_64 ch_bit = 0x0080000000000000;

	for (short i = 0; i < 8; ++i)
	{
		A |= (a & L) << i;
		A |= (a & R) >> (8 - i);

		L = (L >> 8) ^ ch_bit;
		R = (R >> 8) ^ ch_bit;
		ch_bit >>= 9;
	}*/

	// it would be nice if this could be done in parallel using sse instructions
	// TODO: notice that this matrix can be cached and used multiple times
	union { chunk_8 u[8]; chunk_64 m; } T;
	T.m = a;
	for (chunk_8 i = 1; i < 8; ++i)
		T.u[7 - i] = _rotl8(T.u[7 - i], i);
	chunk_64 A = T.m;

	static const chunk_64 F = 0x8080808080808080;

	for (short i = 0; i < 8; ++i)
	{
		chunk_64 t1 = A & F;
		t1 |= t1 >> 1; t1 |= t1 >> 2; t1 |= t1 >> 4;

		res |= t1 & b;
		b = _rotl64(b, 8);
		A <<= 1;
	}
}

// this function differs from the previous in that matrix a lines are already rotated
void chunk_mul2(chunk_64 a, chunk_64 b, chunk_64 &res)
{
	static const chunk_64 F = 0x8080808080808080;

	for (short i = 0; i < 8; ++i)
	{
		chunk_64 t1 = a & F;
		t1 |= t1 >> 1; t1 |= t1 >> 2; t1 |= t1 >> 4;

		res |= t1 & b;
		b = _rotl64(b, 8);
		a <<= 1;
	}
}

//template<>
//void matrix_mul<chunk_64>(chunk_64 *a, chunk_64 *b, chunk_64 *res, std::size_t dim1, std::size_t dim2, std::size_t dim3)
//{
//	dim1 >>= 3;		// divide by 8
//	dim2 >>= 3;
//	dim3 >>= 3;
//	for (std::size_t i = 0; i < dim1; ++i)
//	{
//		for (std::size_t j = 0; j < dim2; ++j)
//		{
//			// function call solution
//			/*res[i * dim1 + j] = 0;
//			for (std::size_t k = 0; k < dim3; ++k)
//				chunk_mul(a[i * dim1 + k], b[k * dim2 + j], res[i * dim1 + j]);*/
//
//			chunk_64 r = 0;
//			for (std::size_t k = 0; k < dim3; ++k)
//			{
//				chunk_64 A = 0;
//				chunk_64 L = 0xff00000000000000;
//				chunk_64 R = 0x0000000000000000;
//				chunk_64 ch_bit = 0x0080000000000000;
//
//				chunk_64 a1 = a[i * dim1 + k];
//
//				// rotate list of a1 chunk-matrix
//				for (short i = 0; i < 8; ++i)
//				{
//					A |= (a1 & L) << i;
//					A |= (a1 & R) >> (8 - i);
//
//					L = (L >> 8) ^ ch_bit;
//					R = (R >> 8) ^ ch_bit;
//					ch_bit >>= 9;
//				}
//
//				const chunk_64 F = 0x8080808080808080;
//				chunk_64 b1 = b[k * dim2 + j];
//
//				for (short i = 0; i < 8; ++i)
//				{
//					chunk_64 t1 = A & F; t1 |= t1 >> 1; t1 |= t1 >> 2; t1 |= t1 >> 4;
//
//					r |= t1 & b1;
//					b1 = _rotl64(b1, 8);
//					A <<= 1;
//				}
//			}
//			res[i * dim1 + j] = r;
//		}
//	}
//}
template<>
void matrix_mul<chunk_64>(chunk_64 *a, chunk_64 *b, chunk_64 *res, std::size_t dim1, std::size_t dim2, std::size_t dim3)
{
	static const chunk_64 F = 0x8080808080808080;

	dim1 >>= 3;		// divide by 8
	dim2 >>= 3;
	dim3 >>= 3;

	for (std::size_t i = 0; i < dim1; ++i)
	{
		// notice that j and k indices are switch - this way rotate line of the left matrix only once
		for (std::size_t k = 0; k < dim3; ++k)
		{
			// prepare a[i * dim1 + k]
			// it would be nice if this could be done in parallel using sse instructions (but it can not)
			union { chunk_64 m; chunk_8 u[8]; } T;
			T.m = a[i * dim1 + k];
			for (chunk_8 x = 1; x < 8; ++x)
				T.u[7 - x] = _rotl8(T.u[7 - x], x);

			for (std::size_t j = 0; j < dim2; ++j)
			{
				chunk_64 A = T.m;

				if (k == 0)
					res[i * dim1 + j] = 0;

				chunk_64 B = b[k * dim2 + j];
				chunk_64 &R = res[i * dim1 + j];

				for (short i = 0; i < 8; ++i)
				{
					chunk_64 t1 = A & F;
					t1 |= t1 >> 1; t1 |= t1 >> 2; t1 |= t1 >> 4;

					R |= t1 & B;
					B = _rotl64(B, 8);
					A <<= 1;
				}
			}
		}
		//for (std::size_t j = 0; j < dim2; ++j)
		//{
		//	// function call solution - working
		//	res[i * dim1 + j] = 0;
		//	for (std::size_t k = 0; k < dim3; ++k)
		//		chunk_mul(a[i * dim1 + k], b[k * dim2 + j], res[i * dim1 + j]);
		//}
	}

	/*chunk_128 *aa = (chunk_128 *)a;
	chunk_128 *bb = (chunk_128 *)b;
	chunk_128 *rr = (chunk_128 *)res;
	for (std::size_t i = 0; i < dim1; ++i)
  //for (std::size_t j = 0; j < dim2; ++j)
	{
		for (std::size_t j = 0; j < dim2; j += 2)
    //for (std::size_t i = 0; i < dim1; ++i)
		{
			chunk_128 &r1 = rr[i * dim1 / 2 + j / 2];
			r1 = _mm_setzero_si128();
			for (std::size_t k = 0; k < dim3; ++k)
			{
				chunk_128 a1 = aa[i * dim1 / 2 + k / 2];
				chunk_128 b1 = bb[k * dim2 / 2 + j / 2];

				chunk_mul(a1.m128i_u64[k % 2], b1.m128i_u64[0], r1.m128i_u64[0]);
				chunk_mul(a1.m128i_u64[k % 2], b1.m128i_u64[1], r1.m128i_u64[1]);
			}
		}
	}*/
}
template<>
bool get_value<chunk_64>(chunk_64 *data, std::size_t cols, std::size_t i, std::size_t j)
{
	chunk_64 block = data[i / 8 * cols / 8 + j / 8];
	chunk_64 res = block & (0x8000000000000000 >> (i % 8 * 8 + j % 8));

	return res != 0;
}
template<>
void set_value<chunk_64>(chunk_64 *data, std::size_t cols, std::size_t i, std::size_t j, bool e)
{
	chunk_64 block = data[i / 8 * cols / 8 + j / 8];
	chunk_64 res = 0x8000000000000000 >> (i % 8 * 8 + j % 8);
	if (!e)
		data[i / 8 * cols / 8 + j / 8] &= ~res;
	else
		data[i / 8 * cols / 8 + j / 8] |= res;
}

//template<>
//void chunk_mul<chunk_256>(chunk_256 &a, chunk_256 &b, chunk_256 &res)
//{
//	chunk_256 A = 0;
//	chunk_256 L = 0xff00000000000000;
//	chunk_256 R = 0x0000000000000000;
//	chunk_256 ch_bit = 0x0080000000000000;
//
//	for (short i = 0; i < 8; ++i)
//	{
//		A |= (a & L) << i;
//		A |= (a & R) >> (8 - i);
//
//		L = (L >> 8) ^ ch_bit;
//		R = (R >> 8) ^ ch_bit;
//		ch_bit >>= 9;
//	}
//
//	const chunk_256 F = 0x8080808080808080;
//
//	for (short i = 0; i < 8; ++i)
//	{
//		chunk_256 t1 = A & F;
//		t1 |= t1 >> 1; t1 |= t1 >> 2; t1 |= t1 >> 4;
//
//		res |= t1 & b;
//		b = _rotl64(b, 8);
//		A <<= 1;
//	}
//}
//template<>
//void matrix_mul<chunk_256>(chunk_256 *a, chunk_256 *b, chunk_256 *res, std::size_t dim1, std::size_t dim2, std::size_t dim3)
//{
//	dim1 >>= 4;		// divide by 16
//	dim2 >>= 4;
//	dim3 >>= 4;
//	for (std::size_t i = 0; i < dim1; ++i)
//	{
//		for (std::size_t j = 0; j < dim2; ++j)
//		{
//			// function call solution
//			/*res[i * dim1 + j] = 0;
//			for (std::size_t k = 0; k < dim3; ++k)
//			chunk_mul(a[i * dim1 + k], b[k * dim2 + j], res[i * dim1 + j]);*/
//
//			chunk_256 r = 0;
//			for (std::size_t k = 0; k < dim3; ++k)
//			{
//				chunk_256 A = 0;
//				chunk_256 L = 0xff00000000000000;
//				chunk_256 R = 0x0000000000000000;
//				chunk_256 ch_bit = 0x0080000000000000;
//
//				chunk_256 a1 = a[i * dim1 + k];
//
//				// rotate list of a1 chunk-matrix
//				for (short i = 0; i < 8; ++i)
//				{
//					A |= (a1 & L) << i;
//					A |= (a1 & R) >> (8 - i);
//
//					L = (L >> 8) ^ ch_bit;
//					R = (R >> 8) ^ ch_bit;
//					ch_bit >>= 9;
//				}
//
//				const chunk_256 F = 0x8080808080808080;
//				chunk_256 b1 = b[k * dim2 + j];
//
//				for (short i = 0; i < 8; ++i)
//				{
//					chunk_256 t1 = A & F; t1 |= t1 >> 1; t1 |= t1 >> 2; t1 |= t1 >> 4;
//
//					r |= t1 & b1;
//					b1 = _rotl64(b1, 8);
//					A <<= 1;
//				}
//			}
//			res[i * dim1 + j] = r;
//		}
//	}
//}
template<>
bool get_value<chunk_256>(chunk_256 *data, std::size_t cols, std::size_t i, std::size_t j)
{
	chunk_256 block = data[i / 16 * cols / 16 + j / 16];
	chunk_256 mask = _mm256_setr_epi64x(0x8000000000000000, 0x0, 0x0, 0x0);
	
		//(0x8000000000000000 >> (i % 8 * 8 + j % 16));
	//chunk_256 res = _mm_and_si128(block, mask);

	//return res.m128i_u64[0] != 0 && res.m128i_u64[1] != 0;
	return false;
}
//template<>
//void set_value<chunk_256>(chunk_256 *data, std::size_t cols, std::size_t i, std::size_t j, bool e)
//{
//	chunk_256 block = data[i / 8 * cols / 16 + j / 16];
//	//chunk_64 res = 0x8000000000000000 >> (i % 8 * 8 + j % 8);
//	chunk_256 res;
//	if (!e)
//		data[i / 8 * cols / 8 + j / 16] = _mm_andnot_si128(data[i / 8 * cols / 8 + j / 16], res);
//	//data[i / 8 * cols / 8 + j / 16] &= ~res;
//	else
//		data[i / 8 * cols / 8 + j / 16] = _mm_or_si128(data[i / 8 * cols / 8 + j / 16], res);
//		//data[i / 8 * cols / 8 + j / 16] |= res;
//}

std::size_t matrix::byte_size() const { return data_size * sizeof(chunk_t); }
std::size_t matrix::vsize() const { return _vsize; }
std::size_t matrix::hsize() const { return _vsize; }
void matrix::set(std::size_t i, std::size_t j, bool e)
{
	set_value<chunk_t>(data, hsize(), i, j, e);
}
bool matrix::get(std::size_t i, std::size_t j) const
{
	return get_value<chunk_t>(data, hsize(), i, j);
}
void matrix::assign_mul(const matrix & a, const matrix & b)
{
	matrix_mul(a.data, b.data, data, a.hsize(), a.vsize(), b.vsize());
}

matrix::matrix(std::size_t m, std::size_t n) : 
	_vsize(m), _hsize(n), 
	data_size((m / sizeof(chunk_t) + (m % sizeof(chunk_t) > 0 ? 1 : 0)) *
		(n / sizeof(chunk_t) + (n % sizeof(chunk_t) > 0 ? 1 : 0))
	),
	data(new chunk_t[data_size])
{
	
}

matrix::~matrix()
{
	if (data != nullptr)
	{
		delete data;
		data = nullptr;
	}
}