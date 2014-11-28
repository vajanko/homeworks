#ifndef GUARD_MLASKAL_EL_SHR_PTR_HPP_
#define GUARD_MLASKAL_EL_SHR_PTR_HPP_

/*

	el_shr_ptr.h

	TR1 addition to the <memory> header

	Kuba, Feb 2006
	Extra-Ligth Shared Ptr

	Namely shared_ptr<T>

*/

#include <ostream>
#include <cstddef>
#include <utility>
#include <cassert>

namespace mlaskal {

	/// @cond INTERNAL

	namespace el_shr_ptr_int_ {

		struct shared_ptr_static_cast { };
		struct shared_ptr_dynamic_cast { };
		struct shared_ptr_const_cast { };

		template <typename T>
		struct shared_ptr_ref_wrk {
			typedef T& type;
		};
		template <>
		struct shared_ptr_ref_wrk<void> {
			typedef void type;
		};
		template <>
		struct shared_ptr_ref_wrk<void const> {
			typedef void type;
		};
		template <>
		struct shared_ptr_ref_wrk<void volatile> {
			typedef void type;
		};
		template <>
		struct shared_ptr_ref_wrk<void const volatile> {
			typedef void type;
		};

		class shared_ptr_common_base {
		public:
			explicit shared_ptr_common_base() : cnt_(1) { }
			virtual ~shared_ptr_common_base() { }
			void acquire() { ++cnt_; }
			void dispose() { if(--cnt_==0) { destroy(); delete this; } }
			long get_count() const { return cnt_; }
		private:
			volatile long		cnt_;
			virtual void destroy()=0;
		};

		template <typename T>
		class shared_ptr_common_t : public shared_ptr_common_base {
		public:
			explicit shared_ptr_common_t(T *t) : ptr_(t) { };
		private:
			T		*ptr_;
			virtual void destroy() { delete ptr_; }
		};

		class shared_ptr_holder {
		public:
			shared_ptr_holder() : com_(0) { }

			template <typename Y>
			explicit shared_ptr_holder(Y *p) : com_(0) {
				try {
					com_ = new shared_ptr_common_t<Y>(p);
				} 
				catch(...) {
					delete p;
					throw;
				}
			}
			
			~shared_ptr_holder() { if(com_!=0) com_->dispose(); }

			shared_ptr_holder(const shared_ptr_holder &sph) : com_(sph.com_) {
				if(com_!=0)
					com_->acquire();
			}

			shared_ptr_holder &operator=(const shared_ptr_holder &sph) {
				shared_ptr_common_base *spcb = sph.com_;
				if(spcb!=com_)			// assign different shared objects
				{
					if(spcb!=0)
						spcb->acquire();
					if(com_!=0)
						com_->dispose();
					com_ = spcb;
				}
				return *this;
			}

			void swap(shared_ptr_holder &sph) { std::swap(com_, sph.com_); }

			long get_count() const { return com_!=0?com_->get_count():0; }

		private:
			shared_ptr_common_base		*com_;

			friend bool operator<(const shared_ptr_holder &lhs, const shared_ptr_holder &rhs);
		};

		inline bool operator<(const shared_ptr_holder &lhs, const shared_ptr_holder &rhs) { return lhs.com_<rhs.com_; }
	}

	/// @endcond

	namespace tr1 {

		/// shared pointer as defined by ISO C++ 2003 TR1
		template<class T>
		class shared_ptr {
		private:

			typedef typename ::mlaskal::el_shr_ptr_int_::shared_ptr_ref_wrk<T>::type my_ref_type_;

			T													*ptr_;
			::mlaskal::el_shr_ptr_int_::shared_ptr_holder	cnt_;

			template<class Y> friend class shared_ptr;		// make access for different covertible types Y
			template<class X, class Y> friend bool operator<(shared_ptr<X> const& a, shared_ptr<Y> const& b);

			template<class Y> bool internal_less_cmp_(const shared_ptr<Y> &rhs) const { return cnt_<rhs.cnt_; }

		public:
			typedef T element_type;
			typedef T value_type;		// usual name, but it is NOT in TR1!!!

			// [2.2.3.1] constructors
			shared_ptr() : ptr_(0) { }				// does "nothing"

			template<class Y> 
			explicit shared_ptr(Y * p) : ptr_(p),cnt_(p) { }

