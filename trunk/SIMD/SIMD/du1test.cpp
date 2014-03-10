#include "du1simd.hpp"

#include<vector>

#include <memory>
#include <algorithm>
#include <xmmintrin.h>
#include <cassert>
#include <string>
#include <iostream>
#include <chrono>

using namespace std;

namespace du1simd {

	template< typename value_type, typename simd_carrier_type> 
	struct simd;

	template<>
	struct simd< float, float> {
		static float broadcast( float x)
		{
			return x;
		}
		static float zero()
		{
			return 0.0F;
		}
		static float add( float a, float b)
		{
			return a + b;
		}
		static float sub( float a, float b)
		{
			return a - b;
		}
		static float mul( float a, float b)
		{
			return a * b;
		}
		static float sum( float a)
		{
			return a;
		}
		static float mask_lower( float a, std::ptrdiff_t lgap)
		{
			assert( lgap == 0);
			return a;
		}
		static float mask_upper( float a, std::ptrdiff_t ugap)
		{
			assert( ugap == 0);
			return a;
		}
		static float mask_both( float a, std::ptrdiff_t lgap, std::ptrdiff_t ugap)
		{
			assert( lgap == 0);
			assert( ugap == 0);
			return a;
		}
	};

	template<>
	struct simd< float, __m128> {
		static __m128 broadcast( float x)
		{
			__m128 a = _mm_load_ss( & x);
			return _mm_shuffle_ps( a, a, 0x00);
		}
		static __m128 zero()
		{
			return _mm_setzero_ps();
		}
		static __m128 add( __m128 a, __m128 b)
		{
			return _mm_add_ps( a, b);
		}
		static __m128 sub( __m128 a, __m128 b)
		{
			return _mm_sub_ps( a, b);
		}
		static __m128 mul( __m128 a, __m128 b)
		{
			return _mm_mul_ps( a, b);
		}
		static float sum( __m128 a)
		{
			float x;
			__m128 b = _mm_hadd_ps( a, a);
			__m128 c = _mm_hadd_ps( b, b);
			_mm_store_ss( & x, c);
			return x;
		}

		static __m128 mask_lower( __m128 a, std::ptrdiff_t lgap)
		{
			assert( lgap >= 0);
			assert( lgap < 4);
			return _mm_and_ps( a, mask_data_.lmask_[ lgap]);
		}
		static __m128 mask_upper( __m128 a, std::ptrdiff_t ugap)
		{
			assert( ugap > -4);
			assert( ugap <= 0);
			return _mm_and_ps( a, mask_data_.umask_[ ugap + 3]);
		}
		static __m128 mask_both( __m128 a, std::ptrdiff_t lgap, std::ptrdiff_t ugap)
		{
			return mask_upper( mask_lower( a, lgap), ugap);
		}
	private:
		struct mask_data {
			__m128 lmask_[ 4];
			__m128 umask_[ 4];
			mask_data()
			{
				lmask_[ 0] = _mm_castsi128_ps( _mm_set_epi32( -1, -1, -1, -1));
				lmask_[ 1] = _mm_castsi128_ps( _mm_set_epi32( -1, -1, -1,  0));
				lmask_[ 2] = _mm_castsi128_ps( _mm_set_epi32( -1, -1,  0,  0));
				lmask_[ 3] = _mm_castsi128_ps( _mm_set_epi32( -1,  0,  0,  0));
				umask_[ 0] = _mm_castsi128_ps( _mm_set_epi32(  0,  0,  0, -1));
				umask_[ 1] = _mm_castsi128_ps( _mm_set_epi32(  0,  0, -1, -1));
				umask_[ 2] = _mm_castsi128_ps( _mm_set_epi32(  0, -1, -1, -1));
				umask_[ 3] = _mm_castsi128_ps( _mm_set_epi32( -1, -1, -1, -1));
			}
		};
		static const mask_data mask_data_;
	};

	const simd< float, __m128>::mask_data simd< float, __m128>::mask_data_;
};

namespace du1example {

	template< typename F>
	double measure_time( F f)
	{
		auto tb = std::chrono::steady_clock::now();
		f();
		auto te = std::chrono::steady_clock::now();
		return std::chrono::duration_cast<std::chrono::microseconds>( te - tb).count() / 1000000.0;
	}

	template< typename simd_carrier_type>
	struct tester
	{
		typedef simd_vector< float, simd_carrier_type> vector_type;

		typedef typename vector_type::iterator simd_iterator;

		typedef du1simd::simd< float, simd_carrier_type> simd_op;

