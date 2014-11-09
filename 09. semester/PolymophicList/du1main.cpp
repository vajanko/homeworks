#include "task1.hpp"
#include "du1container.hpp"

#include "testbed.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cassert>

///////////////////////////////

int main( int argc, char * * argv)
{
	generator_list< std::size_t, time_complexity> gl;

#ifdef _DEBUG
	time_complexity target_complexity = 500000UL;
	std::size_t min_elements = 64 * 1024;
	std::size_t max_elements = min_elements;
#else
	time_complexity target_complexity = 5000000000UL;
	std::size_t min_elements = 1024L;
	std::size_t max_elements = 64 * 1024UL * 1024UL;
//	std::size_t min_elements = 64 * 1024;
//	std::size_t max_elements = min_elements;
#endif

	gl.push_back( make_generic_generator_task< generator_1< du1container, policy_random>, task_1, std::size_t>());
	gl.push_back( make_generic_generator_task< generator_1< du1container, policy_random>, task_1O, std::size_t>());

	for ( std::size_t elements = min_elements; elements <= max_elements; elements <<= 3)
	{
		gl.push_back_size( elements, target_complexity);
	}

	logger log( std::cout, std::cerr);

#ifdef _DEBUG
	gl.run< true>( log);
#else
	gl.run< false>( log);
#endif

	return 0;
}

///////////////////////////////
