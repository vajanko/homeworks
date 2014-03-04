// DU1simd.hpp
// Ondrej Kov·Ë NPRG051 2013/2014
#include <iterator>
#include <vector>
#include <algorithm>

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
	reference operator*() { return *iterator;  }
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

template< typename T, typename S>
class simd_vector_iterator {
public:
	typedef std::vector<T> data_type;

	typedef std::random_access_iterator_tag iterator_category;
	typedef std::ptrdiff_t difference_type;
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type& reference;

	static const std::size_t K = sizeof(S) / sizeof(T);
	typedef step_iterator<typename data_type::iterator, K> simd_iterator;
private:
	pointer current;


public:
	/*simd_iterator lower_block() { return simd_iterator(); }
	simd_iterator upper_block();
	simd_iterator lower_offset();
	simd_iterator upper_offset();*/
	/*...*/
	
	simd_vector_iterator() { }
};

template< typename T, typename S>
simd_vector_iterator< T, S> operator+( simd_vector_iterator< T, S> a, std::ptrdiff_t b)
{
	return a += b;
}

/*...*/

template< typename T, typename S>
class simd_vector {
private:
	typedef std::vector<T> data_type;
	data_type data_;

public:
	typedef simd_vector_iterator< T, S> iterator;

	typedef typename simd_vector_iterator< T, S>::simd_iterator simd_iterator;

public:
	explicit simd_vector( std::size_t s) :
		data_(s)
	{/*...*/}

	iterator begin()
	{
		//return std::begin(data_);
		/*...*/
	}

	iterator end()
	{
		/*...*/
	}

	std::size_t size() { return data_.size(); }

	/*...*/
};