		static float sum( simd_iterator b, simd_iterator e)
		{
			float acc = 0;

			for (; b != e; ++ b)
			{
				acc = acc + * b;
			}

			return acc;
		}

		static float simd_sum( simd_iterator b, simd_iterator e)
		{
			auto bb = b.lower_block();
			auto ee = e.upper_block();

			if ( bb == ee )
			{
				return 0;
			}

			-- ee;

			if ( bb == ee )
			{
				return simd_op::sum( simd_op::mask_both( * bb, b.lower_offset(), e.upper_offset()));
			}

			simd_carrier_type acc = simd_op::mask_lower( * bb, b.lower_offset());

			for ( ++ bb; bb != ee; ++ bb)
			{
				acc = simd_op::add( acc, * bb);
			}

			return simd_op::sum( simd_op::add( acc, simd_op::mask_upper( * bb, e.upper_offset())));
		}

		static void test( const std::string & name)
		{
#ifdef _DEBUG
			std::size_t K1 = 111, K2 = 700666, K3 = 729000;
#else
			std::size_t K1 = 111, K2 = 700000666, K3 = 729000000;
#endif
			float X1 = 0.0F, X2 = 1.00F;

			vector_type vec( K3);

			float gen = X1;

			std::generate( vec.begin(), vec.end(), [ & gen, X2](){
				return gen += X2;
			});

			auto b = vec.begin() + K1;
			auto e = vec.begin() + K2;

			float exp = (K2 - K1) * (X1 + (K2 + K1 + 1) * X2 / 2);
			float s1; 
			double t1 = measure_time( [ & s1, b, e](){
				s1 = sum( b, e);
			});
			float s2;
			double t2 = measure_time( [ & s2, b, e](){
				s2 = simd_sum( b, e);
			});

			assert( std::abs(s1 - s2) / std::abs(s1 + s2) < 0.001);
			assert( std::abs(s1 - exp) / std::abs(s1 + exp) < 0.001);

			std::cout << name << "/sum: " << (1000000000.0 * t1 / (K2-K1)) << " ns" << std::endl;
			std::cout << name << "/simd_sum: " << (1000000000.0 * t2 / (K2-K1)) << " ns" << std::endl;
		}
	};

	void test()
	{
		tester< float>::test( "float");
		tester< __m128>::test( "__m128");
	}
};

namespace unittest
{
	template<typename T, typename S>
	void test_simd_vector_iterator(simd_vector<T, S> &v, simd_vector_iterator<T, S> &it)
	{
		it++;
		++it;
		--it;
		it--;
		it += v.size() - 1;
		it -= v.size() - 1;
		it += v.size();
		it -= v.size();

		it = it;
		auto x = it;
		it = std::move(it);
		auto y(std::move(it));
		it = std::move(y);
		T z = *it;
		const T r = *it;
		T u = it[0];
		T p = it[1];

		it = it + 1;
		it = it - 1;

		it == it;
		it != it;
		it >= it;
		it <= it;
		it > it;
		it < it;

		simd_vector_iterator<std::pair<int, int>, S> it3;
		it3->first;
	}

	template<typename T, typename S>
	void test_vector(simd_vector<T, S> &v)
	{
		for (auto bb = v.begin().lower_block(); bb < v.end().upper_block(); ++bb)
			cout << *bb << endl;

		simd_vector<T, S>::iterator it = v.begin();
		test_simd_vector_iterator(v, it);

		simd_vector<T, S> vec2(3);
		simd_vector<T, S>::iterator it2;

		//it == vec2.begin();
		//it2 == it2;

		typedef typename simd_vector<T, S>::iterator vec_iter;
		vec_iter::value_type val = vec_iter::value_type();
		vec_iter::pointer p = nullptr;
		vec_iter::const_pointer cp = nullptr;
		vec_iter::reference ref = val;

		simd_vector<T, S>::iterator it1_;
		simd_vector<T, S>::simd_iterator it2_;
	}

