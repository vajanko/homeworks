#ifndef _TASK1_HPP
#define _TASK1_HPP

#include <cstddef>
#include <memory>
#include <vector>
#include <cassert>
#include <stdexcept>

#include "testbed.hpp"

//#define USE_VARIADIC
#define STATIC_ITERATORS

#ifdef USE_VARIADIC
#include <tuple>

namespace ulibpp {
#ifdef STATIC_ITERATORS

	template< typename TT, std::size_t i>
	class tuple_iterator
	{
	public:
		tuple_iterator( TT & t)
		: t_( t)
		{
		}

		typedef tuple_iterator< TT, i + 1> next_type;

		next_type operator ++() const
		{
			return next_type( t_);
		}

		typename std::conditional< i != std::tuple_size< TT>::value, std::tuple_element< i, TT>, std::decay< int>>::type::type & operator *() const
		{
			return std::get< i>( t_);
		}
		/*
		typename std::tuple_element< i, TT>::type * operator ->() const
		{
			return std::get< i>( t_);
		}
		*/
	private:
		TT & t_;
	};

	template< typename TT>
	tuple_iterator< TT, 0> static_begin( TT & t)
	{
		return tuple_iterator< TT, 0>( t);
	}

	template< typename TT>
	tuple_iterator< TT, std::tuple_size< TT>::value> static_end( TT & t)
	{
		return tuple_iterator< TT, std::tuple_size< TT>::value>( t);
	}

	template< typename ITB, typename ITE, typename F>
	struct static_for_each_str_
		: public static_for_each_str_< typename ITB::next_type, ITE, F>
	{
		static_for_each_str_( ITB itb, ITE ite, F f)
		: static_for_each_str_< typename ITB::next_type, ITE, F>( ++ itb, ite, f)
		{
			this->f_( * itb);
		}
	};

	template< typename ITE, typename F> 
	struct static_for_each_str_< ITE, ITE, F>
	{
		static_for_each_str_( ITE itb, ITE ite, F f)
		: f_( f)
		{
		}

		operator F() const
		{
			return f_;
		}
	protected:
		F f_;
	};

	template< typename ITB, typename ITE, typename F>
	F static_for_each( ITB itb, ITE ite, F f)
	{
		return static_for_each_str_< ITB, ITE, F>( itb, ite, f);
	}

	template< typename TT, typename F>
	F tuple_for_each( TT & t, F f)
	{
		return static_for_each( static_begin( t), static_end( t), f);
	}

#else
	template< typename F, std::size_t i, typename TT> 
	struct for_each_str
		: public for_each_str< F, i - 1, TT>
	{
		for_each_str( F f, TT & t)
		: for_each_str< F, i - 1, TT>( f, t)
		{
			this->f_( std::get< i - 1>( t));
		}
	};

	template< typename F, typename TT> 
	struct for_each_str< F, 0, TT>
	{
		for_each_str( F f, TT &)
		: f_( f)
		{
		}

		operator F() const
		{
			return f_;
		}
	protected:
		F f_;
	};

	template< typename TT, typename F>
	F tuple_for_each( TT & t, F f)
	{
		return for_each_str< F, std::tuple_size< TT>::value, TT>( f, t);
	}
#endif

	template< typename T, std::size_t i, typename TT, typename ... Types> 
	struct get_str;

	template< typename T, std::size_t i, typename TT, typename T1, typename ... Types> 
	struct get_str< T, i, TT, T1, Types ...>
		: public get_str< T, i + 1, TT, Types ...>
	{
	};

	template< typename T, std::size_t i, typename TT, typename ... Types> 
	struct get_str< T, i, TT, T, Types ...>
	{
		static T & get( TT & t)
		{
			return std::get< i>( t);
		}
	};

	template< typename T, typename ... Types>
	T & get( std::tuple< Types ...> & t)
	{
		return get_str< T, 0, std::tuple< Types ...>, Types ...>::get( t);
	}
};
#endif

///////////////////////////////

typedef int f_data;

template< std::size_t N>
struct array_data_policy
{
	template< template< typename T> class F>
	class muster {
	public:
		typedef typename F< f_data>::type a_type;
		a_type a_[ N];
	};

