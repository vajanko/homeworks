#ifndef _du3bsearch_hdr
#define _du3bsearch_hdr

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <xmmintrin.h>
#include <queue>

typedef unsigned long data_element;

template<typename T> T smaller(T x, T y) { return (x<y) ? x : y; }

#undef BIN_SEARCH
#undef SEQ_SEARCH
#undef HEAP_SEARCH
#undef PBIN_SEARCH
#undef BST_SEARCH
#undef VEB_SEARCH
#undef BLK_SEARCH

class binary_search {
private:
	std::unique_ptr<data_element[]> data;
	std::size_t isize;
public:
    std::size_t get_size() const { return isize; }
    std::size_t find(const data_element el) const
    {
        data_element *data = this->data.get();
        std::size_t l = 0;
        std::size_t r = isize;
        std::size_t i = 0;

        while (l + 1 < r)
        {
            i = (l + r) / 2;
            if (data[i] > el)
                r = i;
            else
                l = i;
        }

        return el < data[l] ? l : l + 1;
    }

    binary_search(const data_element * idata, std::size_t isize) : data(new data_element[isize]), isize(isize)
    {
        std::copy_n(idata, isize, data.get());
    }
};

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

class vEBTraverser {
public:
	uint64_t root() {
		d = 0;
		cix = 1;
		return 1;
	}
	uint64_t left() {
		d++;
		cix <<= 1;
		return vEBIndex();
	}
	uint64_t right() {
		d++;
		cix = (cix << 1) + 1;
		return vEBIndex();
	}
private:
	uint64_t cix;
	uint64_t d;

	uint64_t vEBIndex() {
		// Start with largest sub-tree, work down to smallest.
		uint64_t ix = 1;
		uint32_t new_d = d;
		for (char b = 4; b >= 0; --b) {
			const uint64_t b_val = 1L << b;
			if (d & b_val) {
				// Determine sub triangle and add start offset to index.
				const uint64_t masked_d = d & (b_val - 1);
				const uint64_t new_node_size = (1L << b_val) - 1;
				uint64_t subtri_ix = (cix >> masked_d) & new_node_size;
				ix += new_node_size * (1L + subtri_ix);
			}
		}
		return ix;
	}
};

class bsearch_inner {
public:
	data_element* data;
	std::size_t isize;
	std::size_t tsize;
	std::size_t rsize;
	data_element* rdata;
	std::size_t height;

