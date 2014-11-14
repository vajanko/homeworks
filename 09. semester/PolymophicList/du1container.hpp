#ifndef _DU1CONTAINER_HPP
#define _DU1CONTAINER_HPP

#include "task1.hpp"
#include <array>

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
	std::vector<std::size_t> data_order;

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
		type_id id;
		std::size_t it;

	public:
		static const std::size_t column_size = 4;
		typedef std::array<byte, column_size> column_type;
		static const std::size_t column_count = sizeof(D) / column_size + (sizeof(D) % column_size > 0 ? 1 : 0);
		typedef std::array<std::vector<column_type>, column_count> table_type;
		table_type *data;

    
    struct column_access
    {
      static void set_value( table_type *data, D &val )
      {

      }
      static D& get_value( table_type *data, std::size_t index )
      {

      }
    };
    

	public:
		//void push_back( const plain_row< D> & v) const 
		void push_back( const D & v) const
		{
			//... append the data to the end of the polymorphic vector
			// TODO: template cycle
			column_type *obj = (column_type *)&v;
			for (std::size_t i = 0; i < column_count; ++i)
			{
				data->at(i).push_back(*(obj + i));
			}
			// save item order in the container
			con.data_order.push_back(id);
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
			++it;
		}
		template< typename A>
		void call(std::size_t count, A &fctor)
		{
			std::size_t end = it + count;
			for (; it < end; ++it)
				;
				// TODO: template foreach
				//fctor.call<D>(*it);
		}
		template< typename A>
		void vector_call(A &fctor)
		{
			column_type obj[column_count];
			std::size_t size = data->at(0).size();
			for (std::size_t i = 0; i < size; ++i)
			{
				// TODO: template foreach
				for (std::size_t c = 0; c < column_count; ++c)
					obj[c] = data->at(c)[i];
				fctor.call<D>(*((D *)obj));
			}
		}
		magic(du1container &con, type_id id) : con(con), id(id), data(new table_type()) { }
	};

	template< typename D>
	magic< D> register_type()
	{
		// ... create a magic for the type descriptor D
		return magic< D>(*this, TypeID<D>::value());
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

			virtual ~magic_holder() { delete m.data; }
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
		dpf.first.begin();
		type_id lastId = *data_order.begin();
		size_t count = -1;
		for (type_id id : data_order)
		{
			++count;
			if (lastId != id)
			{
				dpf.first.call(lastId, count, dpf.second);
				count = 0;
				lastId = id;
			}
			
		}
		dpf.first.call(lastId, count + 1, dpf.second);
			
		return dpf.second;
	}
};

#endif
