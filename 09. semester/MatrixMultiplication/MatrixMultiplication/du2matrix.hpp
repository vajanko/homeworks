#ifndef matrix_h_
#define matrix_h_

#include <cstddef>
#include <string>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <immintrin.h>
#include <array>

typedef bool chunk_1;
typedef unsigned char chunk_8;
typedef unsigned short chunk_16;
typedef unsigned __int64 chunk_64;
typedef __m128i chunk_128;
typedef __m256i chunk_256;

// type used by the matrix
typedef chunk_64 chunk_t;


class matrix {
private:
	bool trans = false;
	std::size_t data_size;
	chunk_t *data;
	std::size_t _vsize;
	std::size_t _hsize;

public:
	static std::string name() { return "matrix"; }

	matrix(std::size_t m, std::size_t n);
	~matrix();

	std::size_t byte_size() const;
	std::size_t vsize() const;
	std::size_t hsize() const;
	void set(std::size_t i, std::size_t j, bool e);
	bool get(std::size_t i, std::size_t j) const;

	void assign_mul(const matrix & a, const matrix & b);
};

#endif
