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

	const char16_t F = 0x8888;

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
	_vsize(m), _hsize(n), data_size(m * n / sizeof(chunk_t)), data(new chunk_t[data_size])	// todo: FIX
{

}

matrix::~matrix()
{
	delete data;
}