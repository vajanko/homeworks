#include "task1.hpp"
#include "du1container.hpp"

#include "testbed.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cassert>

///////////////////////////////

using namespace std;

class orange
{
public:
	int size;
	double weight;
	orange(int size, double weight) :
		size(size), weight(weight) { }
};
class apple
{
public:
	char type;
	byte color;
	double weight;
	apple(char type, byte color, double weight) :
		type(type), color(color), weight(weight) { }
};

// user-defined functor template
template< typename T> class partial_fruit;

// user-defined functor specializations
template<> class partial_fruit< orange> {
public:
	void do_something(orange & v)
	{
		cout << "orange (size: " << v.size << ", weight: " << v.weight << ")" << endl;
	}
	template< typename IT>
	void do_something_with_vector(IT b, IT e)
	{
		for (; b != e; ++b) { do_something(*b); }
	}
};

template<> class partial_fruit< apple> {
public:
	void do_something(apple & v)
	{
		cout << "apple (type: " << v.type << ", weight: " << v.weight << ", color: " << v.weight << ")" << endl;
	}
	template< typename IT>
	void do_something_with_vector(IT b, IT e)
	{
		for (; b != e; ++b) { do_something(*b); }
	}
};

// user-defined statically polymorphic functor
class fruit : public partial_fruit< orange>, public partial_fruit< apple>
{
public:
	template< typename T>
	void call(T & v)
	{
		static_cast< partial_fruit< T>*>(this)->do_something(v);
	}

	template< typename T, typename IT>
	void vector_call(IT b, IT e)
	{
		static_cast< partial_fruit< T>>(this)->do_something_with_vector(b, e);
	}
};

void test()
{
	du1container c;
	du1container::magic<orange> orange_magic = c.register_type<orange>();
	du1container::magic<apple> apple_magic = c.register_type<apple>();

	// push data
	orange_magic.push_back(orange(3, 1.5));
	orange_magic.push_back(orange(3, 1.7));
	apple_magic.push_back(apple('a', 3, 1.2));
	orange_magic.push_back(orange(3, 1.5));
	apple_magic.push_back(apple('c', 6, 1.9));
	
	//
	du1container::dynamic_polyfunctor<fruit> dpf(c);
	dpf << orange_magic << apple_magic;

	cout << "unordered" << endl;
	c.unordered_for_each(dpf(fruit()));
	cout << "ordered" << endl;
	c.ordered_for_each(dpf(fruit()));
}

int main( int argc, char * * argv)
{
	generator_list< std::size_t, time_complexity> gl;

#ifdef _DEBUG
	time_complexity target_complexity = 500000ul;
	std::size_t min_elements = 64 * 1024;
	std::size_t max_elements = min_elements;
#else
	time_complexity target_complexity = 5000000000ul;
	std::size_t min_elements = 1024l;
	std::size_t max_elements = 64 * 1024ul * 1024ul;
#endif

	gl.push_back( make_generic_generator_task< generator_1< du1container, policy_random>, task_1, std::size_t>());
	//gl.push_back(make_generic_generator_task< generator_1< du1container, policy_grouped>, task_1o, std::size_t>());
	//gl.push_back( make_generic_generator_task< generator_1< du1container, policy_random>, task_1o, std::size_t>());
	//gl.push_back(make_generic_generator_task< generator_1< du1container, policy_robin>, task_1o, std::size_t>());

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

	//test();
	//system("pause");

	return 0;
}

///////////////////////////////
