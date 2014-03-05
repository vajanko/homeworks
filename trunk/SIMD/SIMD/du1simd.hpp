// DU1simd.hpp
// Ondrej Kov·Ë NPRG051 2013/2014
#include <iterator>
#include <memory>
#include <algorithm>

template<typename Item, typename ItemIter>
class item_iterator
{
public:
	// types
	typedef typename std::random_access_iterator_tag iterator_category;
	typedef typename std::ptrdiff_t difference_type;
	typedef typename Item value_type;
	typedef typename value_type* pointer;
	typedef typename value_type& reference;
protected:
	// data
	pointer data;
	std::size_t index;

	void check_comp(const ItemIter& it)
	{
		if (data == nullptr || data != it.data)
			_DEBUG_ERROR("incompatible iterators");
	}
public:
	// operators
	ItemIter &operator=(const ItemIter &it)
	{
		data = it.data;
		index = it.index;
		return *this;
	}
	reference operator*() { return data[index]; }
	reference operator[](const difference_type n) { return iterator[index + n]; }
	reference operator->() { return data[index]; }

	ItemIter operator+(const difference_type n) { return ItemIter(data, index + n); }
	ItemIter operator-(const difference_type n) { return ItemIter(data, index - n); }
	ItemIter operator+(const ItemIter &it)
	{
		check_comp(it);
		return ItemIter(data, index + it.index);
	}
	ItemIter operator-(const ItemIter &it)
	{
		check_comp(it);
		return ItemIter(data, index - it.index);
	}

	ItemIter &operator++() { ++index; return *static_cast<ItemIter*>(this); }
	ItemIter &operator--() { --index; return *static_cast<ItemIter*>(this); }
	ItemIter &operator+=(const difference_type n) { index += n; return *static_cast<ItemIter*>(this); }
	ItemIter &operator-=(const difference_type n) { index -= n; return *static_cast<ItemIter*>(this); }
	ItemIter operator++(int)
	{
		ItemIter it(*static_cast<ItemIter*>(this));
		++index;
		return it;
	}
	ItemIter operator--(int)
	{
		ItemIter it(*static_cast<ItemIter*>(this));
		--index;
		return it;
	}
	friend ItemIter operator+(const difference_type n, const ItemIter &it)
	{
		return ItemIter(it.data, n + it.index);
	}

	bool operator==(const ItemIter& it)
	{
		check_comp(it);
		return index == it.index;
	}
	bool operator!=(const ItemIter& it)
	{
		check_comp(it);
		return index != it.index;
	}
	bool operator>=(const ItemIter& it)
	{
		check_comp(it);
		return index >= it.index;
	}
	bool operator<=(const ItemIter& it)
	{
		check_comp(it);
		return index <= it.index;
	}
	bool operator>(const ItemIter& it)
	{
		check_comp(it);
		return index > it.index;
	}
	bool operator<(const ItemIter& it)
	{
		check_comp(it);
		return index < it.index;
	}
public:
	item_iterator() : data(nullptr), index(0) {}
	item_iterator(const ItemIter &it) : data(it.data), index(it.index) {}
	item_iterator(pointer data, std::size_t index) : data(data), index(index) {}
	virtual ~item_iterator() {}
};

template<typename S>
class simd_iterator :
	public item_iterator<S, simd_iterator<S>>
{
public:
	simd_iterator() : item_iterator() {}
	simd_iterator(const simd_iterator &it) : item_iterator(it) {}
	simd_iterator(pointer data, std::size_t index) : item_iterator(data, index) {}
	virtual ~simd_iterator() {}
};

template< typename T, typename S>
class simd_vector_iterator :
	public item_iterator<T, simd_vector_iterator<T, S>>
{
public:
	typedef simd_iterator<S> simd_iterator;
private:
	static const std::size_t K = sizeof(S) / sizeof(T);

public:
	// TODO: this is totally wrong
	simd_iterator lower_block() 
	{
		return simd_iterator((simd_iterator::pointer)data, index / K);
	}
	simd_iterator upper_block()
	{   // +1 only if this is not the end() of simd_vector
		return simd_iterator((simd_iterator::pointer)data, index / K + 1);
	}
	difference_type lower_offset() { return index % K; }
	difference_type upper_offset() { return index % K + 1 - K; }

public:
	simd_vector_iterator() : item_iterator() {}
	simd_vector_iterator(const simd_vector_iterator &it) : item_iterator(it) {}
	simd_vector_iterator(pointer data, std::size_t index) : item_iterator(data, index) {}
	virtual ~simd_vector_iterator() {}
};

template< typename T, typename S>
simd_vector_iterator< T, S> operator+( simd_vector_iterator< T, S> a, std::ptrdiff_t b)
{
	return a += b;
}

/*...*/

template< typename T, typename S>
class simd_vector {
public:
	// types
	typedef simd_vector_iterator< T, S> iterator;
	typedef typename iterator::simd_iterator simd_iterator;
private:
	// data
	std::unique_ptr<T[]> data;	// let the array to free automatically
	std::size_t data_size;

public:
	

	iterator begin() { return iterator(data.get(), 0); }
	iterator end() { return iterator(data.get(), data_size); }
	std::size_t size() { return data_size; }

	simd_vector &operator=(const simd_vector &vec) = delete;
	simd_vector &operator=(simd_vector &&vec)
	{
		if (this != &vec)
		{
			data = std::move(vec.data);
			data_size = std::move(vec.data_size);

			vec.data = nullptr;
			vec.data_size = 0;
		}
		return *this;
	}

	/*...*/
public:
	// TODO: the size of data array is not calculated correctly
	explicit simd_vector(std::size_t s) :
		data(new T[s]), data_size(s) {}
		//data((T *)new S[s / sizeof(T)]), data_size(s) {}
		//data(new T[s + sizeof(S) - s % sizeof(S)]), size(s) { }
	simd_vector(const simd_vector &vec) = delete;
	simd_vector(simd_vector &&vec) :
		data(std::move(vec.data)), data_size(std::move(vec.data_size))
	{
		vec.data = nullptr;
		vec.data_size = 0;
	}
		
	~simd_vector() {}
};