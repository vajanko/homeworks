// Ondrej Kov·Ë NPRG051 2013/2014
#include <iterator>

// Iterator wrapper which allows to jump several positions in one step
template<typename Iterator, std::size_t Step>
class step_iterator
{
public:
	// types
	typedef typename Iterator::iterator_category iterator_category;
	typedef typename Iterator::difference_type difference_type;
	// TODO: this is wrong - value is Step sized "array"
	typedef typename Iterator::value_type value_type;
	typedef typename value_type* pointer;
	typedef typename value_type& reference;
private:
	// data
	Iterator iterator;
public:
	// operators
	step_iterator &operator=(const step_iterator &it)
	{
		iterator = it.iterator;
		return *this;
	}
	reference operator*() { return *iterator; }
	reference operator[](const difference_type n) { return iterator[n * Step]; }
	reference operator->() { return *iterator; }

	step_iterator operator+(const difference_type n) { return step_iterator(iterator + n); }
	step_iterator operator-(const difference_type n) { return step_iterator(iterator - n); }
	step_iterator operator+(const step_iterator &it) { return step_iterator(iterator + it.iterator); }
	step_iterator operator-(const step_iterator &it) { return step_iterator(iterator - it.iterator); }

	step_iterator &operator++() { std::advance(iterator, Step); return *this; }
	step_iterator &operator--() { std::advance(iterator, -Step); return *this; }
	step_iterator &operator+=(const difference_type n) { iterator += (n * Step); return *this; }
	step_iterator &operator-=(const difference_type n) { iterator -= (n * Step); return *this; }
	step_iterator operator++(int)
	{
		step_iterator it(*this);
		std::advance(iterator, Step);
		return it;
	}
	step_iterator operator--(int)
	{
		step_iterator it(*this);
		std::advance(iterator, -Step);
		return it;
	}
	friend step_iterator operator+(const difference_type n, const step_iterator &it) { return step_iterator(n * Step + it.iterator); }

	bool operator==(const step_iterator& it) { return iterator == it.iterator; }
	bool operator!=(const step_iterator& it) { return iterator != it.iterator; }
	bool operator>=(const step_iterator& it) { return iterator >= it.iterator; }
	bool operator<=(const step_iterator& it) { return iterator <= it.iterator; }
	bool operator>(const step_iterator& it) { return iterator > it.iterator; }
	bool operator<(const step_iterator& it) { return iterator < it.iterator; }
public:
	step_iterator() : iterator() {}
	step_iterator(const step_iterator &it) : iterator(it.iterator) {}
	explicit step_iterator(Iterator it) : iterator(it) {}
	~step_iterator() {}
};
