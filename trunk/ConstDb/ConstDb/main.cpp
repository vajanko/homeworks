#include<iostream>
#include<map>
#include "du2.hpp"

using namespace std;

template <typename T> struct A 
{ 
	static int n;
	static int f() { static int n = 0; return ++n; } 
}; 
template <typename T> int A<T>::n;

int main(int argc, char **argv)
{
	typedef std::tuple<int, bool, double> value_type;
	std::vector<value_type> data;
	data.push_back(make_tuple(1, true, 1.0));
	data.push_back(make_tuple(2, false, 2.0));
	data.push_back(make_tuple(3, true, 3.0));

	typedef index<0, int, value_type> index_type;
	index<0, int, value_type> ind(data);
	index_holder<0, index_type> holder(data);

	auto x = holder->get(1);

	system("pause");
	return 0;
}