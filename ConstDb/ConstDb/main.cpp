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
	data.push_back(make_tuple(1, true, 2.0));
	data.push_back(make_tuple(2, false, 1.0));
	data.push_back(make_tuple(3, true, 3.0));

	typedef index<2, value_type> index_type;
	index_type ind(data);
	index_holder<index_type> holder(data);

	auto x = holder->get(1);

	/*map<int, double> m;
	auto res = m.find(1);
	res->second*/
	
	index_tuple<2, value_type> v(data);
	//v.get<0>();

	table<int, bool, double> tab(data);
	auto &val = tab.find<0>(1);

	bool res;
	res = tab.contains<0>(123);
	res = tab.contains<0>(1);

	try {
		tab.find<0>(123);
	}
	catch (const not_found_exception &ex) {
		cout << ex.what() << endl;
	}

	system("pause");
	return 0;
}