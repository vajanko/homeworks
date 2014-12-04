#ifndef _DU1CONTAINER_HPP
#define _DU1CONTAINER_HPP

#include "task1.hpp"
#include <array>
#include <xmmintrin.h>

typedef std::size_t type_id;
struct Counter
{
	static type_id value;
};

type_id Counter::value = 0;

template<typename T>
struct TypeID : private Counter
{
	static type_id value()
	{
		static type_id value = Counter::value++;
		return value;
	}
};

template <typename T, size_t TALIGN = 16, size_t TBLOCK = 8>
class aligned_allocator : public std::allocator<T>
{
public:
	aligned_allocator() {}
	aligned_allocator& operator=(const aligned_allocator &rhs){
		std::allocator<T>::operator=(rhs);
		return *this;
	}

	pointer allocate(size_type n, const void *hint){
		pointer p = NULL;
		size_t count = sizeof(T) * n;
		size_t count_left = count % TBLOCK;
		if (count_left != 0)
		{
			count += TBLOCK - count_left;
		}
		if (!hint)
		{
			p = reinterpret_cast<pointer>(aligned_malloc(count, TALIGN));
		}
		else{
			p = reinterpret_cast<pointer>(aligned_realloc((void*)hint, count, TALIGN));
		}
		return p;
	}

	void deallocate(pointer p, size_type n){
		aligned_free(p);
	}

	void construct(pointer p, const T &val){
		new(p)T(val);
	}

	void destroy(pointer p){
		p->~T();
	}
};


class du1container {
private:
	std::vector<std::pair<type_id, std::size_t>> data_order;

public:
	static std::string name() { return "empty"; }

	std::size_t byte_size() const 
	{ 
		//... estimate the memory cost
		return 0;
	}

	template< typename D>
	class magic
	{
	private:
		du1container &con;
		std::size_t it;

	public:
		/* Data stored in columns */
		static const std::size_t column_size = 4;
		typedef std::array<byte, column_size> column_type;
		static const std::size_t step_size = sizeof(__m128i) / column_size;
		static const std::size_t column_count = sizeof(D) / column_size + (sizeof(D) % column_size > 0 ? 1 : 0);
		typedef std::array<std::vector<column_type, aligned_allocator<column_type, 16, 16>>, column_count> data_t;

		/* Data stored as regular objects */
		//typedef std::vector<D> data_t;

		data_t *ptr;
		data_t &data;

	public:
		//void push_back( const plain_row< D> & v) const 
		void push_back( const D & v) const
		{	//... append the data to the end of the polymorphic vector

			// column-base solution
			column_type *obj = (column_type *)&v;
			for (std::size_t i = 0; i < column_count; ++i)
				data[i].push_back(*(obj + i));

			// regular solution
			//data.push_back(v);

			// save item order in the container
			if (con.data_order.empty())
			{
				con.data_order.push_back(std::pair<type_id, std::size_t>(TypeID<D>::value(), 1));
			}
			else
			{
				if (con.data_order.back().first == TypeID<D>::value())
					con.data_order.back().second++;
				else
					con.data_order.push_back(std::pair<type_id, std::size_t>(TypeID<D>::value(), 1));
			}
		}
		void begin()
		{
			it = 0;
		}
		template< typename A>
		void call(A &fctor)
		{
			// column-base solution
			column_type obj[column_count];
			for (std::size_t c = 0; c < column_count; ++c)
				obj[c] = data[c][it];
			fctor.call<D>(*((D *)obj));

			// regular solution
			//fctor.call<D>(data.at(it));

			++it;
		}
		template< typename A>
		void call(std::size_t count, A &fctor)
		{
			// column-base solution
			column_type obj[column_count];
			for (std::size_t end = it + count; it < end; ++it)
			{
				for (std::size_t c = 0; c < column_count; ++c)
					obj[c] = data[c][it];
				fctor.call<D>(*((D *)obj));
			}

			// regular solution
			/*auto b = data.begin() + it;
			auto e = b + count;
			for (auto i = b; i != e; ++i)
				fctor.call<D>(*i);*/
		}

		//template< typename A> 
		//void vector_call1(A &fctor)
		//{
		//	// sse-based solution
		//	int obj;		// there is only one column
		//	__m128i* its;
		//	its = (__m128i *)&data[0].front();
		//	std::size_t rows = data[0].size() / step_size;
		//	for (std::size_t i = 0; i < rows; ++i)
		//	{
		//		__m128i st = its[i];
		//		for (std::size_t x = 0; x < step_size; ++x)
		//			fctor.call<D>(*((D *)&st.m128i_i32[x]));
		//	}
		//	std::size_t tail = data[0].size() % step_size;
		//	if (tail > 0)
		//	{
		//		for (std::size_t x = 0; x < tail; ++x)
		//		{
		//			obj = its[rows].m128i_i32[x];
		//			fctor.call<D>(*((D *)&obj));
		//		}
		//	}
		//}

