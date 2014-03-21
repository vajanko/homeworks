#include <vector>
#include <iostream>
#include <sstream>
#include <memory>
#include <cassert>

#include<fstream>

#include <tbb\task_scheduler_init.h>
#include <tbb\parallel_reduce.h>
#include <tbb\blocked_range.h>
#include <tbb\tick_count.h>

#include <emmintrin.h>

using namespace tbb;
using namespace std;

namespace serial1
{
	typedef unsigned long long value_t;
	typedef unsigned char cluster_t;
	typedef double distance_t;

	struct point
	{
		point(value_t x, value_t y) : x(x), y(y), cluster(0) {}
		point() {}
		value_t x;
		value_t y;
		cluster_t cluster;
	};

	typedef std::vector<point> data_t;
	typedef std::vector<point> means_t;

	void generate(std::size_t count, unsigned int seed, data_t& data)
	{
		srand(seed);
		data.reserve(count);
		while (count--)
		{
			data.push_back(point(rand(), rand()));

		}
	}

	inline distance_t distance(const point& a, const point& b)
	{
		return std::sqrt((double)((b.x - a.x)*(b.x - a.x)) + ((b.y - a.y)*(b.y - a.y)));
	}

	void assign_to_clusters(data_t& data, const means_t& means)
	{
		assert(means.size()>0);
		for (data_t::iterator it = data.begin(); it != data.end(); ++it)
		{
			cluster_t closest_cluster(0);
			distance_t mindist(distance(*it, means[0]));
			for (means_t::const_iterator mit = means.begin() + 1; mit != means.end(); ++mit)
			{
				distance_t dist = distance(*it, *mit);
				if (dist < mindist) { closest_cluster = static_cast<cluster_t>(std::distance(means.begin(), mit)); mindist = dist; }
			}
			it->cluster = closest_cluster;
		}
	}

	void compute_means(const data_t& data, means_t& means)
	{
		std::vector<std::size_t> counts(means.size(), 0);
		for (means_t::iterator mit = means.begin(); mit != means.end(); ++mit)
		{
			mit->x = 0;
			mit->y = 0;
		}
		for (data_t::const_iterator it = data.begin(); it != data.end(); ++it)
		{
			++counts[it->cluster];
			means[it->cluster].x += it->x;
			means[it->cluster].y += it->y;
		}
		for (means_t::iterator mit = means.begin(); mit != means.end(); ++mit)
		{
			mit->x /= counts[std::distance(means.begin(), mit)];
			mit->y /= counts[std::distance(means.begin(), mit)];
		}
	}

	void save(const std::string& file_name, const data_t& data)
	{
		FILE* f;
		fopen_s(&f, file_name.c_str(), "wb");
		//FILE* f = fopen(file_name.c_str(),"wb");
		if (!f) throw std::runtime_error("cannot open file for writing");
		for (data_t::const_iterator it = data.begin(); it != data.end(); ++it)
		{
			if (!fwrite(&it->x, sizeof(value_t), 1, f)) throw std::runtime_error("value cannot be written");
			if (!fwrite(&it->y, sizeof(value_t), 1, f)) throw std::runtime_error("value cannot be written");
		}
		if (fclose(f)) throw std::runtime_error("closing the file failed");
	}

	void load(const std::string& file_name, data_t& data)
	{
		FILE* f;
		fopen_s(&f, file_name.c_str(), "rb");
		//FILE* f = fopen(file_name.c_str(),"rb");
		if (!f) throw std::runtime_error("cannot open file for reading");
		if (fseek(f, 0, SEEK_END)) throw std::runtime_error("seeking failed");
		std::size_t count = ftell(f) / (2 * sizeof(value_t));
		if (fseek(f, 0, SEEK_SET)) throw std::runtime_error("seeking failed");
		while (count--)
		{
			value_t x, y;
			if (!fread(&x, sizeof(value_t), 1, f))  throw std::runtime_error("value cannot be read");
			if (!fread(&y, sizeof(value_t), 1, f))  throw std::runtime_error("value cannot be read");
			data.push_back(point(x, y));
		}
	}

