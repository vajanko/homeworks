// DU1simd.hpp
// Ondrej Kov·Ë NPRG051 2013/2014
#include <iterator>
#include <memory>
#include <assert.h>

template<typename Item, typename ItemIter>
class item_iterator
	: public std::iterator<std::random_access_iterator_tag, Item>
{
public:
	typedef const pointer const_pointer;
	typedef const reference const_reference;

protected:
	// data
	pointer data;			// pointer to the beginning of allocated data
	std::size_t data_size;	// size of the data array
	std::size_t index;		// position of iterator item from the beginning

	// check whether given iterator is compatible with the this one
	void check_comp(const ItemIter& it)
	{   // iterator is not empty and points to the same data
		assert(data != nullptr && data == it.data);
		//if (data == nullptr || data != it.data)
			//_DEBUG_ERROR("incompatible iterators");
	}
public:
	// operators
	ItemIter &operator=(const ItemIter &it)
	{
		data = it.data;
		data_size = it.data_size;
		index = it.index;
		return *this;
	}
	reference operator*() { return data[index]; }
	const_reference operator*() const { return data[index]; }
	reference operator[](const difference_type n) { return data[index + n]; }
	const_reference operator[](const difference_type n) const { return data[index + n]; }
	pointer operator->() const { return data + index; }

	ItemIter operator+(const difference_type n) { return ItemIter(data, data_size, index + n); }
	ItemIter operator-(const difference_type n) { return ItemIter(data, data_size, index - n); }
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
		return ItemIter(it.data, it.data_size, n + it.index);
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
	item_iterator() : data(nullptr), data_size(), index() {}
	item_iterator(const ItemIter &it) : data(it.data), data_size(it.data_size), index(it.index) {}
	item_iterator(pointer data, std::size_t data_size, std::size_t index) :
		data(data), data_size(data_size), index(index) {}
	virtual ~item_iterator() {}
};

template<typename S>
class simd_iterator :
	public item_iterator<S, simd_iterator<S>>
{
public:
	simd_iterator() : item_iterator() {}
	simd_iterator(const simd_iterator &it) : item_iterator(it) {}
	simd_iterator(pointer data, std::size_t data_size, std::size_t index) : item_iterator(data, data_size, index) {}
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
	simd_iterator lower_block() 
	{
		return simd_iterator((simd_iterator::pointer)data, data_size / K, index / K);
	}
	simd_iterator upper_block()
	{   // +1 only if this is not the end() of simd_vector
		std::size_t i = index / K;
		if (index < data_size)
			++i;

		return simd_iterator((simd_iterator::pointer)data, data_size / K, i);
	}
	difference_type lower_offset() { return index % K; }
	difference_type upper_offset() 
	{	// return 0 if this is the end - this is a special case
		return index < data_size ? index % K + 1 - K : 0;
	}

public:
	simd_vector_iterator() : item_iterator() {}
	simd_vector_iterator(const simd_vector_iterator &it) : item_iterator(it) {}
	simd_vector_iterator(pointer data, std::size_t data_size, std::size_t index) : item_iterator(data, data_size, index) {}
	virtual ~simd_vector_iterator() {}
};

template< typename T, typename S>
simd_vector_iterator< T, S> operator+( simd_vector_iterator< T, S> a, std::ptrdiff_t b)
{
	return a += b;
}
template< typename T, typename S>
simd_vector_iterator< T, S> operator-(simd_vector_iterator< T, S> a, std::ptrdiff_t b)
{
	return a -= b;
}

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
	iterator begin() { return iterator(data.get(), data_size, 0); }
	iterator end() { return iterator(data.get(), data_size, data_size); }
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
		simd_vector(s, aligned_mem(s)) {}
	simd_vector(const simd_vector &vec) = delete;
	simd_vector(simd_vector &&vec) :
		data(std::move(vec.data)), data_size(std::move(vec.data_size))
	{
		vec.data = nullptr;
		vec.data_size = 0;
	}
	~simd_vector() {}

private:
	// constructor pattern for non-trivial calculation:
	// allocates array of T aligned at S size
	struct aligned_mem 
	{
		T *memory;		// pointer to allocated aligned memory
		aligned_mem(std::size_t size)
		{
			const std::size_t ALIGNMENT = sizeof(S);

			// we allocate array of T of size "size" + something to be able to 
			// move this array in such a way that is will be aligned at sizeof(S)
			T* mem = new T[size + ALIGNMENT / sizeof(T)];

			// minimum required size of "mem" in bytes (without alignment)
			std::size_t required_size = size * sizeof(T);
			// size of allocated "mem" array in bytes
			std::size_t real_size = (size + 1) * sizeof(T);

			void *ptr = (void*)mem;
			// we want to setup "ptr" in such a wat that it will be aligned at "ALIGNMENT",
			// it will have "required_size" and we shall not use more than "real_size"
			T* aligned_mem = (T*)std::align(ALIGNMENT, required_size, ptr, real_size);

			if (aligned_mem == nullptr)
				throw std::bad_alloc();		// align wasn't successful

			memory = aligned_mem;		// everything went OK
		}
	};
	simd_vector(std::size_t s, aligned_mem m) :
		data(m.memory), data_size(s) {}
		
	
};