			// ignore default copy-constructors, generated one should be fine
			// shared_ptr(shared_ptr const& r);
			
			template<class Y> 
			shared_ptr(shared_ptr<Y> const& r) : ptr_(r.ptr_),cnt_(r.cnt_) { }
			// helpers for shared_ptr casts
			template<class Y> 
			shared_ptr(shared_ptr<Y> const& r, ::mlaskal::el_shr_ptr_int_::shared_ptr_static_cast) : ptr_(static_cast<T*>(r.ptr_)),cnt_(r.cnt_) { }
			template<class Y> 
			shared_ptr(shared_ptr<Y> const& r, ::mlaskal::el_shr_ptr_int_::shared_ptr_dynamic_cast) : ptr_(dynamic_cast<T*>(r.ptr_)),cnt_(r.cnt_)
			{
				if(ptr_==0)		// dynamic_cast failed -> set the empty counter
					cnt_ = ::mlaskal::el_shr_ptr_int_::shared_ptr_holder();
			}
			template<class Y> 
			shared_ptr(shared_ptr<Y> const& r, ::mlaskal::el_shr_ptr_int_::shared_ptr_const_cast) : ptr_(const_cast<T*>(r.ptr_)),cnt_(r.cnt_) { }

			// [2.2.3.2] destructor
			// ignore destructor, generated one should be fine
			// ~shared_ptr();

			// [2.2.3.3] assignment
			// ignore default assignment, generated one should be fine
			// shared_ptr& operator=(shared_ptr const& r);
			
			template<class Y>
			shared_ptr& operator=(shared_ptr<Y> const& r) { cnt_ = r.cnt_; ptr_ = r.ptr_; return *this; }

			// ignoring auto_ptr for now, so it is now disabled
			// template<class Y> shared_ptr& operator=(std::auto_ptr<Y>& r);

			// [2.2.3.4] modifiers
			void swap(shared_ptr& r) { cnt_.swap(r.cnt_); std::swap(ptr_, r.ptr_); }

			void reset() { shared_ptr<T>().swap(*this); }

			template<class Y> 
			void reset(Y * p) { shared_ptr<Y>(p).swap(*this); }

			// [2.2.3.5] observers
			T* get() const { return ptr_; }

			my_ref_type_ operator*() const { assert(ptr_!=0); return *ptr_; }

			T* operator->() const { assert(ptr_!=0); return ptr_; }

			long use_count() const { return cnt_.get_count(); }
			bool unique() const { return use_count()==1; }

			typedef T * shared_ptr<T>::*	unspecified_bool_type;

			operator unspecified_bool_type () const { return cnt_.get_count()?&shared_ptr<T>::get:0; }
		};

		// [2.2.3.6] shared_ptr comparisons
		template<class T, class U>
		bool operator==(shared_ptr<T> const& a, shared_ptr<U> const& b) { return a.get()==b.get(); }
		template<class T, class U>
		bool operator!=(shared_ptr<T> const& a, shared_ptr<U> const& b) { return a.get()!=b.get(); }
		template<class T, class U>
		bool operator<(shared_ptr<T> const& a, shared_ptr<U> const& b) { return a.internal_less_cmp_(b); }

		// [2.2.3.7] shared_ptr I/O
		template<class E, class T, class Y>
		std::basic_ostream<E, T>& operator<< (std::basic_ostream<E, T>& os, shared_ptr<Y> const& p) { return os << p.get(); }

		// [2.2.3.8] shared_ptr specialized algorithms
		template<class T>
		void swap(shared_ptr<T>& a, shared_ptr<T>& b) { a.swap(b); }

		// [2.2.3.9] shared_ptr casts
		template<class T, class U>
		shared_ptr<T> static_pointer_cast(shared_ptr<U> const& r) { return shared_ptr<T>(r, ::mlaskal::el_shr_ptr_int_::shared_ptr_static_cast()); }
		template<class T, class U>
		shared_ptr<T> dynamic_pointer_cast(shared_ptr<U> const& r) { return shared_ptr<T>(r, ::mlaskal::el_shr_ptr_int_::shared_ptr_dynamic_cast()); }
		template<class T, class U>
		shared_ptr<T> const_pointer_cast(shared_ptr<U> const& r) { return shared_ptr<T>(r, ::mlaskal::el_shr_ptr_int_::shared_ptr_const_cast()); }
	}

}

#endif
