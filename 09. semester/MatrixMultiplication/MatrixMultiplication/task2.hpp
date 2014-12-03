#ifndef _TASK2_HPP
#define _TASK2_HPP

#include <cstddef>
#include <memory>
#include <vector>
#include <cassert>
#include <random>

#include "testbed.hpp"

#include <emmintrin.h>

///////////////////////////////

typedef bool matrix_element;

matrix_element matrix_element_add_mul( matrix_element c, matrix_element a, matrix_element b)
{
	return c | a & b;
}

template< typename G>
G grouped_matrix_element_add_mul( G c, G a, G b)
{
	return c | a & b;
}

__m128i grouped_matrix_element_add_mul( __m128i c, __m128i a, __m128i b)
{
	return _mm_or_si128( c, _mm_and_si128( a, b));
}

template< typename M>
unsigned long long chksum( const M & m)
{
	unsigned long long chksum = 0;

	for ( std::size_t i = 0; i < m.vsize(); ++ i)
		for ( std::size_t j = 0; j < m.hsize(); ++j)
		{
			bool x = m.get( i, j);

			chksum = chksum * 3 + x;
		}

	return chksum;
}


template< typename P, typename N>
void fill_matrix( P & gen, N & s)
{
	for ( std::size_t i = 0; i < s.vsize(); ++ i)
	{
		for ( std::size_t j = 0; j < s.hsize(); ++ j)
		{
			s.set( i, j, gen.get());
		}
	}
}

template< typename D, typename P>
struct generator_3 {

	typedef D data_type;
	typedef matrix_element check_type;	// a kind of checksum
		
	static std::string name() { return D::name() + "_" + P::name(); }

	/*
	std::string param() const 
	{ 
		return ulibpp::lexical_cast< std::string>( opsize_);
	}
	*/

	time_complexity complexity() const 
	{ 
		return opsize_ * opsize_ * opsize_;
	}

	check_type check() const
	{
		return 0;
	}

	const D & data() const
	{
		return d_;
	}

	template< typename GP>
	generator_3( const GP &, std::size_t opsize) 
		: opsize_( opsize),
		d_( opsize, opsize, opsize)
	{
		P gen;
		gen.reset();
		fill_matrix( gen, d_.a);
		fill_matrix( gen, d_.b);
	}

private:
	const std::size_t opsize_;
	D d_;
};

struct policy_zero {

	static std::string name() { return "zero"; }

	void reset()
	{
	}

	matrix_element get()
	{
		return 0;
	}
};

struct policy_one {

	static std::string name() { return "one"; }

	void reset()
	{
	}

	matrix_element get()
	{
		return 1;
	}
};

struct policy_random {

	static std::string name() { return "random"; }
		
	policy_random()
	: distribution( 0, 99)	// 1% filling
	{
	}

	void reset()
	{
		//srand( 729);
		engine.seed();
		distribution.reset();
	}

	matrix_element get()
	{
		//return std::rand() % 100 == 0;	// 1% filling
		return distribution( engine) == 0;
	}
private:
	std::mt19937 engine;
	std::uniform_int_distribution<> distribution;
};

template< typename N>
struct data_3 {
public:

	static std::string name() { return "matrix"; }

	std::size_t byte_size() const
	{
//		return a.byte_size() + b.byte_size() + c.byte_size();
		return 0;
	}

	data_3( std::size_t isize, std::size_t jsize, std::size_t ksize)
		: a( isize, jsize), b( jsize, ksize), c( isize, ksize)
	{
	}

	N a, b;
	mutable N c;
};

struct task_3 {

	static std::string name() { return "mul"; }

	template< bool cold, bool debug, typename D, typename C>
	static void run( const D & data, const C & check)
	{
		data.c.assign_mul( data.a, data.b);

		// assert( s == check);
	}


	template< bool debug, typename D, typename C>
	static void initial_check( logger & log, const D & data, const C & check)
	{
		//log.ss() << "CHKSUM A[" << data.a.vsize() << "," << data.a.hsize() << "] = " << chksum( data.a) << std::endl;
		//log.ss() << "CHKSUM B[" << data.b.vsize() << "," << data.b.hsize() << "] = " << chksum( data.b) << std::endl;
	}

	template< bool debug, typename D, typename C>
	static void final_check( logger & log, const D & data, const C & check)
	{
		//log.ss() << "CHKSUM C[" << data.c.vsize() << "," << data.c.hsize() << "] = " << chksum( data.c) << std::endl;
	}
};

#endif