	void save_results(const std::string& means_file_name, const std::string& clusters_file_name, const means_t& means, const data_t& data)
	{
		FILE* f;
		fopen_s(&f, means_file_name.c_str(), "wb");
		//FILE* f = fopen(means_file_name.c_str(),"wb");
		if (!f) throw std::runtime_error("cannot open file for writing");
		for (means_t::const_iterator it = means.begin(); it != means.end(); ++it)
		{
			if (!fwrite(&it->x, sizeof(value_t), 1, f)) throw std::runtime_error("value cannot be written");
			if (!fwrite(&it->y, sizeof(value_t), 1, f)) throw std::runtime_error("value cannot be written");
		}
		if (fclose(f)) throw std::runtime_error("closing the file failed");


		fopen_s(&f, clusters_file_name.c_str(), "wb");
		//f = fopen(clusters_file_name.c_str(),"wb");
		if (!f) throw std::runtime_error("cannot open file for writing");
		for (data_t::const_iterator it = data.begin(); it != data.end(); ++it)
		{
			if (!fwrite(&it->cluster, sizeof(cluster_t), 1, f)) throw std::runtime_error("value cannot be written");
		}
		if (fclose(f)) throw std::runtime_error("closing the file failed");
	}

	void calculate(const std::string &points_file_name, const std::string &means_file_name, const std::string &clusters_file_name, size_t k, size_t iterations)
	{
		data_t data;
		load(points_file_name, data);

		assert(data.size() >= k);

		data_t means(data.begin(), data.begin() + k);

		while (iterations--)
		{
			assign_to_clusters(data, means);
			compute_means(data, means);
		}

		save_results(means_file_name, clusters_file_name, means, data);
	}
}

namespace simd
{
	template<typename Single, typename Multiple> struct Op;

	template<> struct Op<float, __m128>
	{
		static __m128 zero() { return _mm_setzero_ps(); }
		static __m128 add(__m128 a, __m128 b) { return _mm_add_ps(a, b); }
		static float first(__m128 a)
		{
			float x;
			_mm_store_ss(&x, a);
			return x;
		}
	};
	template<> struct Op<double, __m128d>
	{
		static __m128d zero() { return _mm_setzero_pd(); }
		static __m128d add(__m128d a, __m128d b) { return _mm_add_pd(a, b); }
		static double first(__m128d a)
		{
			double x;
			_mm_store_sd(&x, a);
			return x;
		}
	};
	template<> struct Op<long, __m128i>
	{
		static __m128i zero() { return _mm_setzero_si128(); }
		static __m128i add(__m128i a, __m128i b) { return _mm_add_epi32(a, b); }
		static long first(__m128i a)
		{
			long res[4];
			_mm_storeu_si128((__m128i*)&res, a);
			return res[0];
		}
	};
	template<> struct Op<int, __m128i>
	{
		static __m128i zero() { return _mm_setzero_si128(); }
		static __m128i add(__m128i a, __m128i b) { return _mm_add_epi32(a, b); }
		static int first(__m128i a)
		{
			int res[4];
			_mm_storeu_si128((__m128i*)&res, a);
			return res[0];
		}
	};
	template<> struct Op<long long, __m128i>
	{
		static __m128i zero() { return _mm_setzero_si128(); }
		static __m128i add(__m128i a, __m128i b) { return _mm_add_epi32(a, b); }
		static long long first(__m128i a)
		{
			long long res[2];
			_mm_storeu_si128((__m128i*)&res, a);
			return res[0];
		}
	};
	template<> struct Op<unsigned long long, __m128i>
	{
		static __m128i zero() { return _mm_setzero_si128(); }
		static __m128i add(__m128i a, __m128i b) { return _mm_add_epi32(a, b); }
		static unsigned long long first(__m128i a)
		{
			unsigned long long res[2];
			_mm_storeu_si128((__m128i*)&res, a);
			return res[0];
		}
	};