	template<typename T, typename S>
	void test_invariants()
	{
		typedef simd_vector<T, S> simd_vector;
		typedef typename simd_vector::iterator iterator;
		typedef typename simd_vector::simd_iterator simd_iterator;
		const int K = sizeof(S) / sizeof(T);

		simd_vector vec(K * 8);
		iterator b = vec.begin();
		iterator e = vec.end();
		assert(b < e);
		assert((b + vec.size()) == e);

		assert(b.lower_offset() == 0);
		assert(b.upper_offset() == -K + 1);

		for (iterator i = b; i < e; ++i)
		{
			assert(0 <= i.lower_offset() && i.lower_offset() < K);
			assert(-K < i.upper_offset() && i.upper_offset() <= 0);
		}

		assert(&*b == (T*)&*b.lower_block() + b.lower_offset());
		assert(&*e == (T*)&*e.upper_block() + e.upper_offset());

		for (iterator i = b; i < e; ++i)
		{
			assert(&*i == (T*)&*i.lower_block() + i.lower_offset());
			//assert(&*i == (T*)&*i.upper_block() + i.upper_offset());

			auto of = i.upper_offset();
			auto a = &*i.upper_block() - of;
			auto b = (&*i.upper_block()) - of;
			auto c = &*(i.upper_block() - of);
			//assert(a == b == c);

			auto u = (T*)&*i.upper_block() - of;
			auto v = (T*)(&*i.upper_block()) - of;
			auto w = (T*)(&*i.upper_block() - of);

			assert((T *)c == w);

			/*simd_iterator it = i.upper_block() + i.upper_offset();
			S *ptr_s = &*i.upper_block() + i.upper_offset();
			T *ptr_t = (T*)&*i.upper_block() + i.upper_offset();
			T *ptr_t2 = ((T*)&*i.upper_block()) + i.upper_offset();*/
			auto x = (T*)(&*i.upper_block() + i.upper_offset());
			T *y = &*i;
			
			int jjdfasdf = 0;
			//assert(*i == *((T*)&*i.upper_block() + i.upper_offset()));
		}
	}

	template<typename T, typename S>
	void test_offsets()
	{
		typedef simd_vector<T, S> simd_vector;
		typedef typename simd_vector::iterator iterator;
		typedef typename simd_vector::simd_iterator simd_iterator;
		const int K = sizeof(S) / sizeof(T);

		{
			simd_vector vec(K * 2);

			iterator b = vec.begin();
			assert(b.lower_offset() == 0);
			assert(b.upper_offset() == -K + 1);

			iterator e = vec.end();
			assert(e.lower_offset() == 0);
			// this is a special case, when we are at the end
			assert(e.upper_offset() == 0);
		}

		{
			simd_vector vec(K + 1);

			iterator b = vec.begin();
			assert(b.lower_offset() == 0);
			assert(b.upper_offset() == -K + 1);

			iterator e = vec.end();
			assert(e.lower_offset() == (vec.size() % K));
			// this is a special case, when we are at the end
			assert(e.upper_offset() == 0);
		}

		for (int i = 0; i < K; ++i)
		{
			simd_vector vec(K + i);

			iterator b = vec.begin();
			assert(b.lower_offset() == 0);
			assert(b.upper_offset() == -K + 1);

			iterator e = vec.end();
			assert(e.lower_offset() == (vec.size() % K));
			// this is a special case, when we are at the end
			assert(e.upper_offset() == 0);
		}

		{
			simd_vector vec(K * 3);

			iterator b = vec.begin();
			iterator e = vec.end();
			//cout << "K: " << K << endl;
			int index = 0;
			for (iterator i = b; i < e; ++i)
			{
				//cout << "lower: " << i.lower_offset() << " upper: " << i.upper_offset() << " index % K: " << index % K << endl;
				assert(i.lower_offset() == index % K);
				assert(i.upper_offset() == -K + 1 + index % K);

				++index;
			}
		}
	}

	template<typename T, typename S>
	void test_blocks()
	{
		typedef simd_vector<T, S> simd_vector;
		typedef typename simd_vector::iterator iterator;
		typedef typename simd_vector::simd_iterator simd_iterator;
		const int K = sizeof(S) / sizeof(T);

		{
			simd_vector vec(K * 2);

			iterator b = vec.begin();
			simd_iterator bb = b.lower_block();
			simd_iterator eb = b.upper_block();
			assert(bb < eb);
			assert(bb + 1 == eb);
			assert(bb == eb - 1);

			iterator e = vec.end();
			simd_iterator be = e.lower_block();
			simd_iterator ee = e.upper_block();
			assert(be == ee);	// iterator is not moveing behind the end
		}

		{
			simd_vector vec(K + 1);

			iterator b = vec.begin();
			simd_iterator bb = b.lower_block();
			simd_iterator eb = b.upper_block();
			assert(bb < eb);
			assert(bb + 1 == eb);
			assert(bb == eb - 1);

			iterator e = vec.end();
			simd_iterator be = e.lower_block();
			simd_iterator ee = e.upper_block();
			assert(be == ee);	// iterator is not moveing behind the end
		}

		for (int i = 0; i < K; ++i)
		{
			simd_vector vec(K + i);

			iterator b = vec.begin();
			simd_iterator bb = b.lower_block();
			simd_iterator eb = b.upper_block();
			assert(bb < eb);
			assert(bb + 1 == eb);
			assert(bb == eb - 1);

			iterator e = vec.end();
			simd_iterator be = e.lower_block();
			simd_iterator ee = e.upper_block();
			assert(be == ee);	// iterator is not moveing behind the end
		}

		{
			simd_vector vec(1);

			iterator b = vec.begin();
			simd_iterator bb = b.lower_block();
			simd_iterator eb = b.upper_block();
			assert(bb < eb);
			assert(bb + 1 == eb);
			assert(bb == eb - 1);

			iterator e = vec.end();
			simd_iterator be = e.lower_block();
			simd_iterator ee = e.upper_block();
			assert(be == ee);	// iterator is not moveing behind the end
		}
	}

