
// iterator na stlpce, riadky -> stlpec je zase iterator cez riadky a naopak
// nacitat a vypisovat do streamu

#include<iostream>
#include<string>
#include<sstream>
#include "matrix2.hpp"

using namespace std;

void parse_test()
{
	string str = "	{ 4, 5 "
		"	{ 2 , 4, 25  ,		96, 3}{ 1,5, 6, 9, 89 }"
		"{ 4, 39, 20, 10, 10"
		"} { 3, 10, 1, 30, 02  } "
		"}";
	istringstream stream(str);

	matrix2<int> m;
	//bool success = m.parse(stream);
	stream >> m;

	cout << m;

	m += m;

	cout << m;
	//m.print(cout);
}
void operator_test()
{
	matrix2<double> m1(2,3);
	matrix2<double> m2(2,3);
	matrix2<int> m3(2,3);

	cout << (m1 == m2) << endl;
	m1.at(0,0) = 100;
	cout << (m1 == m2) << endl;
	//cout << (m1 == m3) << endl;

	auto m4 = m1 + m2;
	cout << "m4: " << endl;
	cout << m4;

	matrix2<double> m6(3,2);
	auto m5 = m6 * m2;
	cout << "m5: " << endl;
	cout << m5;

	vector<int> vec(3);
}
void iterator_test()
{
	cout << "iterator test" << endl;
	matrix2<int> m1(5,5);

	auto it1 = m1.row_begin();
	auto it2 = it1[1];

	for (auto it = m1.row_begin(); it < m1.row_end(); ++it)
	{
		for (int i = 0; i < it.size(); ++i)
		{
			cout << it[i] << " ";
		}
		cout << endl;
	}

	vector<int> v(3);
	auto x = v.begin();
	//auto y = x[3];
}

int main(int argc, char **argv)
{
	parse_test();
	operator_test();
	iterator_test();

	matrix2<int> m(5,7);

	matrix2<int> m2(7,4);
	//m2 = m;
	//m += m2;
	//for (int i = 0; i < 3; i++)
	//	m.at(i, i) = 0;
	m.print(cout);
	m2.print(cout);

	m *= m2;
	m.print(cout);
	int a = m.at(1,2);
	//m.at(0,0) = 100;

//	int i = m.at(0,0);

	//cout << m;
	//m.print(cout);

	//matrix2<int> m2(m);
	
	auto rb = m.row_begin();
	auto re = m.row_end();
	for (matrix2<int>::row_iterator ri = rb; ri < re; ++ri)
	{
		auto cb = ri.begin();
		auto ce = ri.end();
		for (auto ci = cb; ci < ce; ++ci)
		{
			int x = *ci;
			cout << x << " ";
		}
		cout << endl;
	}
	cout << endl;

	auto cb = m.column_begin();
	auto ce = m.column_end();
	for (matrix2<int>::column_iterator ci = cb; ci < ce; ++ci)
	{
		auto rb = ci.begin();
		auto re = ci.end();
		for (auto ri = rb; ri < re; ++ri)
		{
			int x = *ri;
			cout << x << " ";
		}
		cout << endl;
	}

	system("pause");
	return 0;
};