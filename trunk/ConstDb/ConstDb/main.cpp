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
	table<int> tab{ 1, 2 };
	int x = 100;

	int val = 10;
	int *ptr = &val;
	int &ref = *ptr;

	ptr = &val;
	//ref = *ptr;
	ref = 30;


	system("pause");
	return 0;
}