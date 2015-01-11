#ifndef _du3bsearch_hdr
#define _du3bsearch_hdr

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <xmmintrin.h>
#include <stdlib.h>
#include <queue>

typedef unsigned long data_element;

template<typename T> T smaller(T x, T y) { return (x<y) ? x : y; }
template<typename T> T greater(T x, T y) { return (x>y) ? x : y; }
template<typename T> T binary_tree_size(T height) { return (1 << height) - 1; }

#undef SEQ_SEARCH
#undef HEAP_SEARCH
#undef PBIN_SEARCH
#undef BST_SEARCH
#undef VEB_SEARCH

class binary_search {
public:
	std::unique_ptr<data_element[]> data;
	std::size_t isize;
	std::size_t height;
public:
    std::size_t get_size() const { return isize; }
    std::size_t find(const data_element el) const
    {
#if _DEBUG
        data_element *data = this->data.get();
#endif
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

	void find_many(const data_element *els, const std::size_t count, std::size_t *results) const
	{
		std::size_t *i = new std::size_t[count];
		std::size_t size = isize >> 1;
		std::fill(i, i + count, size);

		for (std::size_t j = 0; j < height; ++j)
		{
			size = size >> 1;

			for (std::size_t k = 0; k < count; ++k)
			{
				if (data[i[k]] > els[k])
					i[k] = i[k] - size;
				else
					i[k] = i[k] + size;
			}
		}

		for (std::size_t k = 0; k < count; ++k)
			results[k] = els[k] < data[i[k]] ? i[k] : i[k] + 1;

		/*std::size_t *l = new std::size_t[count];
		std::size_t *r = new std::size_t[count];
		std::size_t i;
		
		std::fill(l, l + count, 0);
		std::fill(r, r + count, isize);

		for (std::size_t j = 0; j < height; ++j)
		{
			for (std::size_t k = 0; k < count; ++k)
			{
				i = (l[k] + r[k]) >> 1;
				if (data[i] > els[k])
					r[k] = i;
				else
					l[k] = i;
			}
		}

		for (std::size_t k = 0; k < count; ++k)
			results[k] = els[k] < data[l[k]] ? l[k] : l[k] + 1;*/
	}

    binary_search(const data_element * idata, std::size_t isize) : data(isize == 0 ? nullptr : new data_element[isize]), isize(isize),
		height(std::log2(isize))
    {
		if (data.get() != nullptr)
			std::copy_n(idata, isize, data.get());
    }
	binary_search(binary_search &&val) : data(std::move(val.data)), isize(val.isize){ }
	binary_search &operator=(binary_search &&val)
	{
		data = std::move(val.data);
		isize = val.isize;

		return *this;
	}
};

class sequence_search {
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

	sequence_search(const data_element * idata, std::size_t isize) : data(new data_element[isize]), isize(isize)
	{
		std::copy_n(idata, isize, data);
	}
};

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

class block_search {
private:
	const std::size_t CACHE_ELEM_SIZE = 1024 * 64;

	std::unique_ptr<data_element[]> data;
	std::size_t data_size;

	std::unique_ptr<data_element[]> index;
	std::size_t index_size;

	std::size_t block_size;
public:
	std::size_t get_size() const { return data_size; }

