#pragma once

#include<vector>
#include<iostream>
#include<exception>
#include<iterator>
#include<algorithm>
#include<iomanip>

template<typename T>
class matrix2
{
// wrapper iterator which allows jumping over several items in one step
class step_iter
{
public:
	typedef typename matrix2<T>::matrix_type::iterator item_iterator;
	typedef typename matrix2<T>::size_type size_type;

	typedef typename item_iterator::iterator_category iterator_category;
	typedef typename matrix2<T>::matrix_type::difference_type difference_type;
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef const reference const_reference;

private:
	item_iterator iter;		// current iterator position (nested iterator)
	item_iterator last;		// last item in the nested iterator container
	difference_type step;	// number of items to jump in one iterator step

	// Advance given numver of steps without exceeding the last iterator (otherwise the nested iterator would
	// throw an exception.
	void safe_advance(item_iterator &it, difference_type n) const 
	{
		// TODO: check whether n is not too much
		if (iter != last)
		{
			difference_type remaining(std::distance(it, last));
			if (remaining < n)
			  n = remaining;		// iterator points before end() and the next step is behind end()
		}

		// if the iterator points to the last item, this method results in an error
		std::advance(it, n);
	}

public:
	reference operator*() { return *iter; }
	const_reference operator*() const { return *iter; }

	// forward
	step_iter& operator++() { safe_advance(iter, step); return *this; }
	step_iter operator++(int) { step_iter tmp(*this); operator++(); return tmp; }
	// bidirectional
	step_iter& operator--() { safe_advance(iter, -step); return *this; }
	step_iter operator--(int) { step_iter tmp(*this); operator--(); return tmp; }
	// random
	reference operator[](const difference_type& n) const { return iter[n * step]; }
	step_iter& operator+=(const difference_type& n) { safe_advance(iter, n * step); return *this; }
	step_iter& operator-=(const difference_type& n) { safe_advance(iter, -n * step); return *this; }

	bool operator==(const step_iter &it) { return (iter == it.iter); }
	bool operator!=(const step_iter &it) { return !(iter == it.iter); }
	bool operator<(const step_iter &it) { return (iter < it.iter); }
	bool operator>(const step_iter &it) { return (iter > it.iter); }
	bool operator>=(const step_iter &it) { return (iter >= it.iter); }
	bool operator<=(const step_iter &it) { return (iter <= it.iter); }

	step_iter(item_iterator iter, item_iterator last, difference_type step) : iter(iter), last(last), step(step) {}
	step_iter(const step_iter &it) : iter(it.iter), last(it.last), step(it.step) { }
	~step_iter(void) { }
};

// column or row iterator
// its value is an iterator for row/column items
class vector_iter
{
public:
	typedef typename matrix2<T>::iterator item_iterator;
	typedef typename matrix2<T>::matrix_type matrix_type;

	// container definition
	typedef typename matrix2<T>::size_type size_type;
	typedef typename matrix2<T>::matrix_type::difference_type difference_type;
	typedef typename item_iterator::iterator_category iterator_category;
	typedef T value_type;
	typedef value_type& reference;
	typedef const reference const_reference;
	typedef value_type* pointer;
	typedef const pointer const_pointer;
	typedef step_iter iterator;
	typedef const iterator const_iterator;

private:
	
	item_iterator current;			// points to current row/column (the beginning of a row or column)
	difference_type step;					// how many items do I need to jump to get to the next row/column (always 1 for column iterator)
	difference_type next_step;			// step for the nested iterator (always 1 for row iterator)
	matrix_type data;
	item_iterator last;
	size_type size_;

public:
	vector_iter &operator=(const vector_iter &it) { data = it.data; current = it.current; step = it.step; next_step = it.next_step; last = it.last; }

	size_type size() { return size_; }
	bool empty() { return size() == 0; }
	bool operator<(const vector_iter &it) { return current < it.current; }
	// TODO: this is not enough
	bool operator==(const vector_iter &it) { return current == it.current && step == it.step }

	// forward
	vector_iter& operator++() { std::advance(current, step); return *this; }
	vector_iter operator++(int) { vector_iter tmp(*this); operator++(); return tmp; }
	// bidirectional
	vector_iter& operator--() { std::advance(current, -step); return *this; }
	vector_iter operator--(int) { vector_iter tmp(*this); operator--(); return tmp; }
	// random
	reference operator[](const difference_type& n) const { return current[n * next_step]; }
	vector_iter& operator+=(const difference_type& n) { std::advance(current, n * step); return *this; }
	vector_iter& operator-=(const difference_type& n) { std::advance(current, -n * step); return *this; }

