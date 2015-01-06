#ifndef _TASK4_HPP
#define _TASK4_HPP

#include <cstddef>
#include <memory>
#include <vector>
#include <cassert>

#include "testbed.hpp"

#include <emmintrin.h>

///////////////////////////////

typedef unsigned long data_element;

template< typename P>
struct packed_generator : public P {
	using P::reset;
	packed_generator()
	{
		reset();
	}
};

struct core_generator {

	template< typename P>
	core_generator( std::size_t opsize, P & gen)
		: data_( opsize)
	{
		for ( std::size_t j = 0; j < opsize; ++ j)
		{
			data_[ j] = gen.get();
		}
	}

	std::vector< data_element> data_;
};

struct core_generator_sorted : public core_generator {

	template< typename P>
	core_generator_sorted( std::size_t opsize, P & gen)
		: core_generator( opsize, gen)
	{
		std::sort( data_.begin(), data_.end());
	}
};

std::size_t log2( std::size_t s)
{
	std::size_t i = 0;
	for (; s; s >>= 1)
		++ i;
	return i;
}

typedef std::pair< std::size_t, std::size_t> param_type_4;	// outer and inner size

std::ostream & operator<<( std::ostream & os, const param_type_4 & v)
{
	return os << v.first << "*" << v.second;
}

template< typename D, typename P>
struct generator_4 {

	typedef D data_type;
	typedef std::pair< std::pair< const data_element *, std::size_t>, std::size_t> check_type;	// a kind of checksum
		
	static std::string name() { return D::name() + "_" + P::name(); }

	std::string param() const 
	{ 
		return ulibpp::lexical_cast< std::string>( opsize_);
	}

	time_complexity complexity() const 
	{ 
		return outer_data_.data_.size() * log2( inner_data_.data_.size()) + inner_data_.data_.size();
	}

	check_type check() const
	{
		return check_type( std::make_pair( inner_data_.data_.data(), inner_data_.data_.size()), outer_data_.data_.size());
	}

	const D & data() const
	{
		return d_;
	}

	template< typename GP>
	generator_4( const GP &, param_type_4 opsize) 
		: inner_data_( opsize.second, gen_),
		outer_data_( opsize.first, gen_),
		d_( inner_data_.data_.data(), inner_data_.data_.size(), outer_data_.data_.data(), outer_data_.data_.size())
	{
	}

private:
	packed_generator< P> gen_;
	core_generator_sorted inner_data_;
	core_generator outer_data_;
	D d_;
};

struct policy_random {

	static std::string name() { return "random"; }

	void reset()
	{
		srand( 729);
	}

	data_element get()
	{
		data_element lo = std::rand();
		data_element hi = std::rand();
		return (hi << 15) ^ lo;	
	}
};

template< typename NI, typename NO>
struct data_4 {
public:

	static std::string name() { return "du3"; }

	std::size_t byte_size() const
	{
		return inner.byte_size();
	}

	data_4( const data_element * idata, std::size_t isize, const data_element * odata, std::size_t osize)
		: inner( idata, isize), outer( inner, osize), outer_data( odata)
	{
	}

	NI inner;
	mutable NO outer;
	const data_element * outer_data;
};

struct task_4 {

	static std::string name() { return "bsearch"; }

	template< bool cold, bool debug, typename D, typename C>
	static void run( const D & data, const C & check)
	{
		data.outer.bucketize( data.outer_data);

		// assert( s == check);
	}


	template< bool debug, typename D, typename C>
	static void initial_check( logger & log, const D & data, const C & check)
	{
		/*
		log.ss() << "CHKSUM A[" << data.a.vsize() << "," << data.a.hsize() << "] = " << chksum( data.a) << std::endl;
		log.ss() << "CHKSUM B[" << data.b.vsize() << "," << data.b.hsize() << "] = " << chksum( data.b) << std::endl;
		*/
	}

	static void check_low( logger & log, const data_element * bd, std::size_t bs, data_element cmp, std::size_t k)
	{
		for ( std::size_t j = 0; j < bs; ++j)
		{
			if ( bd[ j] < cmp )
			{
				log.ss() << "Too low value at bucket[" << k << "][" << j << "]" << std::endl;
			}
		}
	}

	static void check_high( logger & log, const data_element * bd, std::size_t bs, data_element cmp, std::size_t k)
	{
		for ( std::size_t j = 0; j < bs; ++j)
		{
			if ( bd[ j] >= cmp )
			{
				log.ss() << "Too high value at bucket[" << k << "][" << j << "]" << std::endl;
			}
		}
	}

	template< bool debug, typename D, typename C>
	static void final_check( logger & log, const D & data, const C & check)
	{
		std::size_t total = 0;
		{
			// inspect first bucket
			std::pair< const data_element *, std::size_t> rv = data.outer.bucket( 0);
			check_high( log, rv.first, rv.second, check.first.first[ 0], 0);
			total += rv.second;
		}
		for ( std::size_t k = 1; k < check.first.second; ++ k)
		{
			// inspect k-th bucket
			std::pair< const data_element *, std::size_t> rv = data.outer.bucket( k);
			check_low( log, rv.first, rv.second, check.first.first[ k-1], k);
			check_high( log, rv.first, rv.second, check.first.first[ k], k);
			total += rv.second;
		}
		{
			// inspect last bucket
			std::pair< const data_element *, std::size_t> rv = data.outer.bucket( check.first.second);
			check_low( log, rv.first, rv.second, check.first.first[ check.first.second-1], check.first.second);
			total += rv.second;
		}
		if ( total != check.second )
		{
			log.ss() << "TOTAL SIZE OF BUCKETS " << total << " DOES NOT MATCH OUTER DATA SIZE " << check.second << std::endl;
		}
	}
};

#endif