	std::size_t find(const data_element el) const
	{
		// search in the index
		std::size_t l = 0;
		std::size_t r = index_size;

		while (l + 1 < r)
		{
			std::size_t i = (l + r) >> 1;		// div by 2
			if (index[i] > el)
				r = i;
			else
				l = i;
		}

		l = l * block_size;
		r = smaller(l + block_size, data_size);

		// search in the indexed part
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
	std::size_t find(const data_element el, std::size_t left , std::size_t right) const
	{
		if (left == right)
			return left;

		// search in the index
		std::size_t l = 0;
		std::size_t r = index_size;

		while (l + 1 < r)
		{
			std::size_t i = (l + r) >> 1;		// div by 2
			if (index[i] > el)
				r = i;
			else
				l = i;
		}

		l = greater(l * block_size, left);
		r = smaller(smaller(l + block_size, data_size), right);

		// search in the indexed part
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

	block_search(const data_element * idata, std::size_t isize) :
		data(new data_element[isize]), data_size(isize)
	{
		// TODO: move items in the array at the beginning
		std::copy_n(idata, isize, data.get());

		block_size = data_size / CACHE_ELEM_SIZE;
		if (block_size == 0)
			block_size = data_size;

		index_size = std::ceil(data_size / block_size) + data_size % block_size;
		index.reset(new data_element[index_size]);

		for (std::size_t i = 0; i < index_size; ++i)
			index[i] = data[i * block_size];
	}
};

class btree_search {
private:
	data_element* data;
	std::size_t data_size;

	std::size_t *missing_elements;	// number of elements missing from right side at i-th level
	std::size_t height;

	std::size_t degree;
	std::size_t degree1;
	
	std::size_t total_leafs;
	std::size_t node_height;
public:
	std::size_t get_size() const { return data_size; }

	std::size_t find(const data_element el) const
	{
		std::size_t l = 0;
		std::size_t r = degree;
		if (r <= data_size && data[r - 1] < data[l])
			r -= missing_elements[0];

		std::size_t left = 0;
		std::size_t res = 0;

        // number of leafs in a b-tree of this->height height
		std::size_t leafs = total_leafs;

		for (std::size_t j = 0; j <= height; ++j)
		{
			left = l;

			// binary search in the node
			for (std::size_t k = 0; k < node_height; ++k)
			{
				std::size_t i = (l + r) >> 1;		// div by 2
				if (data[i] > el)
					r = i;
                else
					l = i;
			}

			if (l + 1 < data_size && data[l + 1] < data[l] && data[l] <= el)
			{
				l = el < data[l] ? l : l + 1;
				res += (l - left) * leafs;
				return res;
			}

			l = el < data[l] ? l : l + 1;

			res += (l - left) * leafs;

			leafs /= degree1;

			// jump to the child node
			//l = left * degree1 + (l - left + 1) * degree;
			l = left + (l + 1) * degree - missing_elements[j];
				//(j == 0 ? 0 : missing_elements[j - 1]);
			r = l + degree;

			if (r <= data_size && data[r - 1] < data[l])
				r -= missing_elements[j + 1];
		}
		
		return res;
	}

	void build_btree(std::size_t out, const data_element *idata, std::size_t isize, std::size_t begin, std::size_t end)
	{
		typedef std::pair<std::size_t, std::size_t> be;

		std::size_t idx = 0;
		std::queue<be> bfs;
		bfs.push(be(begin, end));
		std::size_t level = 0;
		std::size_t part_size = std::ceil((double)(end - begin) / (degree + 1));
		missing_elements[level] = 0;

		while (!bfs.empty())
		{
			auto it = bfs.front();
			bfs.pop();

			// it.first == begin
			// it.second == end
			std::size_t tmp = std::ceil((double)(it.second - it.first) / (degree + 1));
			if (tmp != part_size)
			{
				++level;
				missing_elements[level] = missing_elements[level - 1];
			}
			part_size = tmp;

			if (part_size == 0)
				continue;

			for (std::size_t i = it.first + part_size - 1; i < it.second; i += part_size)
			{
				if (i < isize)
				{
					data[idx] = idata[i];
					++idx;
				}
				else
				{
					++missing_elements[level];
				}
				//std::cout << data[idx] << " ";
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
		std::size_t level = 0;

		while (!bfs.empty())
		{
			for (std::size_t j = 0; j < nodes && !bfs.empty(); ++j)
			{
				auto it = bfs.front() - (level == 0 ? 0 : missing_elements[level - 1]);
				bfs.pop();

				for (std::size_t i = 0; i < degree; ++i)
				{
					/*if (it + i < data_size)
					{
						if (data[it + i] == SIZE_MAX)
							std::cout << "M ";
						else
							std::cout << data[it + i] << " ";
					}*/
					if (it + i < data_size)
						std::cout << data[it + i] << " ";

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
			level++;

			nodes *= (degree + 1);
		}
	}
	btree_search(const data_element * idata, std::size_t isize) :
		data(new data_element[isize]), data_size(isize), height(0), degree(0), degree1(0)
	{
		degree = smaller<std::size_t>(2, isize);
		degree1 = degree + 1;
		node_height = std::ceil(std::log2(degree));

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
		total_leafs = std::pow(degree + 1, height);
		//data_size = m;
		//data = new data_element[data_size];
		/*for (std::size_t i = 0; i < data_size; ++i)
			data[i] = SIZE_MAX;*/

		missing_elements = new std::size_t[height];

		build_btree(0, idata, isize, 0, m);

		for (std::size_t i = 0; i < data_size; ++i)
			std::cout << data[i] << " ";
		std::cout << std::endl;

		print_btree(0, m);

		/*for (std::size_t i = 0; i < data_size; ++i)
			std::cout << data[i] << " ";
		std::cout << std::endl;*/
	}
};

#define WORD sizeof(data_element)*8
std::size_t __inline clz(data_element value)
{
	data_element leading_zero = 0;

	if (_BitScanReverse(&leading_zero, value))
	{
		return WORD - 1 - leading_zero;
	}
	else
	{
		// Same remarks as above
		return WORD;
	}
}
#define fls(x) (WORD-clz(x))

class emde_boas_search
{
	// METADATA
private:
	// height of the main tree
	std::size_t height;
	// total number of nodes in the tree
	std::size_t nodes;

	// size of top tree at particular depth in a subtree
	std::size_t *top_size;
	// size of bottom tree at particular depth in a subtree
	std::size_t *bottom_size;
	// depth in the main tree at particular index in the 
	std::size_t *subtree_depth;
private:
	void veb_init(std::size_t height, std::size_t depth)
	{
		if (height > 1)
		{
			std::size_t bottom_height = height / 2;
			std::size_t top_height = height - bottom_height;

			// depth index (-1) of bottom sub-trees
			std::size_t bottom_depth = depth + top_height - 1;

			// number of nodes in the top tree
			top_size[bottom_depth] = binary_tree_size(top_height);

			// number of nodes in the bottom tree
			bottom_size[bottom_depth] = binary_tree_size(bottom_height);

			subtree_depth[bottom_depth] = depth;

			veb_init(top_height, depth);

			veb_init(bottom_height, depth + top_height);
		}
	}
	std::size_t bfs_to_veb(std::size_t bfs)
	{
		if (bfs <= 0 || bfs > data_size)
			return 0;

		std::size_t h = height;
		std::size_t d = 0;
		
		/*std::size_t k = fls(bfs);
		std::size_t p[fls(k)];
		int i = 0;
		for (int d = --k; d > 0; d = L.D[d]){
			int w = v >> (k - d);
			--d;
			p[i] = L.T[d] + (w&L.T[d])*L.B[d];
			++i;
		}
		for (p[i] = 1; i > 0; --i)
			p[i - 1] += p[i];
		return p[0];*/

		return 0;
	}
private:
	data_element *data;
	std::size_t data_size;


public:
	std::size_t get_size() { return data_size; }

	emde_boas_search(const data_element * idata, std::size_t isize)
	{
		
	}
};

typedef btree_search bsearch_inner;
//typedef binary_search bsearch_inner;
//typedef block_search bsearch_inner;

class bsearch_outer {
private:
	const bsearch_inner &inner;
	std::size_t osize;

	data_element *buckets;
	std::size_t *bucket_index;

	std::size_t *bucket_size;
	std::size_t *bucket_start;

public:
	bsearch_outer(const bsearch_inner & inner, std::size_t osize) : inner(inner), osize(osize),
		buckets(new data_element[osize]), bucket_index(new std::size_t[osize]),
		bucket_size(new std::size_t[inner.get_size() + 1]),
		bucket_start(new std::size_t[inner.get_size() + 1]) { }
	bsearch_outer(const bsearch_outer &val) = delete;
	bsearch_outer& operator=(bsearch_outer &other) = delete;
	~bsearch_outer() 
	{
		delete[] buckets; 
		delete[] bucket_index;
		delete[] bucket_size;
		delete[] bucket_start;
	}

	void bucketize(const data_element * odata)
	{
		std::size_t bucket_count = inner.get_size() + 1;

		// clear bucket_size array
		std::fill(bucket_size, bucket_size + bucket_count, 0);

		/*inner.find_many(odata, osize, bucket_index);
		for (std::size_t i = 0; i < osize; ++i)
		{
			++bucket_size[bucket_index[i]];
		}*/

		/*const std::size_t step = 1;
		for (std::size_t i = 0; i < osize; i += step)
		{
			inner.find_many(odata + i, step, bucket_index + i);
			for (std::size_t k = 0; k < step; ++k)
			{
				++bucket_size[bucket_index[i + k]];
			}
		}*/

		/*std::size_t isize = inner.get_size();
		std::size_t l = 0;
		std::size_t r = isize;
		std::size_t idx = inner.find(odata[0], l, r);
		bucket_index[0] = idx;
		++bucket_size[idx];

		for (std::size_t i = 1; i < osize; ++i)
		{
			if (odata[i - 1] > odata[i])
				r = idx;
			else
				r = isize;
			if (odata[i - 1] < odata[i])
				l = idx;
			else
				l = 0;

			idx = inner.find(odata[i], l, r);
			bucket_index[i] = idx;
			++bucket_size[idx];
		}*/

		for (std::size_t i = 0; i < osize; ++i)
		{
			std::size_t idx = inner.find(odata[i]);
			bucket_index[i] = idx;
			++bucket_size[idx];
		}

		std::size_t acc = 0;
		for (std::size_t i = 0; i < bucket_count; ++i)
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
		return bucket_rv(buckets + bucket_start[k] - bucket_size[k], bucket_size[k]);
	}
};

#endif