	//bool operator==(const vector_iter &it) { return (current == it.current); }
	bool operator!=(const vector_iter &it) { return !(current == it.current); }
	//bool operator<(const vector_iter &it) { return (current < it.current); }
	bool operator>(const vector_iter &it) { return (current > it.current); }
	bool operator>=(const vector_iter &it) { return (current >= it.current); }
	bool operator<=(const vector_iter &it) { return (current <= it.current); }

	// if this is a row iterator value -> returns iterator over current row values
	iterator begin() { iterator it(current, last, next_step); return it; }
	const_iterator begin() const { return begin(); }
	const_iterator cbegin() { return begin(); }
	
	iterator end() 
	{
		if (step == 1)
		{
			iterator it(last, last, next_step); 
			return it; 
		}
		else
		{
			item_iterator x(current);
			std::advance(x, step);
			iterator it(x, last, next_step); 
			return it; 
		}
	}
	const_iterator cend() { return end(); }
	const_iterator end() const { return end(); }

	vector_iter(item_iterator current, item_iterator last, difference_type step, difference_type next_step) : 
		last(last), current(current), step(step), next_step(next_step), 
		size_(std::distance(current, last) / step) { }
	vector_iter(const vector_iter &it) : 
		last(it.last), current(it.current), step(it.step), next_step(it.next_step), size_(it.size_) { }
	~vector_iter() { }
};

public:
	// container definition
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T value_type;
	typedef T& reference;
	typedef const T& const_reference;
	//
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef std::vector<value_type> matrix_type;
	// iterator over matrix elements (all)
	typedef typename matrix_type::iterator iterator;
	typedef typename matrix_type::const_iterator const_iterator;
	// iterator over rows
	typedef vector_iter row_iterator;
	typedef const row_iterator const_row_iterator;
	// iterator over columns
	typedef vector_iter column_iterator;
	typedef const column_iterator const_column_iterator;

private:
	size_type cols;
	size_type rows;
	matrix_type data;

	void check_comp(const matrix2<value_type> &m) const
	{
		if (cols != m.cols || rows != m.rows)
			throw new exception("Incompatible matrix");
	}

public:
	void print(std::ostream &stream) const;
	bool parse(std::istream &stream);

	// at the nested container is a vector you could access items in the matrix by overflowing the column index
	reference at(size_type r, size_type c) { if (r >= rows || c >= cols) return data.at(data.size() + 1); return data.at(r * cols + c); }
	const_reference at(size_type r, size_type c) const { if (r >= rows || c >= cols) return data.at(data.size() + 1);  return data.at(r * cols + c); }
	
	// number of matrix elements
	size_type size() const { return data.size(); }
	// only for matrix of type (0x0)
	bool empty() const { return data.empty(); }
	bool operator==(const matrix2 &m) const
	{   // let the nested containter to do the comparison (vector probably)
		return column_size() == m.column_size() && row_size() == m.row_size() && data == m.data;
	}
	bool operator!=(const matrix2 &m) const { return !operator==(m); }
	matrix2 &operator=(const matrix2 &m);

	iterator begin() { return data.begin(); }
	const_iterator begin() const { return data.begin(); }
	const_iterator cbegin() { return data.cbegin(); }

	iterator end() { return data.end(); }
	const_iterator end() const { return data.end(); }
	const_iterator cend() { return data.cend(); }

	// ROWS
	size_type row_size() const { return rows; }
	//
	row_iterator row_begin() 
	{   // column_size() - how many items must be jumped to get to the next row
		// 1 - when we are iterating over the row - each step just 1
		row_iterator b(data.begin(), data.end(), column_size(), 1);	// first item of the first row
		return b;
	}
	const_row_iterator row_begin() const { return row_begin(); }
	const_row_iterator row_cbegin() { return row_begin(); }
	//
	row_iterator row_end() 
	{
		row_iterator e(data.end(), data.end(), column_size(), 1);	// first item after the last row
		return e;
	}
	const_row_iterator row_end() const { return row_end(); }
	const_row_iterator row_cend() { return row_end(); }

	// COLUMNS
	size_type column_size() const { return cols; }
	//
	column_iterator column_begin()
	{   // 1 - to jump to the next column we need to jump 1 item
		// row_size() - to jump to the next item on the column we need to jump over all items on row
		column_iterator b(data.begin(), data.end(), 1, row_size()); 
		return b; 
	}
	const_column_iterator column_begin() const { return column_begin(); }
	const_column_iterator column_cbegin() { return column_begin(); }
	//
	column_iterator column_end() 
	{
		auto last_column(data.begin());				// first item of the first column
		std::advance(last_column, row_size());	// first item after the last column
		column_iterator e(last_column, data.end(), 1, row_size()); 
		return e;
	}
	const_column_iterator column_end() const { return column_end(); }
	const_column_iterator column_cend() { return column_end(); }