	template<typename Single, typename Multiple>
	Single sum_simd(Single *data1, size_t count1)
	{
		const size_t multiplicity = sizeof(Multiple) / sizeof(Single);

		Multiple *data = (Multiple *)data1;
		size_t count = count1 / multiplicity;

		Multiple sum = Op<Single, Multiple>::zero();
		for (size_t i = 0; i < count; ++i)
		{
			sum = Op<Single, Multiple>::add(sum, data[i]);
		}

		for (size_t i = 0; i < multiplicity / 2; ++i)
			sum = Op<Single, Multiple>::add(sum, sum);

		//sum = Op<Single, Multiple>::add(sum, sum);

		return Op<Single, Multiple>::first(sum);
	}
	template<typename Single>
	Single sum_seq(Single *data, size_t count)
	{
		Single sum = 0;
		for (size_t i = 0; i < count; ++i)
		{
			sum += data[i];
		}
		return sum;
	}

#ifndef __GCC__
	template<typename Single, typename Multiple>
	void test(size_t size)
	{
		Single *data1 = new Single[size];
		Single *data2 = new Single[size];
		for (size_t i = 0; i < size; ++i)
		{
			data1[i] = 1;
			data2[i] = 1;
		}

		cout << endl << "parallel: ";
		measure([&] { cout << sum_simd<Single, Multiple>(data1, size) << " ";  });
		cout << endl;

		cout << "serial: ";
		measure([&] { cout << sum_seq<Single>(data2, size) << " "; });
		cout << endl;
	}
#endif
}

using namespace simd;

namespace parallel
{
	typedef unsigned long long value_t;
	typedef unsigned char cluster_t;
	typedef double distance_t;

	struct point
	{
		value_t x;
		value_t y;
		cluster_t cluster;
		point(value_t x, value_t y) : x(x), y(y), cluster(0) {}
		point() {}
	};
	struct center
	{	// center of a cluster
		value_t x;
		value_t y;
		size_t count;		// number of points in this cluster
		center(value_t x, value_t y) : x(x), y(y), count(0) {}
		center() : x(0), y(0), count(0) {}
	};
	
	inline value_t pow2(value_t x) { return x * x;  }
	inline distance_t distance(const point& a, const center& b)
	{
		return std::sqrt(pow2(b.x - a.x) + pow2(b.y - a.y));
	}

	point *points;
	size_t points_count = 1024;

	center *centers;
	cluster_t centers_count = 10;

	center **clusters;
	size_t thread_count = 4;

	struct kmeans_single_thread
	{
		// centers[] is readonly
		// points[] cluster is written (this could be special array)

		center *thread_clusters;
		const size_t thread_index;

		void operator()(const blocked_range<size_t> &range) const
		{
			// divide points into cluster on current range (localy)
			for (size_t i = range.begin(); i != range.end(); ++i)
			{
				distance_t min = distance(points[i], centers[0]);
				cluster_t min_index = 0;
				distance_t cur;

				// iterate over center points
				for (cluster_t j = 1; j < centers_count; ++j)
				{
					cur = distance(points[i], centers[j]);
					if (cur < min)	// if equal, cluster with smaller index wins
					{
						min = cur;
						min_index = j;
					}
				}

				// add points[i] to cluster min_index
				// points[i] is processed by only one thread, this is not true for centers[min_index]
				points[i].cluster = min_index;

				// add point to cluster (localy for this thread)
				++thread_clusters[min_index].count;
				// calculate total sum of distances for current cluster (locally for this thread)
				thread_clusters[min_index].x += points[i].x;
				thread_clusters[min_index].y += points[i].y;
			}

		}

		void join(kmeans_single_thread &rhs)
		{
			for (size_t i = 0; i < centers_count; ++i)
			{
				thread_clusters[i].count += rhs.thread_clusters[i].count;
				thread_clusters[i].x += rhs.thread_clusters[i].x;
				thread_clusters[i].y += rhs.thread_clusters[i].y;
			}
		}

