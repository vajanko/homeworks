#ifndef _du3bsearch_hdr
#define _du3bsearch_hdr

#include <iostream>
#include <vector>
#include <algorithm>
#include <xmmintrin.h>

typedef unsigned long data_element;

#undef BIN_SEARCH
#undef SEQ_SEARCH
#undef HEAP_SEARCH
#undef PBIN_SEARCH
#undef BST_SEARCH
#undef VEB_SEARCH
#define VEB_SEARCH

#ifdef BIN_SEARCH
class bsearch_inner {
public:
	data_element* data;
	std::size_t isize;

	std::size_t find(const data_element el) const
	{
		std::size_t l = 0;
		std::size_t r = isize;
		std::size_t i = l + (r - l) / 2;

		while (data[i] != el && l + 1 < r)
		{
			if (data[i] > el)
			{
				r = i;
			}
			else
			{
				l = i;
			}
			i = l + (r - l) / 2;
		}

		if (el >= data[i])
			++i;

		return i;
	}

	bsearch_inner(const data_element * idata, std::size_t isize) : data(new data_element[isize]), isize(isize)
	{
		std::copy_n(idata, isize, data);
	}
	/*~bsearch_inner()
	{
	delete[] data;
	}*/

};
#endif
#ifdef SEQ_SEARCH
class bsearch_inner {
public:
	data_element* data;
	std::size_t isize;

	std::size_t find(const data_element el) const
	{
		std::size_t i = 0;

		while (i < isize && data[i] <= el)
		{
			++i;
		}

		return i;
	}

	bsearch_inner(const data_element * idata, std::size_t isize) : data(new data_element[isize]), isize(isize)
	{
		std::copy_n(idata, isize, data);
	}
};
#endif
#ifdef HEAP_SEARCH
class bsearch_inner {
public:
	data_element* data;
	std::size_t isize;

	std::size_t find(const data_element el) const
	{
		std::size_t i = 0;

		while (i < isize && data[i] < el)
		{
			std::size_t l = 2 * i + 1;
			std::size_t r = 2 * i + 2;
			if (r < isize && data[r] <= el)
			{
				i = r;
			}
			else if (l < isize)
			{
				i = l;
			}
			else
			{
				++i;
				//break;
			}
		}

		if (el >= data[i])
			++i;

		return i;
	}

	bsearch_inner(const data_element * idata, std::size_t isize) : data(new data_element[isize]), isize(isize)
	{
		std::copy_n(idata, isize, data);
	}
};
#endif
#ifdef PBIN_SEARCH
class bsearch_inner {
public:
	data_element* data;
	std::size_t isize;

	//std::size_t find(const __m128i el) const
	void find(const data_element *el, std::size_t i[]) const
	{
		/*__m128i l = _mm_setzero_si128();
		__m128i r = _mm_setr_epi32(isize, isize, isize, isize);

		__m128i s = _mm_sub_epi32(r, l);
		__m128i d = _mm_srai_epi32(s, 1);
		__m128i i = _mm_add_epi32(l, d);*/

		//while (&& _mm_cmplt_epi32(_mm_add_epi32(l, 1), r))

		/*std::size_t l = 0;
		std::size_t r = isize;
		std::size_t i = l + (r - l) / 2;*/
		std::size_t l[2] = { 0, 0 };
		std::size_t r[2] = { isize, isize };
		i[0] = isize / 2;
		i[1] = isize / 2;

		while (l[0] + 1 < r[0] && l[1] + 1 < r[1])
		{
			if (data[i[0]] > el[0])
			{
				r[0] = i[0];
			}
			else
			{
				l[0] = i[0];
			}
			i[0] = l[0] + (r[0] - l[0]) / 2;

			if (data[i[1]] > el[1])
			{
				r[1] = i[1];
			}
			else
			{
				l[1] = i[1];
			}
			i[1] = l[1] + (r[1] - l[1]) / 2;
		}

		if (el[0] >= data[i[0]])
			++i[0];
		if (el[1] >= data[i[1]])
			++i[1];
	}

	bsearch_inner(const data_element * idata, std::size_t isize) : data(new data_element[isize]), isize(isize)
	{
		std::copy_n(idata, isize, data);
	}
	/*~bsearch_inner()
	{
	delete[] data;
	}*/

};
#endif
#ifdef BST_SEARCH
class bsearch_inner {
public:
	data_element* data;
	std::size_t isize;
	std::size_t tsize;

	void init() const
	{
		//for (std::size_t i = isize; i < tsize; ++i)
		//{
		//	//data[i] <- clear
		//}
	}
	std::size_t find(const data_element el) const
	{
		std::size_t i = 0;	// root
		std::size_t l;
		std::size_t r;

		while (i < tsize)
		{
			l = i * 2 + 1;

			if (l < tsize && data[i] > el)
			{
				i = l;
			}
			else if (l + 1 < tsize)
			{
				r = l + 1;
				i = r;
			}
			else
			{
				return data[i];
			}
		}

		return i;
	}

