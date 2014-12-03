#ifndef matrix_h_
#define matrix_h_

#include <cstddef>
#include <string>
//#include <xmmintrin.h>
#include <emmintrin.h>

typedef bool chunk_1;
typedef unsigned short chunk_16;
typedef __int32 chunk_32;
typedef unsigned long long chunk_64;

// type used by the matrix
typedef chunk_64 chunk_t;


//#define CHUNK_MUL(out, a, b) (out |= a & b)
//#define ASSIGN_ZERO(out) (out = 0)

//__m128i sse_trans_slice(__m128i x)
//{
//	union { unsigned short s[8]; __m128i m; } u;
//	int    i;
//	for (i = 0; i < 8; ++i) {
//		u.s[7 - i] = _mm_movemask_epi8(x);
//		x = _mm_slli_epi64(x, 1);
//	}
//	return  u.m;
//}




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
