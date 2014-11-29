#ifndef matrix_h_
#define matrix_h_

#include <cstddef>
#include <string>
#include <xmmintrin.h>

class matrix {
private:

public:
	static std::string name() { return "matrix"; }

	std::size_t byte_size() const
	{
		return 0;
	}

	matrix( std::size_t m, std::size_t n)
	{
	}

	std::size_t vsize() const
	{
		return 0;
	}

	std::size_t hsize() const
	{
		return 0;
	}

	void set( std::size_t i, std::size_t j, bool e)
	{
	}

	bool get( std::size_t i, std::size_t j) const
	{
		return false;
	}

	void assign_mul( const matrix & a, const matrix & b)
	{
	}
};

#endif