	void build_bst(const data_element *idata, std::size_t isize, data_element *data) const
	{
		data_element leaf_idx = 0;
		std::vector<data_element> wdata(idata, idata + isize);

		std::size_t m = 0;
		while ((2 << m) - 1 <= isize)
			++m;

		// now holds: isize < 2^m -1 and "m" is minimal
		--m;
		std::size_t leafs = isize - (2 << m) + 1;

		std::size_t step = 2;
		long pos = isize - 1;

		// remove the leafs
		for (long i = leafs * step - 2; i >= 0; i -= step)
		{
			data[pos] = wdata.at(i);
			--pos;
		}
		for (long i = leafs * step - 2; i >= 0; i -= step)
		{
			wdata.erase(wdata.begin() + i);
		}

		// now wdata has size 2^m -1
		while (pos >= 0)
		{
			for (long i = wdata.size() - 1; i >= 0; i -= step)
			{
				data[pos] = wdata.at(i);
				--pos;
			}
			for (long i = wdata.size() - 1; i >= 0; i -= step)
			{
				wdata.erase(wdata.begin() + i);
			}
		}

		// init leafs
		for (std::size_t i = isize; i < tsize - leafs - 1; ++i)
			data[i] = i - isize + leafs + 1;

		for (std::size_t i = 0; i < leafs + 1; ++i)
			data[tsize - leafs - 1 + i] = i;
	}
	bsearch_inner(const data_element * idata, std::size_t isize) : isize(isize), tsize(isize * 2 + 1), data(new data_element[isize * 2 + 1])
	{
		build_bst(idata, isize, data);

		std::size_t m = 0;
		while ((2 << m) < isize)
			++m;

		/*for (std::size_t i = 0; i < tsize; ++i)
			std::cout << data[i] << " ";
		std::cout << std::endl;*/
	}
	~bsearch_inner()
	{
		//delete[] data;
	}
};
#endif
#ifdef VEB_SEARCH
class bsearch_inner {
public:
	data_element* data;
	std::size_t isize;
	std::size_t tsize;
	std::size_t rsize;
	data_element* rdata;
	std::size_t height;

	std::size_t *succ;

	std::size_t find(const data_element el) const
	{
		if (rsize > 0 && rdata[0] <= el)
		{
			std::size_t res = isize + 1;
			for (std::size_t j = 1; j < rsize - 1; ++j)
			{
				if (rdata[j] < el)
					return res;
				
				++res;
			}

			return res;
		}

		std::size_t i = 0;	// root
		std::size_t l;
		std::size_t r;

		for (std::size_t j = 0; j < height - 1; ++j)
		{
			l = succ[i * 2];
			r = succ[i * 2 + 1];

			if (l < tsize && data[i] > el)
			{
				i = l;
			}
			else if (r < tsize)
			{
				i = r;
			}
			/*else
			{
				return data[i];
			}*/
		}

		return data[i];
	}

