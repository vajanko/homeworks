#include<iostream>
#include<string>
// SEQ
#include<array>
#include<vector>
#include<deque>
#include<list>
#include<forward_list>

// ASOC
#include<set>
#include<map>
#include<unordered_set>
#include<unordered_map>

using namespace std;

class A
{
private:
	int value;
public:
	A(int x):value(x) { }
};

typedef std::map<int, std::string> dictionary;

int main(int argc, char **argv)
{
	// SEQential containers
	// array - use instead of standard array - fixed size
	array<int, 3> arr;
	vector<int> vec(3);
	deque<int> dec(3);
	// doubly linked list
	list<int> lst;
	// linked list
	forward_list<int> f_lst;
	// initialize a vector of size 4 initialized with A(3) value
	vector<A> vecA(4, A(3));

	// ASOCiative containers
	set<int> set1;
	// allows multiplicity of the same items
	multiset<int> set2;
	map<int, string> map1;
	// allows multiplicity of key values
	multimap<int, int> map2;
	// hash function
	unordered_set<int> set3;
	unordered_multiset<int> set4;
	unordered_map<int, int> map3;
	unordered_multimap<int, int> map4;

	// methods
	vector<int>::size_type size = arr.size();
	arr.empty();
	array<int,3>::iterator iter = arr.begin();
	// constant iterator - does not allow to change target value - more efficient
	array<int,3>::const_iterator citer = arr.cbegin();
	// begin() also returns const_iterator if arr is const already
	// does not exist on array
	vec.clear();

	// VECTOR - random iterator
	vector<int>::size_type;		// type of container size
	vector<int>::value_type;	// type of container item
	vector<int>::reference;		// int&
	vector<int>::const_reference;// const int&
	vector<int>::iterator;
	vector<int>::const_iterator;
	vector<int>::reverse_iterator;
	vector<int>::const_reverse_iterator;
	// useful to use auto type

	// at check the array range, [] does not
	arr.at(1) == arr[1];
	auto riter = arr.rbegin();		// reverse iterators
	auto criter = arr.crbegin();

	// vector is empty after initialization !!!
	vec.push_back(123);		// insert at the end
	vec.pop_back();			// remove from the end - does not return anything
	// vec.insert(...);
	// vec.erase(...);
	vec.resize(20, 3);		// resize to size 20 and initialize with value 3

	// DEQUE - random iterator (index of items can change at runtime)
	dec.push_back(4);
	dec.push_front(5);
	dec.pop_back();
	dec.pop_front();

	// LIST - bidirectional iterator
	lst.insert(lst.begin(), 3);
	lst.erase(lst.begin(), lst.end());

	// SET
	set<int>::iterator it1 = set1.find(3);
	
	// for multiversions
	// 1,2,3,3,4,4,5,5,5
	multiset<int>::iterator it2 = set2.lower_bound(3);	// returns iterator pointing to the first 3 key
	multiset<int>::iterator it3 = set2.upper_bound(3);	// returns interator pointing after the last 3 key

	// MAP
	map1.insert(make_pair(3, "hello"));
	// map1.insert(make_pair<int, string>(3, "hello"));

	// set<T, COMP>	// COMP is a type that can compare T type, it is enough to implement < operator
	// map<T, X>	// T type must be hashable, or needs to implement function that returns int value
	// unordered_map	// does not restrict anything

	// example
	auto ub = map2.upper_bound(3);
	for (auto lb = map2.lower_bound(3); lb != ub; ++lb)
	{
		// first = key, second = value
		cout << lb->first << " " << lb->second << endl;
		// *lb - get value of the iterator - this is the pair
		// (*lb).first is equivalent to lb->first
	}

	system("pause");
	return 0;
}




// Containers
// seq, asoc, unorder

// Iterators
// forward ++
// bidirectional ++, --
// random ++,--,[]