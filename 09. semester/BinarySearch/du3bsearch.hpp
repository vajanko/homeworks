#ifndef _du3bsearch_hdr
#define _du3bsearch_hdr

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <xmmintrin.h>
#include <stdlib.h>
#include <queue>
#include <map>
#include <assert.h>
#include <intrin.h>

typedef unsigned long data_element;

template<typename T> T smaller(T x, T y) { return (x<y) ? x : y; }
template<typename T> T greater(T x, T y) { return (x>y) ? x : y; }
template<typename T> T binary_tree_size(T height) { return (1 << height) - 1; }

class binary_search {
public:
	data_element *data;
	std::size_t data_size;
public:
	std::size_t get_size() const { return data_size; }
    std::size_t find(const data_element el) const
    {
        std::size_t l = 0;
		std::size_t r = data_size;
        std::size_t i = 0;

        while (l + 1 < r)
        {
            i = (l + r) >> 1;
            if (data[i] > el)
                r = i;
            else
                l = i;
        }

        return el < data[l] ? l : l + 1;
    }

    binary_search(const data_element *idata, std::size_t isize) : 
		data(new data_element[isize]), data_size(isize)
    {
		std::copy_n(idata, isize, data);
    }
};

class linear_search 
{
private:
	data_element* data;
	std::size_t data_size;
public:
	std::size_t get_size() const { return data_size; }
	std::size_t find(const data_element el) const
	{
		std::size_t i = 0;

		for (;;) 
		{
			if (data[i] >= el)
				return i;
			++i;
		}
	}

	linear_search(const data_element * idata, std::size_t isize)
		: data(new data_element[isize + 1]), data_size(isize)
	{
		std::copy_n(idata, isize, data);
		data[isize] = ULONG_MAX;
	}
};

class linear_sse_search
{
private:
	data_element* data;
	std::size_t data_size;
public:
	std::size_t get_size() const { return data_size; }
	std::size_t find(const data_element el) const
	{
		if (el < data[0])
			return 0;

		std::size_t i = 0;
		__m128i el4 = _mm_set_epi32(el, el, el, el);
		__m128i *d4 = (__m128i*)data;
		int res;
		
		for (;;)
		{
			__m128i tmp = _mm_cmpgt_epi32(el4, d4[i]);
			res = _mm_movemask_epi8(tmp);
			if (res != 0xffff)
				break;
			++i;
		}

		unsigned long index;
		_BitScanForward(&index, ~res);
		std::size_t r = i * 4 + index / 4;

		
		if (el == data[r])
			++r;
		else if (r > data_size)
			r = data_size;

		return r;
	}

	linear_sse_search(const data_element * idata, std::size_t isize)
		: data(new data_element[isize + 4]), data_size(isize)
	{
		std::copy_n(idata, isize, data);
		data[isize] = ULONG_MAX;
		data[isize + 1] = ULONG_MAX;
		data[isize + 2] = ULONG_MAX;
		data[isize + 3] = ULONG_MAX;
	}
};

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

class index_search
{
private:
	std::size_t CACHE_SIZE[3];
	std::size_t block_size[3];
	std::size_t index_size[3];

	data_element* data;
	std::size_t data_size;

public:
	std::size_t get_size() const { return data_size; }

	std::size_t find(const data_element el) const
	{
		// search in the index
		std::size_t l = 0;
		std::size_t r = index_size[0];

		while (l + 1 < r)
		{
			std::size_t i = (l + r) >> 1;		// div by 2
			if (data[i] > el)
				r = i;
			else
				l = i;
		}

		if (el < data[l])
			return l;

		std::size_t res = l;

		l = index_size[0] + l * (block_size[0] - 1);
		r = smaller(l + block_size[0] - 1, data_size);

		// search in the indexed part
		while (l + 1 < r)
		{
			std::size_t i = (l + r) >> 1;		// div by 2
			if (data[i] > el)
				r = i;
			else
				l = i;
		}

		r = el < data[l] ? 0 : 1;
		l = l - index_size[0] + res + 1;

		return l + r;
	}
	void build_index(data_element *odata, const data_element *idata, std::size_t index_size, std::size_t block_size)
	{
		// copy index at the beginning of the array
		for (std::size_t i = 0; i < index_size; ++i)
			data[i] = idata[i * block_size];
		// copy the rest of the array
		std::size_t di = index_size;
		for (std::size_t i = 0; i < index_size; ++i)
		{
			for (std::size_t j = i * block_size + 1; j < (i + 1) * block_size; ++j)
			{
				data[di] = idata[j];
				++di;
			}
		}
	}

