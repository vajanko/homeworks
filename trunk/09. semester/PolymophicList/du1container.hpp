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

	template< typename D>
	class magic
	{
	private:
		du1container &con;
		std::shared_ptr<std::vector<D>> data;
	public:
		//void push_back( const plain_row< D> & v) const 
		void push_back( const D & v) const
		{
			data->push_back(v);
			//... append the data to the end of the polymorphic vector
			// TODO: notify container about this item
		}
		template< typename A>
		void for_each(A &fctor)
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
	private:
		struct magic_holder_base
		{
			virtual void for_each(A &fctor) = 0;
		};
		template<typename D>
		struct magic_holder : public magic_holder_base
		{
			magic<D> m;
			virtual void for_each(A &fctor)
			{
				m.for_each(fctor);
			}
			magic_holder(magic<D> m) : m(m) { }
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
			magics.push_back(new magic_holder<D>(m));
			return * this;
		}

		std::pair< const dynamic_polyfunctor< A> &, A> operator()( A a) const
		{
			return std::pair< const dynamic_polyfunctor< A> &, A>( * this, a);
		}

		void for_each(A &fctor) const
		{
			for (auto magic : magics)
			{
				magic->for_each(fctor);
			}
		}
		
	};

	template< typename A>
	A unordered_for_each( std::pair< const dynamic_polyfunctor< A> &, A> dpf) const
	{
		//... pass in any order
		dpf.first.for_each(dpf.second);
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
