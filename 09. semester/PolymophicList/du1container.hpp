#ifndef _DU1CONTAINER_HPP
#define _DU1CONTAINER_HPP

#include "task1.hpp"

class du1container {
private:

public:
	void push_back() {}
	static std::string name() { return "empty"; }

	std::size_t byte_size() const 
	{ 
		//... estimate the memory cost
		return 0;
	}

	class magic_base
	{
	public:
		
	};

	template< typename D>
	class magic : public magic_base
	{
	private:
		du1container &con;
		// TODO: unique_ptr
		std::vector<D> *data;
	public:
		//void push_back( const plain_row< D> & v) const 
		void push_back( const D & v) const
		{
			data->push_back(v);
			//... append the data to the end of the polymorphic vector
			// TODO: notify container about this item
		}
		template< typename A>
		void foreach(A &fctor)
		{
			for (auto b = data->begin(); b != data->end(); b++)
				fctor.call(*b);
		}
		magic(du1container &con) : con(con), data(new std::vector<D>()) { }
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
	public:
		typedef void(*func_type)(A&);
		func_type f1;

	public:
		dynamic_polyfunctor( const du1container & d)
		{
			//...
		}

		template< typename D>
		dynamic_polyfunctor & operator<<( magic< D> m)
		{
			//typedef void(magic<D>::*func_t)(A&);
			//... register a magic in this polyfunctor
			//func_t fun = &magic<D>::foreach<A>;
			//this->f1 = fun;
			//void(magic<D>::foreach)(A)
			return * this;
		}

		std::pair< const dynamic_polyfunctor< A> &, A> operator()( A a) const
		{
			return std::pair< const dynamic_polyfunctor< A> &, A>( * this, a);
		}
	};

	template< typename A>
	A unordered_for_each( std::pair< const dynamic_polyfunctor< A> &, A> dpf) const
	{
		//... pass in any order
		dpf.first.f1(dpf.second);
		return dpf.second;
	}

	template< typename A>
	A ordered_for_each( std::pair< const dynamic_polyfunctor< A> &, A> dpf) const
	{
		//... pass in the original order
		return dpf.second;
	}
};

#endif
