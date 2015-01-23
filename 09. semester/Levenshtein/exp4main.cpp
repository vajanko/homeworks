#include "task4.hpp"
#include "du4levenstein.hpp"

#include "testbed.hpp"

#include <iostream>
#include <fstream>

///////////////////////////////

///////////////////////////////
void test()
{
	const char *str2 = "kitten";
	const char *str1 = "sitting";

	/*levenstein lev(str1, str1 + strlen(str1), str2, str2 + strlen(str2));
	int res = lev.compute();
	std::cout << res << std::endl;*/

	const int count = 128;

	data_element *s1 = new data_element[count];
	data_element *s2 = new data_element[count];
	for (int i = 0; i < count; ++i)
	{
		s1[i] = i;
		s2[i] = count + i;
	}
	s1[0] = 0;
	s1[2] = 0;
	s2[1] = 0;

	levenstein lev(s1, s1 + count, s2, s2 + count);
	int res = lev.compute();
	std::cout << res << std::endl;
}
///////////////////////////////

int main( int argc, char * * argv)
{
	 /*test();
	 return 0;*/

	generator_list< param_type_4, time_complexity> gl;

#ifdef _DEBUG
	std::size_t min_inner = 64UL;	
	//std::size_t min_inner = 128UL;
	std::size_t step_inner = 4;	
	std::size_t max_inner = 2UL * 1024UL;
	std::size_t min_outer = 64UL;	
	//std::size_t min_outer = 128UL;
	std::size_t step_outer = 4;	
	std::size_t max_outer = 2UL * 1024UL;
	time_complexity target_complexity = 250000UL;
#else
	std::size_t min_inner = 64UL;	
	std::size_t step_inner = 8;	
	std::size_t max_inner = 32UL * 1024UL;
	std::size_t min_outer = 64UL;	
	std::size_t step_outer = 8;	
	std::size_t max_outer = 32UL * 1024UL;
	time_complexity target_complexity = 1000000000UL;
#endif

	gl.push_back( make_generic_generator_task< generator_4< data_4< levenstein>, policy_random>, task_4, param_type_4>());

	for ( std::size_t outer = min_outer; outer <= max_outer; outer *= step_outer)
	{
		for ( std::size_t inner = min_inner; inner <= max_inner; inner *= step_inner)
		{
			gl.push_back_size( param_type_4( outer, inner), target_complexity);
		}
	}
/*
#ifdef _DEBUG
	std::ofstream oss( "exp4.debug.tsv");
#else
	std::ofstream oss( "exp4.tsv");
#endif
*/
	logger log( std::cout, std::cerr);

#ifdef _DEBUG
	gl.run< true>( log);
#else
	gl.run< false>( log);
#endif
	return 0;
}

///////////////////////////////
