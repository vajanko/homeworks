#include <iostream>

using namespace std;

template<typename T> T smaller(T x, T y) { return (x<y) ? x : y; }
template<typename T> T smaller(T x, T y, T z) { return smaller<T>(smaller<T>(x, y), z); }

size_t levenshtein_serial(const char *s, size_t s_size, const char *t, size_t t_size)
{
	// degenerated cases
	if (s_size == 0) return t_size;
	if (t_size == 0) return s_size;

	// create two work vectors of integer distances
	size_t vec_size = t_size + 1;

	size_t *dst1 = new size_t[vec_size];
	size_t *dst2 = new size_t[vec_size];

	for (int i = 0; i <= t_size; ++i)
		dst2[i] = i;

	for (int i = 1; i <= s_size; ++i)
	{
		dst1[0] = i;

		// use formula to fill in the rest of the row
		for (int j = 1; j <= t_size; ++j)
		{
			size_t upper = dst2[j];
			size_t left = dst1[j - 1];
			size_t upperleft = dst2[j - 1];
			char cost = (s[i - 1] == t[j - 1]) ? 0 : 1;

			size_t total = smaller<size_t>(upper + 1, left + 1, upperleft + cost);
			dst1[j] = total;
		}

		std::swap(dst1, dst2);
	}

	return dst2[t_size];
}

//int main(int argc, char **argv)
//{
//
//
//	return 0;
//}