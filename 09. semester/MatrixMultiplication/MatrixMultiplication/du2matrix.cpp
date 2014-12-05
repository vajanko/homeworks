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
	union { chunk_64 m; chunk_8 u[8]; } T;
	T.m = a;
	for (chunk_8 i = 1; i < 8; ++i)
		T.u[i] = _rotl8(T.u[i], i);
	chunk_64 A = T.m;

	const chunk_64 F = 0x8080808080808080;

	for (short i = 0; i < 8; ++i)
	{
		chunk_64 t1 = A & F;
		t1 |= t1 >> 1; t1 |= t1 >> 2; t1 |= t1 >> 4;

		res |= t1 & b;
		b = _rotl64(b, 8);
		A <<= 1;
	}

	//static const chunk_64 D = 0x8040201008040201;	// diagonal
	//static const chunk_64 UD = 0x80C0E0F0F8FCFE;	// ones under the diagonal
	//static const chunk_64 AD = 0x7F3F1F0F07030100;	// ones above the diagonal

	//for (short i = 0; i < 8; ++i)
	//{

	//	chunk_64 A = a & D;
	//	A = A & (A >> 1);			// no oveflows
	//	A = A & ((A >> 2) & UD);	// clear overflows from the upper line
	//	A = A & ((A >> 4) & UD);	// clear overflows from the upper line
	//	A = A & (A << 1);			// no overflows
	//	A = A & ((A << 2) & AD);	// clear overflows from the lower line
	//	A = A & ((A << 4) & AD);	// clear overflows from the lower line

	//	// now A is a matrix 8x8 with 8 ones on the line when there is 1 on the diagonal
	//	// otherwise there are zeros


	//	b = _rotl64(b, 8);
	//}
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
	dim1 >>= 3;		// divide by 8
	dim2 >>= 3;
	dim3 >>= 3;

	chunk_128 *aa = (chunk_128 *)a;
	chunk_128 *bb = (chunk_128 *)b;
	chunk_128 *rr = (chunk_128 *)res;
	for (std::size_t i = 0; i < dim1; ++i)
	{
		//for (std::size_t j = 0; j < dim2; j += 2)
		for (std::size_t j = 0; j < dim2; ++j)
		{
			// function call solution
			res[i * dim1 + j] = 0;
			for (std::size_t k = 0; k < dim3; ++k)
				chunk_mul(a[i * dim1 + k], b[k * dim2 + j], res[i * dim1 + j]);
			/*res[i * dim1 + j] = 0;
			res[(i + 1) * dim1 + j] = 0;*/
			//rr[i * dim1 + j] = _mm_setzero_si128();
			//for (std::size_t k = 0; k < dim3; k += 2)
			//{
			//	chunk_mul(aa[i * dim1 + k].m128i_u64[0], bb[k * dim2 + j].m128i_u64[0], rr[i * dim1 + j].m128i_u64[0]);
			//	chunk_mul(aa[i * dim1 + k].m128i_u64[1], bb[k * dim2 + j].m128i_u64[1], rr[i * dim1 + j].m128i_u64[1]);
			//	chunk_mul(aa[i * dim1 + k].m128i_u64[0], bb[k * dim2 + j].m128i_u64[1], rr[i * dim1 + j].m128i_u64[0]);
			//	chunk_mul(aa[i * dim1 + k].m128i_u64[1], bb[k * dim2 + j].m128i_u64[0], rr[i * dim1 + j].m128i_u64[1]);
			//	//chunk_mul(a[i * dim1 + k], b[k * dim2 + j], res[i * dim1 + j]);
			//}
		}
	}
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
//void chunk_mul<chunk_128>(chunk_128 &a, chunk_128 &b, chunk_128 &res)
//{
//	chunk_128 A = 0;
//	chunk_128 L = 0xff00000000000000;
//	chunk_128 R = 0x0000000000000000;
//	chunk_128 ch_bit = 0x0080000000000000;
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
//	const chunk_128 F = 0x8080808080808080;
//
//	for (short i = 0; i < 8; ++i)
//	{
//		chunk_128 t1 = A & F;
//		t1 |= t1 >> 1; t1 |= t1 >> 2; t1 |= t1 >> 4;
//
//		res |= t1 & b;
//		b = _rotl64(b, 8);
//		A <<= 1;
//	}
//}
//template<>
//void matrix_mul<chunk_128>(chunk_128 *a, chunk_128 *b, chunk_128 *res, std::size_t dim1, std::size_t dim2, std::size_t dim3)
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
//			chunk_mul(a[i * dim1 + k], b[k * dim2 + j], res[i * dim1 + j]);*/
//
//			chunk_128 r = 0;
//			for (std::size_t k = 0; k < dim3; ++k)
//			{
//				chunk_128 A = 0;
//				chunk_128 L = 0xff00000000000000;
//				chunk_128 R = 0x0000000000000000;
//				chunk_128 ch_bit = 0x0080000000000000;
//
//				chunk_128 a1 = a[i * dim1 + k];
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
//				const chunk_128 F = 0x8080808080808080;
//				chunk_128 b1 = b[k * dim2 + j];
//
//				for (short i = 0; i < 8; ++i)
//				{
//					chunk_128 t1 = A & F; t1 |= t1 >> 1; t1 |= t1 >> 2; t1 |= t1 >> 4;
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
//template<>
//bool get_value<chunk_128>(chunk_128 *data, std::size_t cols, std::size_t i, std::size_t j)
//{
//	chunk_128 block = data[i / 8 * cols / 8 + j / 16];
//	chunk_128 mask = 
//		//(0x8000000000000000 >> (i % 8 * 8 + j % 16));
//	chunk_128 res = _mm_and_si128(block, mask);
//
//	return res.m128i_u64[0] != 0 && res.m128i_u64[1] != 0;
//}
//template<>
//void set_value<chunk_128>(chunk_128 *data, std::size_t cols, std::size_t i, std::size_t j, bool e)
//{
//	chunk_128 block = data[i / 8 * cols / 16 + j / 16];
//	//chunk_64 res = 0x8000000000000000 >> (i % 8 * 8 + j % 8);
//	chunk_128 res;
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
	delete data;
}