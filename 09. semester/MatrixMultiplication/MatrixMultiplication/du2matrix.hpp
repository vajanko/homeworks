#ifndef matrix_h_
#define matrix_h_

#include <cstddef>
#include <string>

#define CHUNK_W 2;
#define CHUNK_H 2;

typedef long long int chunk;  // 64bits

class matrix {
public:

	static std::string name() { return "matrix"; }

  std::size_t byte_size( ) const;

  matrix( std::size_t m, std::size_t n );

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