	template< typename C>
	static void ctor( C & t)
	{
		std::fill_n( t.a_, N, 1);
	}

	template< typename C, typename B>
	static void cctor( C & t, B & u)
	{
		std::copy_n( u.a_, N, t.a_);
	}

	template< typename C, typename B, typename D>
	static void cctor( C & t, B & u, D & d)
	{
		std::transform( u.a_, u.a_ + N, t.a_, 
			[&]( const typename B::a_type & tt)
		{
			return typename C::a_type( tt, d); 
		});
	}

	template< typename C>
	static const typename C::a_type & selected_field( const C & t)
	{
		return t.a_[ N - 1];
	}
};

template< typename T> 
struct identity
{
	typedef T type;
};

template< std::size_t N>
struct array_data_plain_row 
	: public array_data_policy< N>::template muster< identity>
{
	array_data_plain_row()
	{
		array_data_policy< N>::ctor( * this);
	}
};

/*
template< typename D>
class plain_row 
: public D::template muster< identity>
{
public:
	plain_row()
	{
		D::ctor( * this);
	}

	template< typename T>
	T & operator()( T & v) const 
	{
		return v;
	}
};
*/

template< typename S>
class abstract_item_factory {
public:
	virtual ~abstract_item_factory() {}

	void make( S & s) const { make_( s); }
//	f_data reordered_f(S & s) const { reordered_f_(s); }

private:
	virtual void make_(S & s) const = 0;
//	virtual f_data reordered_f_(S & s) const = 0;
};

template< typename S, typename my_data>
class item_factory : public abstract_item_factory< S> {
public:
	item_factory( S & s)
		: m_( s.template register_type< my_data>())
	{
	}

	typename S::template magic< my_data> m_;

private:
	virtual void make_( S & s) const
	{
//		m_.push_back( plain_row< my_data>());
		m_.push_back( my_data());
	}
};

template< typename S, std::size_t N>
//class array_item_factory : public item_factory< S, array_data_policy< N> > {
class array_item_factory : public item_factory< S, array_data_plain_row< N> > {
public:
	array_item_factory( S & s)
//		: item_factory< S, array_data_policy< N> >( s)
		: item_factory< S, array_data_plain_row< N> >( s)
	{
	}
};

struct f_ftor
{
	f_ftor()
	: s_(0)
	{
	}

	template< typename D, typename IT>
	void vector_call( IT b, IT e)
	{
		ftor_rebind< D> f2;
		f2.vector_call( b, e);
		s_ += f2.result();
	}

	template< typename D, typename C>
	void call( const C & c)
	{
		ftor_rebind< D> f2;
		f2( c);
		s_ += f2.result();
	}

	template< typename T>
	struct ftor_rebind;

	template< std::size_t N>
	//struct ftor_rebind< array_data_policy< N>>
	struct ftor_rebind< array_data_plain_row< N>>
	{
		ftor_rebind()
		: s_(0)
		{
		}

		template< typename IT>
		void vector_call( IT b, IT e)
		{
			* this = std::for_each( b, e, * this);
		}

		template< typename T>
		void operator()( const T & c)
		{
			s_ += array_data_policy< N>::selected_field( c);
		}

		f_data result() const
		{
			return s_;
		}
	private:
		f_data s_;
	};

	f_data result() const
	{
		return s_;
	}
private:
	f_data s_;
};

#ifdef USE_VARIADIC

template< typename S, typename my_data>
class magic_holder {
public:
	magic_holder( S & s)
		: m_( s.register_type< my_data>())
	{
	}

	typename S::template magic< my_data> m_;

	void make( S & s) const
	{
		m_.push_back( plain_row< my_data>());
	}
};

template< typename S>
struct generator_1_tl_adaptor {
private:
	template< typename ... TL>
	struct data_type_base : public S
	{
		data_type_base()
		: m( magic_holder< S, TL>( * this) ...) 
		{
		}
		std::tuple< magic_holder< S, TL> ... > m;

		template< typename F>
		F for_each_type( F f)
		{
			return ulibpp::tuple_for_each( m, f);
		}