	index_search(const data_element * idata, std::size_t isize) : 
		data(new data_element[isize]), data_size(isize)
	{
		auto x = std::log2(22);

		CACHE_SIZE[0] = 8;
		block_size[0] = data_size / CACHE_SIZE[0];
		if (block_size[0] == 0)
			block_size[0] = data_size;
		index_size[0] = std::ceil(data_size / block_size[0]) + data_size % block_size[0];

		CACHE_SIZE[1] = 4;
		block_size[1] = index_size[0] / CACHE_SIZE[1];
		index_size[1] = index_size[0] / block_size[1];
		
		build_index(data, idata, index_size[0], block_size[0]);

		std::size_t di = index_size[0];
		for (std::size_t i = 0; i < index_size[0]; ++i)
		{
			for (std::size_t j = i * block_size[0] + 1; j < (i + 1) * block_size[0]; ++j)
			{
				data[di] = idata[j];
				++di;
			}
		}

		// debug
		/*for (std::size_t i = 0; i < data_size; ++i)
			std::cout << data[i] << " ";
		std::cout << std::endl;*/
	}
};

class inter_search
{
public:
	data_element* data;
	std::size_t data_size;
public:
	std::size_t get_size() const { return data_size; }

	std::size_t find(const data_element el) const
	{
		std::size_t l = 0;
		std::size_t r = data_size - 1;

		while (data[l] <= el && data[r] >= el)
		{
			std::size_t i = l + ((el - data[l]) * (r - l)) / (data[r] - data[l]);
			if (data[i] < el)
			{
				l = i + 1;
			}
			else if (data[i] > el)
			{
				r = i - 1;
			
			}
			else
			{
				return el < data[i] ? i : i + 1;
			}

			//std::cout << l << " - " << r << std::endl;
		}

		r = r + 1;

		while (l + 1 < r)
		{
			std::size_t i = (l + r) / 2;
			if (data[i] > el)
				r = i;
			else
				l = i;
		}

		return el < data[l] ? l : l + 1;

		/*if (el < data[l])
			return l;
		else if (el > data[r])
			return r + 1;
		else
			return l + 1;*/
	}
	inter_search(const data_element * idata, std::size_t isize) : 
		data(new data_element[isize]), data_size(isize)
	{
		std::copy_n(idata, isize, data);
	}
};

class bucket_search
{
private:
	std::size_t data_size;	// total number of elements

	data_element *buckets;
	std::size_t *gaps;
	std::size_t size;		// size of buckets and gaps arrays

public:
	std::size_t get_size() const { return data_size; }

	std::size_t find(const data_element el) const
	{
		if (el < buckets[0])
			return 0;

		// first find bucket index ...
		std::size_t l = 0;
		std::size_t r = size - 1;
		std::size_t i = 0;

		while (l + 1 < r)
		{
			i = (l + r) >> 1;
			if (buckets[i] > el)
				r = i;
			else
				l = i;
		}

		i = l;
		
		// .. then inside the bucket calculate the index
		std::size_t res = el - gaps[i] + 1;
		std::size_t bm = buckets[i + 1] - gaps[i + 1];
		if (res > bm)
			return bm;

		return res;
	}
	bucket_search(const data_element * idata, std::size_t isize)
		: data_size(isize)
	{
		std::vector<data_element> b;
		std::vector<std::size_t> m;
		std::size_t missing = idata[0];

		b.push_back(idata[0]);
		m.push_back(missing);

		data_element el = 0;

		for (std::size_t i = 1; i < isize; ++i)
		{
			std::size_t dif = idata[i] - idata[i - 1] - 1;
			missing += dif;

			if (dif > 0)
			{
				b.push_back(idata[i]);
				m.push_back(missing);
			}
		}

		b.push_back(idata[isize - 1]);
		m.push_back(missing - 1);

		size = b.size();
		buckets = new data_element[size];
		std::copy_n(b.begin(), b.size(), buckets);

		gaps = new std::size_t[size];
		std::copy_n(m.begin(), m.size(), gaps);
	}
};

struct node
{
	std::size_t value;
	node *left;
	node *right;
};

class interval_search
{
private:
	node *root;
	data_element minimum;
	std::size_t data_size;
	const data_element mask = 1 << (sizeof(data_element) * 8 - 1);
	//const data_element mask = 1 << 4;

