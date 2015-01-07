#include <iostream>
#include <cassert>
#include <cstdlib>
#include <chrono>
#include <random>
#include <string>
#include <cmath>
#include <algorithm>

typedef unsigned long data_element;

using namespace std;

const int SORTED_ARRAY_SIZE = 100 * 1000 * 1000;
const int NUM_ITERS = 100 * 1000;
//const int NUM_ITERS = 1;
const int L3_INT_ARRAY_SIZE = 100 * 1000;

void show_sketch(string name, int *array, int array_size) {
	cout << name << ": [ ";
	for (int i = 0; i < 5; ++i) {
		cout << array[i] << " ";
	}
	cout << "... ";
	for (int i = array_size - 5; i < array_size; ++i) {
		cout << array[i] << " ";
	}
	cout << "]\n";
}

int *get_sorted_array(const int array_size) {
	int *result = new int[array_size];
	int delta = 10;
	result[0] = 0;
	for (int i = 1; i < array_size; ++i) {
		result[i] = result[i - 1] + rand() % delta + 1;
	}
	assert(0.2 * delta * array_size <= result[array_size - 1]);
	assert(result[array_size - 1] <= delta * array_size);
	return result;
}

int *get_random_array(int array_size, int rand_max) {
	int *result = new int[array_size];
	std::minstd_rand generator;
	std::uniform_int_distribution<> random_int_generator(0, rand_max);
	for (int i = 0; i < array_size; ++i) {
		result[i] = random_int_generator(generator);
	}
	return result;
}

class SlowBinarySearch {
private:
	int *sorted_array_;
	int sorted_array_size_;
public:
	SlowBinarySearch() {}

	SlowBinarySearch(int *sorted_array, int sorted_array_size) :
		sorted_array_(sorted_array), sorted_array_size_(sorted_array_size) {

	}

	int upper_bound(int query) const {
		int left = 0, right = sorted_array_size_;
		while (left + 1 < right) {
			//            cout << "slow_run " << left << " " << right << "\n";
			int mid = (left + right) >> 1;
			if (sorted_array_[mid] <= query) {
				left = mid;
			}
			else {
				right = mid;
			}
		}
		return left;
	}
};

class FastBinarySearch {
private:
	int *sorted_array_;
	int sorted_array_size_, chunk_size_;
	SlowBinarySearch topLevelBinarySearch_;
public:
	FastBinarySearch(int *sorted_array, int sorted_array_size) :
		sorted_array_(sorted_array), sorted_array_size_(sorted_array_size) {

		chunk_size_ = sorted_array_size / L3_INT_ARRAY_SIZE;

		int inner_array_size = ceil((double)sorted_array_size / chunk_size_);

		cout << "chunk_size: " << chunk_size_ << "\n";
		cout << "inner_array_size: " << inner_array_size << "\n";

		int *top_level_sorted_array = new int[inner_array_size];
		for (int i = 0; i < inner_array_size; ++i) {
			top_level_sorted_array[i] = sorted_array[i * chunk_size_];
		}
		topLevelBinarySearch_ = SlowBinarySearch(top_level_sorted_array, inner_array_size);

		show_sketch("top_level_array", top_level_sorted_array, inner_array_size);
	}

	int upper_bound(int query) const {
		int top_level_answer = topLevelBinarySearch_.upper_bound(query);
		int left = top_level_answer * chunk_size_;
		int right = min((top_level_answer + 1) * chunk_size_, sorted_array_size_);
		while (left + 1 < right) {
			int mid = (left + right) >> 1;
			if (sorted_array_[mid] <= query) {
				left = mid;
			}
			else {
				right = mid;
			}
		}
		return left;
	}
};

class Measurer {
public:
	Measurer(string name, int num_iters) : name_(name), num_iters_(num_iters) {}

	void start() {
		start_ = std::chrono::high_resolution_clock::now();
	}

	void stop() {
		auto stop = std::chrono::high_resolution_clock::now();
		auto delta = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start_).count();
		assert((sizeof delta) == 8);
		std::cout << name_ << ", " << num_iters_ << " iterations: " << delta << " ns\n";
		std::cout << name_ << ": " << delta / (double)num_iters_ << " ns per iteration\n";
	}

private:
	std::chrono::high_resolution_clock::time_point start_;
	std::string name_;
	int num_iters_;
};