		kmeans_single_thread() : thread_index(0) 
		{ 
			thread_clusters = parallel::clusters[thread_index];
		}
			//thread_clusters(new center[centers_count]), thread_index(0) { }
		kmeans_single_thread(kmeans_single_thread& s, split) : thread_index((s.thread_index + 1) % thread_count) 
		{
			thread_clusters = parallel::clusters[thread_index];
		}
			//thread_clusters(new center[centers_count]), thread_index(s.thread_index + 1) { }
		//~kmeans_single_thread() { delete[] thread_clusters; }
	};
	
	void load_points(const std::string& file_name)
	{
		// open file
		FILE* f;
		fopen_s(&f, file_name.c_str(), "rb");
		if (!f) throw std::runtime_error("cannot open file for reading");
		if (fseek(f, 0, SEEK_END)) throw std::runtime_error("seeking failed");
		std::size_t count = ftell(f) / (2 * sizeof(value_t));
		if (fseek(f, 0, SEEK_SET)) throw std::runtime_error("seeking failed");

		// allocate memory
		points_count = count;
		points = new point[points_count];

		// initialize points
		value_t point[2];
		for (size_t i = 0; i < count; ++i)
		{
			fread(&point, sizeof(value_t), 2, f);
			points[i].x = point[0];
			points[i].y = point[1];
		}

		// first "center_count" points are centers
		centers = new center[centers_count];
		for (cluster_t i = 0; i < centers_count; ++i)
		{
			centers[i].x = points[i].x;
			centers[i].y = points[i].y;
		}

		//
		clusters = new center*[thread_count];
		for (size_t i = 0; i < thread_count; ++i)
		{
			clusters[i] = new center[centers_count];
		}
	}
	void save_results(const std::string& means_file_name, const std::string& clusters_file_name)
	{
		// open centers file
		FILE* f;
		fopen_s(&f, means_file_name.c_str(), "wb");
		if (!f) throw std::runtime_error("cannot open file for writing");

		// write centers file
		for (cluster_t i = 0; i < centers_count; ++i)
		{
			fwrite(&centers[i].x, sizeof(value_t), 1, f);
			fwrite(&centers[i].y, sizeof(value_t), 1, f);
		}
		// close
		if (fclose(f)) throw std::runtime_error("closing the file failed");

		// open
		fopen_s(&f, clusters_file_name.c_str(), "wb");
		if (!f) throw std::runtime_error("cannot open file for writing");

		// write clusters assignment
		for (size_t i = 0; i < points_count; ++i)
		{
			fwrite(&points[i].cluster, sizeof(cluster_t), 1, f);
		}

		// close
		if (fclose(f)) throw std::runtime_error("closing the file failed");
	}
	void calculate_iteration()
	{
		kmeans_single_thread result;
		blocked_range<size_t> range(0, points_count, 64);
		simple_partitioner partitioner;

		parallel_reduce(range, result, partitioner);

		// computer mean values
		for (size_t i = 0; i < centers_count; ++i)
		{
			centers[i].x = result.thread_clusters[i].x / result.thread_clusters[i].count;
			centers[i].y = result.thread_clusters[i].y / result.thread_clusters[i].count;
		}
	}
	void calculate(const std::string &points_file_name, const std::string &means_file_name, const std::string &clusters_file_name, size_t k, size_t iterations)
	{
		centers_count = (cluster_t)k;
		load_points(points_file_name);

		for (size_t i = 0; i < iterations; ++i)
		{
			calculate_iteration();
		}

		save_results(means_file_name, clusters_file_name);
	}
}

namespace common
{
	template<typename T>
	T lexical_cast(const std::string& x)
	{
		std::istringstream stream(x);
		T res;
		stream >> res;
		return res;
	}

	void usage()
	{
		std::cout << "Usage:" << std::endl << "kmeans <data_file> <means_file> <clusters_file> <k> <iterations>" << std::endl << "kmeans --generate <data_file> <size> <seed>" << std::endl;
	}