	std::size_t find(const data_element el) const
	{
		/*vEBTraverser tr;
		std::size_t i = tr.root();
		while (true)
		{
			if (data[i] > el)
				i = tr.right();
			else
				i = tr.left();
		}*/

		std::size_t left = 0;
		std::size_t right = 0;
		std::size_t h = height;		// currently searched tree depth
		std::size_t d = 0;			// search depth

		while (right - left > 256)	// ??
		{
			std::size_t r_height = h / 2;
			std::size_t ch_height = r_height + h % 2;

			// TODO: this can be done more efficiently
			std::size_t r_nodes = std::pow(2, r_height) - 1;

			++d;
		}

		// regular binary search in small blocks
		while (left + 1 < right)
		{
			std::size_t i = (left + right) >> 1;		// div by 2
			if (data[i] > el)
				right = i;
			else
				left = i;
		}

		return left;
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
				ch_pos = i_pos;
				build_veb(ch_pos, odata, o_pos, ch_height);
				i_pos += 1;
				o_pos += ch_nodes;
			}
		}
	}
	bsearch_inner(const data_element * idata, std::size_t isize) : isize(isize), tsize(isize * 2 + 1), data(new data_element[(isize * 2 + 1) * 2])
	{
		/*std::size_t m = 0;
		while ((2 << m) - 1 <= isize)
			++m;

		rsize = isize - (std::pow(2, m) - 1);
		this->isize -= rsize;
		isize -= rsize;

		rdata = new data_element[rsize];
		std::copy(idata + isize, idata + isize + rsize, rdata);*/

		build_bst(idata, isize, data);

		/*for (std::size_t i = 0; i < tsize; ++i)
			std::cout << data[i] << " ";
		std::cout << std::endl;*/

		height = std::ceil(std::log2(isize)) + 1;
		std::size_t i_pos = 0;
		data_element *tmp = new data_element[tsize];

		build_veb(i_pos, tmp, 0, height);

		/*for (std::size_t i = 0; i < (isize * 2 + 1) * 2; ++i)
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
		}*/


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
#ifdef BLK_SEARCH

class bin_search
{
private:
	data_element *data;
	std::size_t size;
public:
	std::size_t find(data_element el) const
	{
		std::size_t l = 0;
		std::size_t r = size;

		while (l + 1 < r)
		{
			std::size_t i = (l + r) >> 1;		// div by 2
			if (data[i] > el)
				r = i;
			else
				l = i;
		}

		return l;
	}
	bin_search(data_element *data, std::size_t size) : data(data), size(size) { }
};

class bsearch_inner {
private:
	data_element *data;
	std::size_t data_size;

	std::size_t block_size;

	bin_search index1;
	bin_search index2;
public:
	std::size_t get_size() const { return data_size; }

	std::size_t find(const data_element el) const
	{
		std::size_t idx = index1.find(el);
		std::size_t l = idx * block_size;
		std::size_t r = smaller(l + block_size, data_size);

		while (l + 1 < r)
		{
			std::size_t i = (l + r) >> 1;		// div by 2
			if (data[i] > el)
				r = i;
			else
				l = i;
		}
		
		return el < data[0] ? l : l + 1;
	}

	bsearch_inner(const data_element * idata, std::size_t isize) :
		data(new data_element[isize]), data_size(isize), 
		index1(nullptr, 0), index2(nullptr, 0)
	{
		const std::size_t CACHE_ELEM_SIZE = 1024 * 64;

		std::copy_n(idata, isize, data);

		block_size = data_size / CACHE_ELEM_SIZE;
		if (block_size == 0)
			block_size = data_size;

		std::size_t index_size = std::ceil(data_size / block_size);
		data_element *index_data = new data_element[index_size];

		for (std::size_t i = 0; i < index_size; ++i)
			index_data[i] = data[i * block_size];

		index1 = bin_search(index_data, index_size);
	}
};
#endif

class btree_search {
private:
	std::unique_ptr<data_element[]> data;
	std::size_t data_size;

	std::size_t degree;
	std::size_t degree1;
	std::size_t height;
	std::size_t total_leafs;
public:
	std::size_t get_size() const { return data_size; }

	std::size_t find(const data_element el) const
	{
		std::size_t l = 0;
		std::size_t r = degree;
		std::size_t i;
		std::size_t res = 0;

        // number of leafs in a b-tree of this->height height
		std::size_t leafs = total_leafs;
			//std::pow(degree + 1, height);

		for (std::size_t j = 0; j <= height; ++j)
		{
			std::size_t left = l;

			// binary search in the node
			for (std::size_t k = 0; k < degree; ++k)
			{
				i = (l + r) >> 1;		// div by 2
				if (data[i] > el)
					r = i;
                else
					l = i;
			}

			l = el < data[l] ? l : l + 1;

			res += (l - left) * leafs;

            leafs /= (degree + 1);

			// jump to the child node
			l = left * (degree + 1) + (l - left + 1) * degree;
			r = l + degree;
		}
		
		return res;
	}

	void build_btree(std::size_t out, const data_element *idata, std::size_t isize, std::size_t begin, std::size_t end)
	{
		typedef std::pair<std::size_t, std::size_t> be;

		std::size_t idx = 0;
		std::queue<be> bfs;
		bfs.push(be(begin, end));

		while (!bfs.empty())
		{
			auto it = bfs.front();
			bfs.pop();

			// it.first == begin
			// it.second == end
			std::size_t part_size = std::ceil((double)(it.second - it.first) / (degree + 1));

			if (part_size == 0)
				continue;

			for (std::size_t i = it.first + part_size - 1; i < it.second; i += part_size)
			{
				if (i < isize)
					data[idx] = idata[i];
				//std::cout << data[idx] << " ";
				++idx;
			}

			if (part_size <= 1)
				continue;

			for (std::size_t i = it.first; i < it.second; i += part_size)
			{
				bfs.push(be(i, i + part_size - 1));
			}
		}


		//// block position (not element position)
		//std::size_t part_size = std::ceil((double)(end - begin) / (degree + 1));
		//std::size_t idx = out;
		//
		//for (std::size_t i = begin + part_size - 1; i < end; i += part_size)
		//{
		//	data[idx] = idata[i];
		//	std::cout << data[idx] << " ";
		//	++idx;
		//}

		//if (part_size == 1)
		//	return;

		//idx = out * degree + degree;
		//for (std::size_t i = begin; i < end; i += part_size)
		//{
		//	build_btree(idx, idata, i, i + part_size - 1);
		//	idx += degree;
		//	//out += (part_size - 1);
		//}
	}
	void print_btree(std::size_t begin, std::size_t end)
	{
		std::size_t idx = 0;
		std::queue<std::size_t> bfs;
		bfs.push(0);

		std::size_t nodes = 1;

		while (!bfs.empty())
		{
			for (std::size_t j = 0; j < nodes && !bfs.empty(); ++j)
			{
				auto it = bfs.front();
				bfs.pop();

				for (std::size_t i = 0; i < degree; ++i)
				{
					if (it + i < data_size)
					{
						if (data[it + i] == SIZE_MAX)
							std::cout << "M ";
						else
							std::cout << data[it + i] << " ";
					}
					std::size_t ch = it * (degree + 1) + (i + 1) * degree;
					if (ch < data_size)
						bfs.push(ch);
				}
				std::size_t ch = it * (degree + 1) + (degree + 1) * degree;
				if (ch < data_size)
					bfs.push(ch);
				std::cout << "| ";
			}

			std::cout << std::endl;

			nodes *= (degree + 1);
		}
	}
	btree_search(const data_element * idata, std::size_t isize) :
		/*data(new data_element[isize])*/data(nullptr), data_size(isize), height(0), degree(2)
	{
		std::size_t layer = degree;
		std::size_t m = degree;
		std::size_t h = 0;
		while (m < isize)
		{
			layer *= (degree + 1);
			m += layer;
			++h;
		}

		height = h;
		data_size = m;
		data.reset(new data_element[data_size]);
		for (std::size_t i = 0; i < data_size; ++i)
			data[i] = SIZE_MAX;
		degree1 = degree + 1;
		total_leafs = std::pow(degree + 1, height);

		build_btree(0, idata, isize, 0, m);

		//print_btree(0, m);

		/*for (std::size_t i = 0; i < data_size; ++i)
			std::cout << data[i] << " ";
		std::cout << std::endl;*/
	}
};

typedef btree_search bsearch_inner;
//typedef binary_search bsearch_inner;

class bsearch_outer {
private:
	const bsearch_inner &inner;
	std::size_t osize;

	std::unique_ptr<data_element> buckets;
	std::unique_ptr<std::size_t> bucket_index;

	std::unique_ptr<std::size_t> bucket_size;
	std::unique_ptr<std::size_t> bucket_start;

public:
	bsearch_outer(const bsearch_inner & inner, std::size_t osize) : inner(inner), osize(osize),
		buckets(new data_element[osize]), bucket_index(new std::size_t[osize]),
		bucket_size(new std::size_t[inner.get_size() + 1]), 
		bucket_start(new std::size_t[inner.get_size() + 1]) { }

	void bucketize(const data_element * odata)
	{
		std::size_t *bucket_index = this->bucket_index.get();
		data_element *buckets = this->buckets.get();
		std::size_t *bucket_size = this->bucket_size.get();
		std::size_t *bucket_start = this->bucket_start.get();

		// clear bucket_size array
		/*std::size_t bucket_count = (inner.get_size() + 1) / 4;
		__m128i *b = (__m128i*)bucket_size;
		for (std::size_t i = 0; i < bucket_count; ++i)
			b[i] = _mm_setzero_si128();
		std::size_t last_count = (inner.get_size() + 1) % 4;
		for (std::size_t i = 0; i < last_count; ++i)
			bucket_size[inner.get_size() + i] = 0;*/

		std::size_t bucket_count = inner.get_size() + 1;
		for (std::size_t i = 0; i < bucket_count; ++i)
			bucket_size[i] = 0;

		for (std::size_t i = 0; i < osize; ++i)
		{
			std::size_t idx = inner.find(odata[i]);
			bucket_index[i] = idx;
			++bucket_size[idx];
		}

		std::size_t acc = 0;
		for (std::size_t i = 0; i <= inner.get_size(); ++i)
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
	}

	typedef std::pair< const data_element *, std::size_t> bucket_rv;

	bucket_rv bucket(std::size_t k) const 
	{
		return bucket_rv(buckets.get() + bucket_start.get()[k] - bucket_size.get()[k], bucket_size.get()[k]);
	}
};

#endif