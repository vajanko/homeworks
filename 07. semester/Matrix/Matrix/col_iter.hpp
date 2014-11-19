#pragma once

#include <iterator>

/*template<typename T> 
class col_iter
{
public:
	typedef std::random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef const reference const_reference;

public:
	reference operator*() { }
	const_reference operator*() const { }
	col_iter<value_type>& operator=(const col_iter<value_type> &it) { }
	bool operator==(const col_iter<value_type> &it) { }
	// ... 

private:
	size_t column;
	std::iterator<std::input_iterator_tag, T> rows_iter;

public:
	col_iter<T>& operator++() { ++rows_iter; return *this; }
	col_iter<T> operator++(int) { col_iter<T> tmp(*this); operator++(); return tmp; }
	bool operator==(const col_iter<T> &rhs) { return p == rhs.p; }
	bool operator!=(const col_iter<T> &rhs) { return p != rhs.p; }
	reference operator*() { return rows_iter[column]; }

	col_iter(std::iterator<std::input_iterator_tag, T> rows_iter, size_t column) : rows_iter(rows_iter), column(column) {}
	col_iter(const col_iter<T> &it) : rows_iter(it.rows_iter), column(it.column) { }
	~col_iter(void) { }
};*/