	template<typename T, typename S>
	void test_iterator()
	{
		typedef simd_vector<T, S> simd_vector;
		typedef typename simd_vector::iterator iterator;
		typedef typename simd_vector::simd_iterator simd_iterator;
		const int K = sizeof(S) / sizeof(T);

		{
			simd_vector vec(K * 4);

			iterator b = vec.begin();
			iterator e = vec.end();
			for (auto it = b + 1; it < e - 1; ++it)
			{
				simd_iterator bi = it.lower_block();
				simd_iterator ei = it.upper_block();
				assert(bi < ei);
				assert(bi + 1 == ei);
				assert(bi == ei - 1);

				auto val = *it;
				auto b_val = *bi;
			}
		}

	}

	void test_simd_vector_iterator_methods()
	{
		test_offsets<short, int>();
		test_offsets<short, long>();
		test_offsets<short, float>();
		test_offsets<short, double>();
		test_offsets<char, short>();
		test_offsets<char, int>();
		test_offsets<char, long>();
		test_offsets<char, float>();
		test_offsets<char, double>();

		test_blocks<short, int>();
		test_blocks<short, long>();
		test_blocks<short, float>();
		test_blocks<short, double>();
		test_blocks<char, short>();
		test_blocks<char, int>();
		test_blocks<char, long>();
		test_blocks<char, float>();
		test_blocks<char, double>();

		test_iterator<short, int>();
		test_iterator<short, long>();
		test_iterator<short, float>();
		test_iterator<short, double>();
		test_iterator<char, short>();
		test_iterator<char, int>();
		test_iterator<char, long>();
		test_iterator<char, float>();
		test_iterator<char, double>();
	}

	void test_complete()
	{
		simd_vector<short, int> vec(4);
		short acc = 1;
		short param = 7;
		std::generate(vec.begin(), vec.end(), [&acc, param](){ return acc += param; });

		test_vector(vec);

		test_simd_vector_iterator_methods();

		test_invariants<char, int>();
	}
};

int main(int argc, char* *argv)
{
	//char ar[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	//simd_vector_iterator<char, int> iter1(ar, 0);
	//auto low = iter1.lower_block();
	//iter1++;
	//auto iter3 = iter1.lower_block();
	//iter1++;
	//auto iter4 = iter1.lower_block();
	//simd_iterator<short> iter((short *)ar, 0);
	//simd_iterator<short> end((short *)ar, 5);
	//int size = sizeof((short *)ar);
	//while (iter < end)
	//{
	//	cout << *iter << endl;
	//	++iter;
	//}
	//--iter;
	//iter++;
	//iter--;
	//iter += 1;
	//iter -= 1;
	//simd_vector<char, int> xvec(24);
	//simd_vector<char, int> vec = std::move(xvec);
	//simd_vector<char, int> vec2 = simd_vector<char, int>(std::move(xvec));
	//vec = std::move(vec2);
	//
	//std::vector<int> v;
	//size_t len = 10;
	//int *p = new int[len];
	//auto result = std::align(3, sizeof(short) * 5, (void *&)p, len);

	size_t size = 8;
	simd_vector<char, short> vec(size);
	char i = 0;

	for (auto &it : vec)
		it = i++ % 2 == 0 ? 1 : 0;

	for (auto b = vec.begin(); b < vec.end(); ++b)
	{
		cout << (short)(*b) << endl;
	}
	cout << "finished" << endl;

	auto bb = vec.begin().lower_block();
	auto ee = vec.end().upper_block();
	for (; bb < ee; ++bb)
	{
		cout << *bb << endl;
	}
	
	//du1example::test();
	unittest::test_complete();

	system("pause");
	return 0;
}

