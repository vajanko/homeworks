// DU1simd.hpp
// Ondrej Kov·Ë NPRG051 2013/2014
#include <iterator>
#include <memory>
#include <assert.h>

// Compiled in VS 2013 and GCC 4.8 

// Iterator base class
// Item: type of containter item
// ItemIter: real type of the iterator. Should be a derived class from this iterator
template<typename Item, typename ItemIter>
class item_iterator
	: public std::iterator<std::random_access_iterator_tag, Item>
{
private:
	typedef std::iterator<std::random_access_iterator_tag, Item> base_type;
public:
	// Aaa ... grrrr ... gcc
	typedef typename base_type::value_type value_type;
	typedef typename base_type::pointer pointer;
	typedef typename base_type::reference reference;
	typedef typename base_type::difference_type difference_type;
	typedef typename base_type::iterator_category iterator_category;

	typedef const pointer const_pointer;
	typedef const reference const_reference;

protected:
	// pointer to the beginning of allocated data
	pointer data;
	// size of the data array
	std::size_t data_size;
	// position of iterator item from the beginning
	std::size_t index;

	// check whether given iterator is compatible with the this one
	void check_comp(const ItemIter& it)
	{   // iterator is not empty and points to the same data
#ifdef __GNUC__
		assert(data != nullptr && data == it.data);
#else
		if (data == nullptr || data != it.data)
			_DEBUG_ERROR("incompatible iterators");
#endif
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
	/*ItemIter operator+(const ItemIter &it)
	{
		check_comp(it);
		return ItemIter(data, index + it.index);
	}
	ItemIter operator-(const ItemIter &it)
	{
		check_comp(it);
		return ItemIter(data, index - it.index);
	}*/

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
	/*friend dirrerence_type operator-(const ItemIter &a)
	{
		return a.index - b.index;
	}*/

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
	virtual ~item_iterator() { }
};

// Simple specialization of item_iterator. No special behaviour is added. Used
// as simd_iterator for simd_vector<T, S>
template<typename S>
class simd_iterator :
	// notice that operators and methods defined above actually return "simd_iterator<S>" not "item_iterator<S>"
	public item_iterator<S, simd_iterator<S>>
{
private:
	typedef item_iterator<S, simd_iterator<S>> base_type;
public:
	typedef typename base_type::pointer pointer;

	// Add methods specially for this type of iterator if necessary here
	// ...

// Copy-paste of constructors defined in the base class above
public:
	simd_iterator() : base_type() {}
	simd_iterator(const simd_iterator &it) : base_type(it) {}
	simd_iterator(pointer data, std::size_t data_size, std::size_t index) : base_type(data, data_size, index) {}
	virtual ~simd_iterator() {}
};

template< typename T, typename S>
class simd_vector_iterator :
	public item_iterator<T, simd_vector_iterator<T, S>>
{
private:
	typedef item_iterator<T, simd_vector_iterator<T, S>> base_type;
public:
	typedef typename base_type::pointer pointer;
	typedef typename base_type::difference_type difference_type;

	typedef simd_iterator<S> iterator;
private:
	static const std::size_t K = sizeof(S) / sizeof(T);

// Methods specially for this type of iterator
// Notice that we are using this->data_member coding style. It is because of the gcc
// compiler tries to lookup up the member too early (if it is not dependant on the
// template argument - we made it dependant)
public:
	// Gets simd_iterator at the first block where current iterator belongs to
	iterator lower_block()
	{
		return iterator((typename iterator::pointer)this->data, this->data_size / K, this->index / K);
	}
	// Gets simd_iterator at the following block after that into which current iterator belongs to.
	// Of returns simd end if current iterator is end
	iterator upper_block()
	{   // +1 only if this is not the end() of simd_vector
		std::size_t i = this->index / K;
		if (this->index < this->data_size)
			++i;

		return iterator((typename iterator::pointer)this->data, this->data_size / K, i);
	}
	// Returns number of items between current simd block and current iterator
	difference_type lower_offset() { return this->index % K; }
	// Returns the difference between current iterator position and the end of current simd
	// block or return zero if current iterator is end. Notice that this number is lower or
	// equal to zero
	difference_type upper_offset() 
	{	// return 0 if this is the end - this is a special case
		return this->index < this->data_size ? this->index % K + 1 - K : 0;
	}

// Copy-paste of constructors defined in the base class above
public:
	simd_vector_iterator() : base_type() {}
	simd_vector_iterator(const simd_vector_iterator &it) : base_type(it) {}
	simd_vector_iterator(pointer data, std::size_t data_size, std::size_t index) : base_type(data, data_size, index) {}
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
	typedef typename iterator::iterator simd_iterator;
private:
	std::unique_ptr<T[]> data;	// let the array to free automatically
	std::size_t data_size;		// size of the "data" array

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

public:
	// Creates a new simd_vector of given fixed size
	explicit simd_vector(std::size_t s) :
		simd_vector(aligned_mem(s)) {}
	// Creates a new simd_vector of given fixed size initialized with "init" value
	simd_vector(std::size_t s, T init) :
		simd_vector(aligned_mem(s, init)) {}
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
		T *memory;			// pointer to allocated aligned memory
		std::size_t size;	// size of "memory"

		// Allocates aligned memory and stores it to "memory"
		aligned_mem(std::size_t s) :
			size(s)
		{
			const std::size_t ALIGNMENT = sizeof(S);

			// we allocate array of T of size "size" + something to be able to 
			// move this array in such a way that is will be aligned at sizeof(S)
			T* mem = new T[s + ALIGNMENT / sizeof(T)];

			// minimum required size of "mem" in bytes (without alignment)
			std::size_t required_size = s * sizeof(T);
			// size of allocated "mem" array in bytes
			std::size_t real_size = (s + 1) * sizeof(T);

			void *ptr = (void*)mem;
			// we want to setup "ptr" in such a wat that it will be aligned at "ALIGNMENT",
			// it will have "required_size" and we shall not use more than "real_size"
#ifdef __GNUC__
			// no support for aligned memory in GCC compiler (actually it is possible to implement yourself)
			T* aligned_mem = mem;
#else
			T* aligned_mem = (T*)std::align(ALIGNMENT, required_size, ptr, real_size);
#endif

			if (aligned_mem == nullptr)
				throw std::bad_alloc();		// align wasn't successful

			memory = aligned_mem;		// everything went OK
		}

		// Allocates aligned memory, stores it to "memory" and initialize with "init" value
		aligned_mem(std::size_t s, T init) :
			aligned_mem(s)
		{
			// std::fill_n(memory, s, init);		// some WARNNINGS must be disabled

			for (std::size_t i = 0; i < s; ++i)
				memory[i] = init;			// copy constructor needed here, this could be a problem
		}
	};
	// Serve for other constructors as initialization of aligned memory
	explicit simd_vector(aligned_mem m) :
		data(m.memory), data_size(m.size) {}
		
	
};