		template< typename T>
		void push_back( const plain_row< T> & x)
		{
			magic_holder< S, T> & mm = ulibpp::get< magic_holder< S, T> >( m);

			mm.m_.push_back( x);
		}
	};

public:
	template< typename ... TL>
	struct data_type : public data_type_base< TL ...>
	{
		data_type()
		: dpf( * this)	
		{
			// dpf << m1.m_ << m2.m_ << m3.m_ << m4.m_ << m5.m_;
			this->for_each_type( dpf_filler( dpf));
		}

		template< typename A>
		A unordered_for_each_static( A f) const
		{
			return unordered_for_each( dpf( f));
		}

		template< typename A>
		A ordered_for_each_static( A f) const
		{
			return ordered_for_each( dpf( f));
		}

		typename S::template dynamic_polyfunctor< f_ftor> dpf;
	};

private:
	struct dpf_filler
	{
		dpf_filler( typename S::template dynamic_polyfunctor< f_ftor> & dpf)
		: dpf_( dpf)
		{
		}

		template< typename X>
		void operator()( const X & x)
		{
			dpf_ << x.m_;
		}
	private:
		typename S::template dynamic_polyfunctor< f_ftor> & dpf_;
	};
};

template< template< typename ... TLL> class ST, typename P, typename ... TL>
struct generator_1_tl_static {

	typedef ST< TL ...> data_type;

	typedef f_data check_type;

	class sole_abstract_item_factory {
	public:
		virtual ~sole_abstract_item_factory() {}

		void make( data_type & s) const { make_( s); }

	private:
		virtual void make_(data_type & s) const = 0;
	};

	template< typename my_data>
	class sole_item_factory : public sole_abstract_item_factory {
	public:
		sole_item_factory()
		{
		}
	private:
		virtual void make_( data_type & s) const
		{
			s.push_back< my_data>( plain_row< my_data>());
		}
	};

	typedef const sole_abstract_item_factory * sole_item_factory_ptr;
	typedef std::vector< sole_item_factory_ptr> sole_item_factory_vector;

	static std::string name() { return data_type::name() + "_" + P::name(); }

	std::string param() const 
	{ 
		return ulibpp::lexical_cast< std::string>( ICOUNT);
	}

	time_complexity complexity() const 
	{ 
		return ICOUNT;
	}

	check_type check() const
	{
		return (check_type)ICOUNT;
	}

	const data_type & data() const
	{
		return s_;
	}

	template< typename GP>
	generator_1_tl_static( const GP &, std::size_t icount) 
		: ICOUNT( icount)
	{
		std::tuple< sole_item_factory< TL> ... > ift;
		sole_item_factory_vector ifv;

		ulibpp::tuple_for_each( ift, ifv_filler( ifv));

		for ( std::size_t i = 0; i < ICOUNT; ++ i)
		{
			ifv.at( P::type_index( i, ifv.size(), ICOUNT))->make( s_);
		}
	}

private:
	const std::size_t ICOUNT;
	data_type s_;

	struct ifv_filler
	{
		ifv_filler( sole_item_factory_vector & ifv)
		: ifv_( ifv)
		{
		}

		template< typename X>
		void operator()( const X & x)
		{
			ifv_.push_back( & x);
		}
	private:
		sole_item_factory_vector & ifv_;
	};
};

template< typename S, typename P>
struct generator_1 : public generator_1_tl_static< 
	typename generator_1_tl_adaptor< S>::data_type, 
	P, 
	array_data_policy< 1>, 
	array_data_policy< 2>, 
	array_data_policy< 3>, 
	array_data_policy< 4>, 
	array_data_policy< 5> >
{
	template< typename GP>
	generator_1( const GP & g, std::size_t icount) 
		: generator_1_tl_static< 
	typename generator_1_tl_adaptor< S>::data_type, 
	P, 
	array_data_policy< 1>, 
	array_data_policy< 2>, 
	array_data_policy< 3>, 
	array_data_policy< 4>, 
	array_data_policy< 5> >( g, icount)
	{
	}
};

#else

template< typename S, typename P>
struct generator_1 {

