#ifndef _DU1CONTAINER_HPP
#define _DU1CONTAINER_HPP

#include "task1.hpp"

class du1container {
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
	public:
		void push_back( const plain_row< D> & v) const 
		{
			//... append the data to the end of the polymorphic vector
		}
	};

	template< typename D>
	magic< D> register_type()
	{
		//... create a magic for the type descriptor D
		return magic< D>();
	}

	template< typename A>
	class dynamic_polyfunctor
	{
	public:
		dynamic_polyfunctor( const du1container & d)
		{
			//...
		}

		template< typename D>
		dynamic_polyfunctor & operator<<( magic< D> m)
		{
			//... register a magic in this polyfunctor
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