	matrix2<T>& operator+=(const matrix2<T> &left);
	matrix2<T>& operator*=(const matrix2<T> &left);
	friend std::istream &operator>>(std::istream &stream, matrix2<T> &m) 
	{ 
		if (!m.parse(stream))
			throw exception("Invalid input data");
		return stream; 
	}
	friend std::ostream &operator<<(std::ostream &stream, const matrix2<T> &m) { m.print(stream); return stream; }
	
	// default constructor without parameters is needed for container definition
	matrix2(size_type m = 0, size_type n = 0) : cols(n), rows(m), data(m * n, 0) 
	{
		for (size_type i = 0; i < m * n; ++i)
			data[i] = i;
	}
	// copy constructor is needed for container definition
	matrix2(const matrix2<value_type> &m) : cols(m.cols), rows(m.rows), data(m.data) {}
	~matrix2(void) { }
};

template<typename T>
matrix2<T>& matrix2<T>::operator=(const matrix2<T> &m)
{
	cols = m.cols;
	rows = m.rows;
	data = m.data;	// let the nested container to do the assignment (vector probably)
	return *this;
};

template<typename T>
matrix2<T>& matrix2<T>::operator+=(const matrix2<T> &left)
{
	check_comp(left);

	for (int i = 0; i < data.size(); ++i)
		data[i] += left.data[i];

	return *this;
};

template<typename T>
matrix2<T>& matrix2<T>::operator*=(const matrix2<T> &left)
{
	if (cols != left.rows)
		throw new exception("Incompatible matrix");

	// new matrix will have the same number of rows, number of columns change
	// to left.cols
	matrix2<T> right(*this);		// copy of this matrix
	data.resize(rows * left.cols);
	cols = left.cols;

	for (size_type r = 0; r < right.row_size(); ++r)
	{
		for (size_type c = 0; c < left.column_size(); ++c)
		{
			T sum = 0;
			for (size_type i = 0; i < left.row_size(); ++i)
			{
				size_type a = right.at(r, i);
				size_type b = left.at(i, c);
				sum += right.at(r, i) * left.at(i, c);
			}
			at(r, c) = sum;
		}
	}

	return *this;
};

template<typename T>
void matrix2<T>::print(std::ostream &stream) const
{
	stream << setw(4) << this->rows << "," << this->cols << endl;

	for (int r = 0; r < row_size(); ++r)
	{
		for (int c = 0; c < column_size(); ++c)
			stream << setw(4) << at(r, c);
		stream << endl;
	}
}
template<typename T>
bool matrix2<T>::parse(std::istream &stream)
{
	const char left = '{';
	const char right = '}';
	const char sep = ',';
	char ch;

	stream >> std::skipws >> ch >> std::skipws;
	if (ch != left) return false;

	size_type rows;
	stream >> rows >> std::skipws >> ch >> std::skipws;
	if (ch != sep) return false;

	size_type cols;
	stream >> cols >> std::skipws;

	matrix2<T> m(rows, cols);
	T elem;
	for (size_type r = 0; r < rows; ++r)
	{
		stream >> std::skipws >> ch;
		if (ch != left) return false;

		for (size_type c = 0; c < cols; ++c)
		{
			stream >> std::skipws >> elem >> std::skipws >> ch;
			if (ch != sep && ch != right) return false;
			m.at(r, c) = elem;
		}

		//stream >> std::skipws >> ch;
		if (ch != right) return false;
	}
	
	stream >> std::skipws >> ch;
	if (ch != right) return false;
	
	operator=(m);
	return true;
}

template<typename T>
matrix2<T> operator+(const matrix2<T> &left, const matrix2<T> &right)
{
	matrix2<T> m(left);
	m += right;
	return m;
}

template<typename T>
matrix2<T> operator*(const matrix2<T> &left, const matrix2<T> &right)
{
	matrix2<T> m(left);
	m *= right;
	return m;
}

/*template<typename T>
matrix2<T> ::operator*=(const matrix2<T> &left)
{
	matrix2 m(l);
	m *= this;
	return m;
};*/

/*template<typename T, typename U>
matrix2<decltype(T * U + T * U)>::operator*(const matrix2<T> &right, const matrix2<T> &left)
{

}*/