	void build_tree(node *n, data_element mask, const data_element *idata, std::size_t isize)
	{
		if (mask == 0)
			return;

		std::size_t i = 0;
		while (i < isize && (idata[i] & mask) == 0)
			++i;

		n->left = new node();
		n->left->value = i;
		n->right = new node();
		n->right->value = 0;

		mask >>= 1;

		if (i > 0)
			build_tree(n->right, mask, idata, i);
		if (isize - i > 0)
			build_tree(n->left, mask, idata + i, isize - i);
	}

public:
	std::size_t get_size() const { return data_size; }

	std::size_t find(const data_element el) const
	{
		if (el < minimum)
			return 0;

		node *n = root;
		std::size_t m = mask;
		std::size_t res = 0;

		while (n != nullptr)
		{
			res += n->value;

			if ((el & m) == 0)
				n = n->right;
			else
				n = n->left;
			
			m >>= 1;
		}

		if (res == data_size)
			return res;
		return res + 1;
	}
	interval_search(const data_element * idata, std::size_t isize)
		: data_size(isize), root(new node()), minimum(idata[0])
	{
		root->value = 0;
		build_tree(root, mask, idata, isize);
	}
};

class hash_search
{
private:
	data_element minimum;
	std::map<data_element, std::size_t> data;

public:
	std::size_t get_size() const { return data.size(); }

	std::size_t find(const data_element el) const
	{
		if (el < minimum)
			return 0;

		auto it = data.upper_bound(el);
		if (it == data.end())
			return data.size();
		else if (it->first == el)
			return it->second;
		else
			return (--it)->second;
	}
	hash_search(const data_element * idata, std::size_t isize)
		: minimum(idata[0])
	{
		for (std::size_t i = 0; i < isize; ++i)
			data.insert(std::make_pair(idata[i], i + 1));
	}
};

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
	data_element *data;
	std::size_t data_size;
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
	std::size_t hyperceil(std::size_t n)
	{
		std::size_t value = 1;
		std::size_t tmp = n;
		while (tmp != 1)
		{
			tmp >>= 1;
			value <<= 1;
		}

		return value == n ? value : (value << 1);
	}
	std::size_t pow2(std::size_t n)
	{
		return 1 << n;
	}
	std::size_t fix_bits(std::size_t x, std::size_t i, std::size_t j)
	{
		// delete first j bits and the result move to start at i-th position
		std::size_t y = (x >> j) << i;

		// take only i-1 first bits of x number
		std::size_t z = x & ((1 << i) - 1);

		return y + z;
	}
	std::size_t bfs_to_veb(std::size_t bfs, std::size_t d, std::size_t h)
	{
		// based on this article: http://people.csail.mit.edu/bradley/papers/Kasheff04.pdf

		if (h <= 3)
			return bfs;

		std::size_t h2 = hyperceil(h >> 1);
		std::size_t h1 = h - h2;

		if (d < h1)
			return bfs_to_veb(bfs, d, h1);

		std::size_t d2 = d - h1;

		std::size_t y = pow2(h1);
		std::size_t a = y - 1;
		std::size_t b = pow2(h2) - 1;

		std::size_t sh = d2 - 1;
		std::size_t i = bfs << sh;

		std::size_t x = a + (i - 1) * b;

		std::size_t bfs2 = fix_bits(bfs, d2, sh);

		return x + bfs_to_veb(bfs2, d2, h2);

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
	}
	std::size_t bfs_to_veb(std::size_t bfs)
	{
		return bfs_to_veb(bfs, height, height) - 1;
	}

	void build_veb(const data_element * idata, std::size_t l, std::size_t r, std::size_t bfs, std::size_t d, std::size_t h)
	{
		if (l < r)
		{
			std::size_t i = (r + l) >> 1;
			std::size_t veb = bfs_to_veb(bfs, d, h);
			data[veb] = idata[i];

			build_veb(idata, l, i, 2 * bfs, d + 1, h);
			build_veb(idata, i + 1, r, 2 * bfs + 1, d + 1, h);
		}
	}

public:
	std::size_t get_size() const { return data_size; }
	std::size_t find(data_element el) const
	{
		return 0;
	}

	emde_boas_search(const data_element * idata, std::size_t isize) :
		data(new data_element[isize]), data_size(isize),
		height(std::ceil(std::log2(isize)) + 1)
	{
		build_veb(idata, 0, isize, 1, 1, height);

		for (std::size_t i = 0; i < isize; ++i)
			std::cout << " " << data[i];
		std::cout << std::endl;
	}
};

