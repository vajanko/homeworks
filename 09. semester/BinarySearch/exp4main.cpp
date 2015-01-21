#include "task4.hpp"
#include "du3bsearch.hpp"

#include "testbed.hpp"

#include <iostream>
#include <fstream>

///////////////////////////////
///////////////////////////////
///////////////////////////////

void test()
{
	const std::size_t in_size = 6;

	data_element in_data[in_size] = { 1, 2, 3, 6, 7, 8 };// 16 , 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };

	bsearch_inner inner(in_data, in_size);

	const std::size_t out_size = 29;
	bsearch_outer outer(inner, out_size);
	data_element out_data[out_size] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28 };

	outer.bucketize(out_data);
	for (std::size_t i = 0; i < in_size + 1; ++i)
	{
		if (i > 0)
			std::cout << in_data[i - 1] << " -> ";
		else
			std::cout << ". -> ";

		auto b = outer.bucket(i);
		for (std::size_t j = 0; j < b.second; ++j)
			std::cout << b.first[j] << " ";
		std::cout << std::endl;
	}
}

int main( int argc, char * * argv)
{
	/*test();
	system("pause");
	return 0;*/

	generator_list< param_type_4, time_complexity> gl;

#ifdef _DEBUG
	std::size_t min_inner = 64UL;	
	//std::size_t min_inner = 1024 * 4UL;
	std::size_t step_inner = 4;	
	std::size_t max_inner = 4 * 1024UL * 1024UL;
	//std::size_t max_inner = 4 * 1024UL;
	std::size_t min_outer = 1024UL;	
	std::size_t step_outer = 16;	
	//std::size_t max_outer = 4 * 1024UL * 1024UL;
	std::size_t max_outer = 4 * 1024UL * 16;
	time_complexity target_complexity = 2500000UL;
#else
	std::size_t min_inner = 64UL;
	//std::size_t min_inner = 4 * 1024UL;
	//std::size_t min_inner = 4UL;
	std::size_t step_inner = 4;
	//std::size_t step_inner = 2;
	std::size_t max_inner = 4 * 1024UL * 1024UL;
	//std::size_t max_inner = 256;
	std::size_t min_outer = 1024UL;	
	std::size_t step_outer = 16;	
	std::size_t max_outer = 4 * 1024UL * 1024UL;
	//std::size_t max_outer = 4 * 1024UL;
	time_complexity target_complexity = 100000000UL;
#endif

//	gl.push_back( make_generic_generator_task< generator_4< data_4< bsearch_std_inner, bsearch_std_outer< bsearch_std_inner> >, policy_random>, task_4, param_type_4>());
//	gl.push_back( make_generic_generator_task< generator_4< data_4< bsearch_handmade_inner, bsearch_std_outer< bsearch_handmade_inner> >, policy_random>, task_4, param_type_4>());
//	gl.push_back( make_generic_generator_task< generator_4< data_4< bsearch_tree_inner, bsearch_std_outer< bsearch_tree_inner> >, policy_random>, task_4, param_type_4>());
//	gl.push_back( make_generic_generator_task< generator_4< data_4< bsearch_tree_inner, bsearch_parallel_outer< bsearch_tree_inner> >, policy_random>, task_4, param_type_4>());
	gl.push_back( make_generic_generator_task< generator_4< data_4< bsearch_inner, bsearch_outer>, policy_random>, task_4, param_type_4>());

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
