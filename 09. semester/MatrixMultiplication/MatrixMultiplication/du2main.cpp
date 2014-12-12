#include "task2.hpp"

#include "testbed.hpp"

#include "du2matrix.hpp"

#include <iostream>
#include <fstream>

using namespace std;

///////////////////////////////

///////////////////////////////

///////////////////////////////

///////////////////////////////

///////////////////////////////
const std::size_t size = 16;
void init_id(matrix &m)
{
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			m.set(i, j, i == j);
}
void init_trian(matrix &m)
{
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			m.set(i, j, i <= j);
}
void init_id2(matrix &m)
{
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			m.set(i, j, i != j);
}
void init_cols(matrix &m)
{
	bool val = true;
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			m.set(i, j, val);
			val = !val;
		}
	}
}
void write(matrix &m)
{
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			bool val = m.get(i, j);
			std::cout << val ? 1 : 0;
			std::cout << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
void test()
{
	matrix a(size, size);
	init_id(a);
	matrix b(size, size);
	init_trian(b);

	matrix c(size, size);
	c.assign_mul(b, a);
	
	write(a);
	write(b);
	write(c);
}

int main( int argc, char * * argv)
{
#ifdef _DEBUG
	test();
#endif

	generator_list< std::size_t, time_complexity> gl;

#ifdef _DEBUG
	std::size_t min_elements = 256UL;	// one SSE group
	std::size_t max_elements = 1024UL;//128UL;
	time_complexity target_complexity = 25000UL;
#else
	std::size_t min_elements = 128UL;	
	std::size_t max_elements = 2048UL;
	time_complexity target_complexity = 7000000000UL;
#endif

	gl.push_back( make_generic_generator_task< generator_3< data_3< matrix>, policy_zero>, task_3, std::size_t>());
	gl.push_back( make_generic_generator_task< generator_3< data_3< matrix>, policy_random>, task_3, std::size_t>());
	gl.push_back( make_generic_generator_task< generator_3< data_3< matrix>, policy_one>, task_3, std::size_t>());

	for ( std::size_t elements = min_elements; elements <= max_elements; elements <<= 1)
	{
		gl.push_back_size( elements, target_complexity);
	}

	logger log( std::cout, std::cerr);

#ifdef _DEBUG
	gl.run< true>( log);
#else
	gl.run< false>( log);
#endif
	
	
	//system("pause");
	//return 0;
}

///////////////////////////////