class table_search
{
private:
	std::size_t lut_bits = 16;
	std::size_t shift_bits = 32 - lut_bits;

	std::vector<std::pair<std::size_t, std::size_t>> lut;
	data_element min_elem;
	data_element max_elem;

	data_element *data;
	std::size_t data_size;

public:
	std::size_t get_size() const { return data_size; }
	std::size_t find(data_element el) const
	{
		if (el < min_elem)
			return 0;
		/*else if (el >= max_elem)
			return data_size;*/
		
		// table search - direct index
		// based on: https://geidav.wordpress.com/2013/12/29/optimizing-binary-search/
		auto it = lut.at(el >> shift_bits);
		std::size_t l = it.first;
		std::size_t r = it.second;
		std::size_t i = 0;

		// binary search
		while (l + 64 < r)
		{
			i = (l + r) >> 1;
			if (data[i] > el)
				r = i;
			else
				l = i;
		}
		// return l + 1;

		// linear search with sse
		/*__m128i el4 = _mm_set_epi32(el, el, el, el);
		__m128i *d4 = (__m128i*)data;
		std::size_t ln = l >> 2;

		for (;;)
		{
			int cmp = _mm_movemask_epi8(_mm_cmpgt_epi32(el4, d4[ln]));
			if (cmp != 0xffff)
			{
				unsigned long index;
				_BitScanForward(&index, ~cmp);
				std::size_t res = (ln << 2) + (index >> 2);

				if (el == data[res])
					++res;
				else if (r > data_size)
					r = data_size;

				return res;
			}
				
			++ln;
		}*/

		// linear search
		for (;;)
		{
			if (data[l] > el)
				return l;
			++l;
		}
	}

	table_search(const data_element * idata, std::size_t isize) :
		data_size(isize), data(new data_element[isize + 4]),
		min_elem(idata[0]), max_elem(idata[isize - 1])
	{
		lut_bits = isize >= (1 << 18) ? 24 : 16;
		shift_bits = 32 - lut_bits;

		std::copy_n(idata, isize, data);
		data[isize] = ULONG_MAX;
		data[isize + 1] = ULONG_MAX;
		data[isize + 2] = ULONG_MAX;
		data[isize + 3] = ULONG_MAX;

		const std::size_t lut_size = 1 << lut_bits;
		lut.resize(lut_size);

		// populate look-up-table
		size_t thresh = 0, last = 0;

		for (std::size_t i = 0; i < isize - 1; i++)
		{
			const size_t nextThresh = idata[i + 1] >> shift_bits;
			lut[thresh] = { last > 0 ? last - 1 : 0, i + 2 };

			if (nextThresh > thresh) // more than one LUT entry to fill?
			{
				last = i + 1;
				for (std::size_t j = thresh + 1; j <= nextThresh; j++)
					lut[j] = { last > 0 ? last - 1 : 0, i + 2 };
			}

			thresh = nextThresh;
		}

		// set remaining thresholds that couldn't be found
		/*for (std::size_t i = thresh; i < lut.size(); i++)
			lut[i] = { last, isize - 1 };*/
	}
};

//typedef btree_search bsearch_inner;
//typedef binary_search bsearch_inner;
//typedef linear_search bsearch_inner;
//typedef linear_sse_search bsearch_inner;
//typedef block_search bsearch_inner;
//typedef index_search bsearch_inner;
//typedef inter_search bsearch_inner;
//typedef emde_boas_search bsearch_inner;
//typedef hash_search bsearch_inner;
//typedef bucket_search bsearch_inner;
//typedef interval_search bsearch_inner;
typedef table_search bsearch_inner;


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
		
		/*__m128i *b4 = (__m128i*)bucket_size;
		std::size_t count4 = bucket_count >> 2;
		for (std::size_t i = 0; i < count4; ++i)
			b4[i] = _mm_setzero_si128();
		for (std::size_t i = bucket_count - bucket_count % 4; i < bucket_count; ++i)
			bucket_size[i] = 0;*/
		/*for (std::size_t i = 0; i < bucket_count; ++i)
			if (bucket_size[i] != 0)
			{
				std::cout << " non-zero: " << i << std::endl;
				throw 0;
			}*/
			//assert(bucket_size[i] == 0);

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