void assertEquals(int *first_array, int *second_array, int array_size) {
	for (int i = 0; i < array_size; ++i) {
		if (first_array[i] != second_array[i]) {
			throw std::runtime_error(string("difference at ") + to_string(i) + ": "
				"first_array[i] = " + to_string(first_array[i]) + ", "
				"second_array[i] = " + to_string(second_array[i]));
		}
	}
}

int main1() {
	int *sorted_array = get_sorted_array(SORTED_ARRAY_SIZE);

	int *query = get_random_array(NUM_ITERS, sorted_array[SORTED_ARRAY_SIZE - 1]);



	int *slow_result = new int[NUM_ITERS];

	show_sketch("sorted_array", sorted_array, SORTED_ARRAY_SIZE);
	show_sketch("query", query, NUM_ITERS);

	Measurer measurer_slow("slow", NUM_ITERS);
	SlowBinarySearch slowBinarySearch(sorted_array, SORTED_ARRAY_SIZE);

	measurer_slow.start();
	for (int i = 0; i < NUM_ITERS; ++i) {
		slow_result[i] = slowBinarySearch.upper_bound(query[i]);
	}
	measurer_slow.stop();

	show_sketch("slow_result", slow_result, NUM_ITERS);



	int *fast_result = new int[NUM_ITERS];

	Measurer measurer_fast("fast", NUM_ITERS);
	FastBinarySearch fastBinarySearch(sorted_array, SORTED_ARRAY_SIZE);

	// warm-up iterations
	for (int i = 0; i < NUM_ITERS; ++i) {
		fastBinarySearch.upper_bound(query[i]);
	}

	measurer_fast.start();
	for (int i = 0; i < NUM_ITERS; ++i) {
		fast_result[i] = fastBinarySearch.upper_bound(query[i]);
	}
	measurer_fast.stop();

	show_sketch("fast_result", fast_result, NUM_ITERS);



	assertEquals(slow_result, fast_result, NUM_ITERS);

	return 0;
}

void build_veb(data_element* inputArray, data_element* outputArray, 
	std::size_t root, std::size_t  height, std::size_t  depth, std::size_t  N) {

	static int recursion = 0;
	static int idx = 0;
	if (N <= 3) 
	{
		outputArray[idx++] = inputArray[root];
		if (N == 3) {
			int lchild = 2 * root + 1;
			int rchild = 2 * root + 2;
			outputArray[idx++] = inputArray[lchild];
			outputArray[idx++] = inputArray[rchild];
		}
		return;
	}

	//int totalHeight = static_cast<int>((std::log(N+1)/std::log(2)));
	//int h0 = static_cast<int>(std::floor(totalHeight/2));
	//int t = totalHeight - h0;
	std::size_t totalHeight = static_cast<std::size_t>(std::log(N + 1) / std::log(2));

	// is seems that here it should be enough to have height / 2
	std::size_t h0 = static_cast<std::size_t>(std::floor(static_cast<float>(height) / 2));
	// and here height / 2 + height % 2
	std::size_t t = static_cast<std::size_t>(std::ceil(static_cast<float>(height) / 2));

	//std::cout << "Recursion         :" << recursion << std::endl;
	//std::cout << "Total Tree Height :" << totalHeight << "\n";
	//std::cout << "H0 height         :" << h0 << "\n";
	//std::cout << "Rem height        :" << t << "\n";

	std::size_t h0nodes = static_cast<std::size_t>(std::pow(2, h0)) - 1;
	std::size_t remNodes = N - h0nodes;		// reminening nodes
	/*std::cout << "H0 nodes          :" << h0nodes << std::endl;
	std::cout << "Rem nodes         :" << remNodes << std::endl;*/

	// now calculate number of subtrees 
	std::size_t numSubtrees = static_cast<std::size_t>(std::pow(2, h0));
	std::size_t perSubtreeElement = remNodes / numSubtrees;

	/*std::cout << "Num of Subtrees	  :" << numSubtrees << std::endl;
	std::cout << "Per subtree cnt   :" << perSubtreeElement << std::endl;
	std::cout << "----------------------------------------------------" << std::endl;*/
	recursion++;
	// now for tree rooted at @h0, we launch one recursive function
	build_veb(inputArray, outputArray, root, h0, t, h0nodes);
	// and for all subtrees @later height, we launch recursive functions
	for (int i = 0; i < numSubtrees; i++) {
		// find the root elements for all the subtrees
		std::size_t r = static_cast<std::size_t>(std::pow(2, t)) - 1;
		r += i;		//move appropriately along the dimension
		build_veb(inputArray, outputArray, r, t, t + 1, perSubtreeElement);
	}
}