	struct data_type_base : public S
	{
		data_type_base()
		: m1( * this),
		m2( * this),
		m3( * this),
		m4( * this),
		m5( * this)
		{
		}
		array_item_factory< S, 1> m1;
		array_item_factory< S, 2> m2;
		array_item_factory< S, 3> m3;
		array_item_factory< S, 4> m4;
		array_item_factory< S, 5> m5;
	};

	struct data_type : public data_type_base
	{
		using data_type_base::m1;
		using data_type_base::m2;
		using data_type_base::m3;
		using data_type_base::m4;
		using data_type_base::m5;
		//using data_type_base::unordered_for_each;
		//using data_type_base::ordered_for_each;

		data_type()
		: dpf( * this)	
		{
			dpf << m1.m_ << m2.m_ << m3.m_ << m4.m_ << m5.m_;
		}

		template< typename A>
		A unordered_for_each_static( A f) const
		{
			return unordered_for_each( dpf( f));
		}

		template< typename A>
		A ordered_for_each_static( A f) const
		{
			return ordered_for_each( dpf( f));
		}

		typename S::template dynamic_polyfunctor< f_ftor> dpf;
	};
	typedef f_data check_type;
		
	typedef const abstract_item_factory< S> * item_factory_ptr;
	typedef std::vector< item_factory_ptr> item_factory_vector;

	static std::string name() { return S::name() + "_" + P::name(); }

	/*
	std::string param() const 
	{ 
		return ulibpp::lexical_cast< std::string>( ICOUNT);
	}
	*/

	time_complexity complexity() const 
	{ 
		return ICOUNT;
	}

	check_type check() const
	{
		return (check_type)ICOUNT;
	}

	const data_type & data() const
	{
		return s_;
	}

	template< typename GP>
	generator_1( const GP &, std::size_t icount) 
		: ICOUNT( icount)
	{
		item_factory_vector ifv_;

		ifv_.push_back( & s_.m1);
		ifv_.push_back( & s_.m2);
		ifv_.push_back( & s_.m3);
		ifv_.push_back( & s_.m4);
		ifv_.push_back( & s_.m5);

		for ( std::size_t i = 0; i < ICOUNT; ++ i)
		{
			ifv_.at( P::type_index( i, ifv_.size(), ICOUNT))->make( s_);
		}
	}

private:
	const std::size_t ICOUNT;
	data_type s_;
};

#endif

struct policy_robin {

	static std::string name() { return "robin"; }

	static std::size_t type_index( std::size_t i, std::size_t s, std::size_t c)
	{
		return i % s;
	}
};

struct policy_random {

	static std::string name() { return "random"; }

	static std::size_t type_index( std::size_t i, std::size_t s, std::size_t c)
	{
		return std::rand() % s;
	}
};

struct policy_grouped {

	static std::string name() { return "grouped"; }

	static std::size_t type_index( std::size_t i, std::size_t s, std::size_t c)
	{
		return i * s / c;
	}
};

struct task_1 {

	static std::string name() { return "unordered_for_each"; }

	static time_complexity complexity() 
	{ 
		return 1;
	}

	template< bool cold, bool debug, typename D, typename C>
	static void run( const D & data, const C & check)
	{
		f_data s = data.unordered_for_each_static( f_ftor()).result();

		//assert( s == check);
		if ( s != check )
			throw std::logic_error( name());
	}

	template< bool debug, typename D, typename C>
	static void initial_check( logger & log, const D & data, const C & check)
	{
	}

	template< bool debug, typename D, typename C>
	static void final_check( logger & log, const D & data, const C & check)
	{
	}
};

struct task_1O {

	static std::string name() { return "ordered_for_each"; }

	static time_complexity complexity() 
	{ 
		return 20;
	}

	template< bool cold, bool debug, typename D, typename C>
	static void run( const D & data, const C & check)
	{
		f_data s = data.ordered_for_each_static( f_ftor()).result();

		//assert( s == check);
		if ( s != check )
			throw std::logic_error( name());
	}

	template< bool debug, typename D, typename C>
	static void initial_check( logger & log, const D & data, const C & check)
	{
	}

	template< bool debug, typename D, typename C>
	static void final_check( logger & log, const D & data, const C & check)
	{
	}
};

#endif

