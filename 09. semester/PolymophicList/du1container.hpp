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



class du1container {
private:
	//std::vector<std::size_t> data_order;
	// type_id - count
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
		/*static const std::size_t column_size = 4;
		typedef std::array<byte, column_size> column_type;
		static const std::size_t column_count = sizeof(D) / column_size + (sizeof(D) % column_size > 0 ? 1 : 0);
		typedef std::array<std::vector<column_type>, column_count> table_type;
		table_type *data;*/
		typedef std::vector<D> data_t;
		data_t *ptr;
		data_t &data;

    
		//struct column_access
		//{
		//	table_type &data;
		//	typedef typename table_type::reference col_reference;
		//	column_type current[column_count];
		//	
		//	template<std::size_t Index>inline void init_data(std::size_t index)
		//	{
		//		current[Index] = data.at(Index)[index];
		//		init_data<Index - 1>(index);
		//	}
		//	template<>inline void init_data<0>(std::size_t index)
		//	{
		//		current[0] = data.at(0)[index];
		//	}

		//	template<typename A>
		//	void call_step(std::size_t index, A &fctor)
		//	{
		//		//init_data<column_count - 1>(index);
		//		fctor.call<D>(*((D *)current));
		//	}
		//	template<typename A>
		//	void vector_call(A &fctor)
		//	{
		//		std::size_t size = data.at(0).size();
		//		column_type obj[column_count];
		//		for (std::size_t i = 0; i < size; ++i)
		//		{
		//			for (std::size_t c = 0; c < column_count; ++c)
		//				obj[c] = data.at(c)[i];
		//			fctor.call<D>(*((D *)current));
		//		}
		//	}
		//	column_access(table_type &data) : data(data) { }
		//};
    

	public:
		//void push_back( const plain_row< D> & v) const 
		void push_back( const D & v) const
		{
			//... append the data to the end of the polymorphic vector
			// TODO: template cycle
			/*column_type *obj = (column_type *)&v;
			for (std::size_t i = 0; i < column_count; ++i)
				data->at(i).push_back(*(obj + i));*/
			data.push_back(v);
			// save item order in the container
			if (con.data_order.empty())
				con.data_order.push_back(std::pair<type_id, std::size_t>(TypeID<D>::value(), 1));
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
			/*fctor.call<D>(*it);*/
			// TODO: template foreach
			/*column_type obj[column_count];
			for (std::size_t c = 0; c < column_count; ++c)
				obj[c] = data->at(c)[it];
			fctor.call<D>(*((D *)obj));*/
			fctor.call<D>(data.at(it));
			++it;
		}
		template< typename A>
		void call(std::size_t count, A &fctor)
		{
			auto b = data.begin() + it;
			auto e = b + count;
			for (auto i = b; i != e; ++i)
				fctor.call<D>(*i);
		}
		template< typename A>
		void vector_call(A &fctor)
		{
			for (auto obj : data)
				fctor.call<D>(obj);
			/*column_access col(*data);
			col.vector_call<A>(fctor);*/
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
			virtual void vector_call(A &fctor) { m.vector_call(fctor); }
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
