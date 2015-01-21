#ifndef du4levenstein_hpp_
#define du4levenstein_hpp_

#include <iostream>

template<typename T> T smaller(T x, T y) { return (x<y) ? x : y; }
template<typename T> T smaller(T x, T y, T z) { return smaller<T>(smaller<T>(x, y), z); }

class levenstein 
{
public:
	typedef int data_element;

private:
	data_element *str1;
	std::size_t size1;

	data_element *str2;
	std::size_t size2;

public:
	template< typename I1, typename I2>
	levenstein(I1 i1b, I1 i1e, I2 i2b, I2 i2e) :
		str1(new data_element[std::distance(i1b, i1e)]), size1(std::distance(i1b, i1e)),
		str2(new data_element[std::distance(i2b, i2e)]), size2(std::distance(i2b, i2e))
	{
		std::copy(i1b, i1e, str1);
		std::copy(i2b, i2e, str2);
	}

	data_element compute_default()
	{
		// degenerated cases
		if (size1 == 0) return size2;
		if (size2 == 0) return size1;

		// create two work vectors of integer distances
		size_t vec_size = size2 + 1;

		size_t *dst1 = new size_t[vec_size];
		size_t *dst2 = new size_t[vec_size];

		for (int i = 0; i <= size2; ++i)
			dst2[i] = i;

		for (int i = 1; i <= size1; ++i)
		{
			dst1[0] = i;

			// use formula to fill in the rest of the row
			for (int j = 1; j <= size2; ++j)
			{
				size_t upper = dst2[j];
				size_t left = dst1[j - 1];
				size_t upperleft = dst2[j - 1];
				char cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;

				size_t total = smaller<size_t>(upper + 1, left + 1, upperleft + cost);
				dst1[j] = total;
			}

			std::swap(dst1, dst2);
		}

		return dst2[size2];
	}

	data_element compute()
	{
		return compute_default();
	}

};

#endif