	void build_bst(const data_element *idata, std::size_t isize, data_element *data) const
	{
		data_element leaf_idx = 0;
		std::vector<data_element> wdata(idata, idata + isize);

		std::size_t m = 0;
		while ((2 << m) - 1 <= isize)
			++m;

		// now holds: isize < 2^m -1 and "m" is minimal
		--m;
		std::size_t leafs = isize - (2 << m) + 1;

		std::size_t step = 2;
		long pos = isize - 1;

		// remove the leafs
		for (long i = leafs * step - 2; i >= 0; i -= step)
		{
			data[pos] = wdata.at(i);
			--pos;
		}
		for (long i = leafs * step - 2; i >= 0; i -= step)
		{
			wdata.erase(wdata.begin() + i);
		}

		// now wdata has size 2^m -1
		while (pos >= 0)
		{
			for (long i = wdata.size() - 1; i >= 0; i -= step)
			{
				data[pos] = wdata.at(i);
				--pos;
			}
			for (long i = wdata.size() - 1; i >= 0; i -= step)
			{
				wdata.erase(wdata.begin() + i);
			}
		}

		// init leafs
		for (std::size_t i = isize; i < tsize; ++i)
			data[i] = i - isize;
		/*for (std::size_t i = isize; i < tsize - leafs - 1; ++i)
			data[i] = i - isize + leafs + 1;

		for (std::size_t i = 0; i < leafs + 1; ++i)
			data[tsize - leafs - 1 + i] = i;*/
	}
	void build_veb(std::size_t &i_pos, data_element *odata, std::size_t o_pos, std::size_t height) const
	{
		if (height == 1)
		{
			odata[o_pos] = data[i_pos];
			i_pos = i_pos * 2 + 1;
		}
		else
		{
			std::size_t r_height = height / 2;
			std::size_t ch_height = height / 2 + height % 2;

			std::size_t r_nodes = std::pow(2, r_height) - 1;
			std::size_t ch_pos = i_pos;
			build_veb(ch_pos, odata, o_pos, r_height);

			// points to the first child tree
			i_pos = ch_pos;
			//i_pos += r_nodes;
			o_pos += r_nodes;

			// number of nodes in the child tree
			std::size_t ch_nodes = std::pow(2, ch_height) - 1;
			std::size_t ch_trees = r_height * 2;

			for (std::size_t i = 0; i < ch_trees; ++i)
			{
				//std::size_t ch_pos = (i % 2 == 0) ? i_pos * 2 + 1 : i_pos * 2 + 2;
				ch_pos = i_pos;
				build_veb(ch_pos, odata, o_pos, ch_height);
				//i_pos += (i % 2 == 0) ? 1 : ch_nodes;
				i_pos += 1;
				o_pos += ch_nodes;
			}
		}
	}
	bsearch_inner(const data_element * idata, std::size_t isize) : isize(isize), tsize(isize * 2 + 1), data(new data_element[(isize * 2 + 1) * 2]), 
		succ(new std::size_t[isize * 2 + 1])
	{
		std::size_t m = 0;
		while ((2 << m) - 1 <= isize)
			++m;

		rsize = isize - (std::pow(2, m) - 1);
		this->isize -= rsize;
		isize -= rsize;

		rdata = new data_element[rsize];
		std::copy(idata + isize, idata + isize + rsize, rdata);

		build_bst(idata, isize - rsize, data);

		/*for (std::size_t i = 0; i < tsize; ++i)
			std::cout << data[i] << " ";
		std::cout << std::endl;*/

		height = std::ceil(std::log2(isize)) + 1;
		std::size_t i_pos = 0;
		data_element *tmp = new data_element[tsize];
		build_veb(i_pos, tmp, 0, height);

		for (std::size_t i = 0; i < (isize * 2 + 1) * 2; ++i)
			succ[i] = 0;

		std::size_t j;
		for (std::size_t i = 0; i < isize; ++i)
		{
			std::size_t k = std::distance(tmp, std::find(tmp, tmp + tsize, data[i]));

			if (2 * i + 1 < tsize)
			{
				data_element l = data[2 * i + 1];
				for (j = i + 1; j < tsize; ++j)
				{
					if (tmp[j] == l)
					{
						succ[k * 2] = j;
						break;
					}
				}
			}

			if (2 * i + 2 < tsize)
			{
				data_element r = data[2 * i + 2];
				for (j = j + 1; j < tsize; ++j)
				{
					if (tmp[j] == r)
					{
						succ[k * 2 + 1] = j;
						break;
					}
				}
			}
		}


		delete[] data;
		data = tmp;

		/*for (std::size_t i = 0; i < tsize; ++i)
			std::cout << data[i] << " ";
		std::cout << std::endl;
		for (std::size_t i = 0; i < tsize; ++i)
			std::cout << succ[2 * i] << " ";
		std::cout << std::endl;
		for (std::size_t i = 0; i < tsize; ++i)
			std::cout << succ[2 * i + 1] << " ";*/
	}
	~bsearch_inner()
	{
		//delete[] data;
	}
};
#endif

class bsearch_outer {
private:
	const bsearch_inner inner;
	std::size_t osize;

	data_element *buckets;
	std::size_t *bucket_index;

	std::size_t *bucket_size;
	std::size_t *bucket_start;

public:
	bsearch_outer(const bsearch_inner & inner, std::size_t osize) : inner(inner), osize(osize),
		buckets(new data_element[osize]), bucket_index(new std::size_t[osize]),
		bucket_size(new std::size_t[inner.isize + inner.rsize + 1]), 
		bucket_start(new std::size_t[inner.isize + inner.rsize + 1])
	{
	}
	~bsearch_outer()
	{
		delete[] buckets;
		delete[] bucket_size;
	}

	void bucketize(const data_element * odata)
	{
		for (std::size_t i = 0; i < inner.isize + inner.rsize + 1; ++i)
			bucket_size[i] = 0;

		std::size_t idx;

		for (std::size_t i = 0; i < osize; ++i)
		{
			idx = inner.find(odata[i]);
			bucket_index[i] = idx;
			++bucket_size[idx];

			/*std::size_t bi = bucket_size[idx];
			buckets[idx * osize + bi] = odata[i];
			bucket_size[idx] = bi + 1;*/
		}

		std::size_t acc = 0;
		for (std::size_t i = 0; i <= inner.isize + inner.rsize; ++i)
		{
			bucket_start[i] = acc;
			acc += bucket_size[i];
		}

		for (std::size_t i = 0; i < osize; ++i)
		{
			std::size_t idx = bucket_index[i];
			buckets[bucket_start[idx]] = odata[i];
			++bucket_start[idx];
		}

		/*for (std::size_t i = 0; i < osize / 4; ++i)
		{
			__m128i cur = *((__m128i*)odata + i);

			for (int j = 0; j < 4; ++j)
			{
				data_element el = cur.m128i_u32[j];
				idx = inner.find(el);

				std::size_t bi = buckets_size[idx];
				buckets[idx * osize + bi] = el;

				buckets_size[idx] = bi + 1;
			}
			
		}*/
	}

	typedef std::pair< const data_element *, std::size_t> bucket_rv;

	bucket_rv bucket(std::size_t k) const 
	{
		//return bucket_rv(buckets + k * osize, bucket_size[k]);
		return bucket_rv(buckets + bucket_start[k] - bucket_size[k], bucket_size[k]);
	}
};

#endif