		template< typename A>
		void vector_call(A &fctor)
		{
			// sse-based solution
			int obj[column_count];
			__m128i* its[column_count];		// iterators
			for (std::size_t c = 0; c < column_count; ++c)
				its[c] = (__m128i *)&data[c].front();

			std::size_t rows = data[0].size() / step_size;
			for (std::size_t i = 0; i < rows; ++i)
			{
				for (std::size_t x = 0; x < step_size; ++x)
				{
					for (std::size_t c = 0; c < column_count; ++c)
						obj[c] = its[c][i].m128i_i32[x];
					fctor.call<D>(*((D *)obj));
				}
			}
			std::size_t tail = data[0].size() % step_size;
			if (tail > 0)
			{
				for (std::size_t x = 0; x < tail; ++x)
				{
					for (std::size_t c = 0; c < column_count; ++c)
						obj[c] = its[c][rows].m128i_i32[x];
					fctor.call<D>(*((D *)obj));
				}
			}

			// column-base solution
			//column_type obj[column_count];
			//column_type* its[column_count];
			//for (std::size_t c = 0; c < column_count; ++c)
			//	its[c] = &data[c].front();

			//std::size_t rows = data[0].size();
			//for (std::size_t i = 0; i < rows; ++i)
			//{
			//	for (std::size_t c = 0; c < column_count; ++c)
			//		obj[c] = its[c][i];
			//		//data[c][i];
			//	fctor.call<D>(*((D *)obj));
			//}

			// regular solution
			/*for (auto obj : data)
				fctor.call<D>(obj);*/
		}

		magic(du1container &con) : con(con), ptr(new data_t()), data(*ptr) { }
	};

	template< typename D>
	magic< D> register_type()
	{
		// ... create a magic for the type descriptor D
		return magic< D>(*this);
	}

	template< typename A>
	class dynamic_polyfunctor
	{
	private:
		struct magic_holder_base
		{
			virtual void begin() = 0;
			virtual void call(A &fctor) = 0;
			virtual void call(std::size_t count, A &fctor) = 0;
			virtual void vector_call(A &fctor) = 0;

			virtual ~magic_holder_base() { }
		};
		template<typename D>
		struct magic_holder : public magic_holder_base
		{
			magic<D> m;

			virtual void begin() { m.begin(); }
			virtual void call(A &fctor) { m.call(fctor); }
			virtual void call(std::size_t count, A &fctor) { m.call(count, fctor); }
			virtual void vector_call(A &fctor) 
			{
				/*if (magic<D>::column_count == 1)
					m.vector_call1(fctor);
				else*/
					m.vector_call(fctor); 
			}
			magic_holder(magic<D> m) : m(m) { }

			virtual ~magic_holder() { delete m.ptr; }
		};
		std::vector<magic_holder_base *> magics;

	public:
		dynamic_polyfunctor( const du1container & d)
		{
			//...
		}
		~dynamic_polyfunctor()
		{
			for (auto magic : magics)
				delete magic;
		}

		template< typename D>
		dynamic_polyfunctor & operator<<( magic< D> m)
		{
			magics.resize(max(magics.size(), TypeID<D>::value()));
			magics.insert(magics.begin() + TypeID<D>::value(), new magic_holder<D>(m));
			return * this;
		}

		std::pair< const dynamic_polyfunctor< A> &, A> operator()( A a) const
		{
			return std::pair< const dynamic_polyfunctor< A> &, A>( * this, a);
		}

		void begin() const
		{
			for (auto magic : magics)
				magic->begin();
		}
		void call(type_id id, A &fctor) const
		{
			magics.at(id)->call(fctor);
		}
		void call(type_id id, std::size_t count, A &fctor) const
		{
			magics.at(id)->call(count, fctor);
		}
		void vector_call(A &fctor) const
		{
			for (auto magic : magics)
				magic->vector_call(fctor);
		}
	};

	template< typename A>
	A unordered_for_each( std::pair< const dynamic_polyfunctor< A> &, A> dpf) const
	{
		//... pass in any order
		dpf.first.vector_call(dpf.second);
		return dpf.second;
	}

	template< typename A>
	A ordered_for_each( std::pair< const dynamic_polyfunctor< A> &, A> dpf) const
	{
		//... pass in the original order

		// switching between magics as needed
		/*dpf.first.begin();
		for (type_id id : data_order)
			dpf.first.call(id, dpf.second);*/

		// processing all items of same type at once
		dpf.first.begin();
		for (auto ord : data_order)
		{
			if (ord.second <= 1)
				dpf.first.call(ord.first, dpf.second);
			else
				dpf.first.call(ord.first, ord.second, dpf.second);
		}
			
		return dpf.second;
	}
};

#endif