	template< typename F>
	void measure(F f)
	{
		tick_count t0 = tick_count::now();

		f();

		tick_count t1 = tick_count::now();
		printf("time for action = %g seconds\n", (t1 - t0).seconds());
	}
	float *alinged_aloc(size_t size, size_t alignment)
	{
		size_t real_size = size + 4;
		float *mem = new float[real_size];
		void *ptr = (void *)mem;
		float *aligned_mem = (float*)std::align(alignment, size, ptr, real_size);

		return aligned_mem;
	}

	bool compare(const std::string& lFilePath, const std::string& rFilePath)
	{
		const size_t BUFFER_SIZE = 1024;
		 
		std::ifstream lFile(lFilePath.c_str(), std::ifstream::in | std::ifstream::binary);
		std::ifstream rFile(rFilePath.c_str(), std::ifstream::in | std::ifstream::binary);

		if (!lFile.is_open() || !rFile.is_open())
		{
			return false;
		}

		char *lBuffer = new char[BUFFER_SIZE]();
		char *rBuffer = new char[BUFFER_SIZE]();

		do {
			lFile.read(lBuffer, BUFFER_SIZE);
			rFile.read(rBuffer, BUFFER_SIZE);
			auto numberOfRead = lFile.gcount();//I check the files with the same size

			if (std::memcmp(lBuffer, rBuffer, numberOfRead) != 0)
			{
				memset(lBuffer, 0, numberOfRead);
				memset(rBuffer, 0, numberOfRead);
				return false;
			}
		} while (lFile.good() || rFile.good());

		delete[] lBuffer;
		delete[] rBuffer;
		return true;
	}

	int generate_file(char **argv)
	{
		std::string option(argv[1]);
		if (option != "--generate") { usage(); return 1; }
		std::string file_name(argv[2]);
		std::string s_size(argv[3]);
		std::string s_seed(argv[4]);
		std::size_t size = lexical_cast<std::size_t>(s_size);
		unsigned int seed = lexical_cast<unsigned int>(s_seed);
		serial1::data_t data;
		generate(size, seed, data);
		save(file_name, data);

		return 0;
	}
}

using namespace common;


//int main(int argc, char **argv)
//{
//	size_t count = 1000000;
//	test<float, __m128>(count);
//	test<long, __m128i>(count);
//	test<int, __m128i>(count);
//	test<long long, __m128i>(count);
//
//	#ifndef __GCC__
//		std::system("pause");
//	#endif
//		return 0;
//}

int main(int argc, char **argv)
{
	task_scheduler_init init(4);
	parallel::thread_count = 4;
		//init.default_num_threads();

	if (argc == 5)
	{
		return generate_file(argv);
	}
	else if (argc != 6)
	{
		usage();
		return 1;
	}

	std::string file_name(argv[1]);
	std::string means_file_name(argv[2]);
	std::string clusters_file_name(argv[3]);
	std::string s_k(argv[4]);

	std::string s_iterations(argv[5]);
	std::size_t k = lexical_cast<std::size_t>(s_k);
	std::size_t iterations = lexical_cast<std::size_t>(s_iterations);

	// customize
	//iterations = 30;

	cout << "iteratrions: " << iterations << endl;
	cout << "clusters:    " << k << endl;

	cout << endl << "parallel: " << endl;
	measure([=] { parallel::calculate(file_name, means_file_name + ".par", clusters_file_name + ".par", k, iterations); });

	cout << "serial:" << endl;
	measure([=] { serial1::calculate(file_name, means_file_name + ".ser" , clusters_file_name + ".ser", k, iterations); });
	cout << endl;

	bool comparison1 = compare(means_file_name + ".ser", means_file_name + ".par");
	cout << "Comparison result (means file): " << comparison1 << endl;

	bool comparison2 = compare(clusters_file_name + ".ser", clusters_file_name + ".par");
	cout << "Comparison result (clusters file): " << comparison2 << endl;

#ifndef __GCC__
	std::system("pause");
#endif
	return 0;
}