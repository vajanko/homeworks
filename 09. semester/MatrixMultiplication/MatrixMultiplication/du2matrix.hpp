#ifndef matrix_h_
#define matrix_h_

#include <cstddef>
#include <string>
#include <memory>

#define CHUNK_W 2;
#define CHUNK_H 2;

typedef uint64_t chunk;

#define MULTIPLY(res_ptr, in_a, in_b) (*res_ptr |= in_a & in_b)
#define BIT(in, pos) ((in & (1 << pos)) >> pos)

class matrix {
private:
	std::unique_ptr<chunk> _data;

public:

	static std::string name() { return "matrix"; }

	std::size_t byte_size() const;

	matrix(std::size_t m, std::size_t n)
		// TODO: somehow calculate the size of matrix
		: _data(new chunk[m * n / sizeof(chunk)]) {}

	std::size_t vsize() const
	{
		return 0;
	}

	std::size_t hsize() const
	{
		return 0;
	}

	void set(std::size_t i, std::size_t j, bool e)
	{

	}

	bool get(std::size_t i, std::size_t j) const
	{
		return false;
	}

	void assign_mul(const matrix & a, const matrix & b)
	{
	}
};

#endif
