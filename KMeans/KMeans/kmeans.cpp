#include <vector>
#include <cmath>
#include <cassert>
#include <iterator>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <stdexcept>

// time measurement
#include <omp.h>

#include <tbb\parallel_for.h>
#include <tbb\tick_count.h>

#include "simd_vector.hpp"

typedef unsigned long long value_t;
typedef unsigned char cluster_t;

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

typedef double distance_t;

void generate(std::size_t count,unsigned int seed, data_t& data)
{
	srand(seed);
	data.reserve(count);
	while(count--)
	{
		data.push_back(point(rand(),rand()));

	}
}

inline distance_t distance(const point& a, const point& b)
{
	return std::sqrt((double)((b.x-a.x)*(b.x-a.x)) + ((b.y-a.y)*(b.y-a.y)));
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
	std::vector<std::size_t> counts(means.size(),0);
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
	for(data_t::const_iterator it=data.begin();it!=data.end();++it)
	{
		if (!fwrite(&it->x,sizeof(value_t),1,f)) throw std::runtime_error("value cannot be written");
		if (!fwrite(&it->y,sizeof(value_t),1,f)) throw std::runtime_error("value cannot be written");
	}
	if (fclose(f)) throw std::runtime_error("closing the file failed");
}

void load(const std::string& file_name, data_t& data)
{
	FILE* f;
	fopen_s(&f, file_name.c_str(), "rb");
	//FILE* f = fopen(file_name.c_str(),"rb");
	if (!f) throw std::runtime_error("cannot open file for reading");
	if (fseek(f,0,SEEK_END)) throw std::runtime_error("seeking failed");
	std::size_t count=ftell(f)/(2*sizeof(value_t));
	if (fseek(f,0,SEEK_SET)) throw std::runtime_error("seeking failed");
	while(count--)
	{
		value_t x,y;
		if (!fread(&x,sizeof(value_t),1,f))  throw std::runtime_error("value cannot be read");
		if (!fread(&y,sizeof(value_t),1,f))  throw std::runtime_error("value cannot be read");
		data.push_back(point(x,y));
	}
}

template<typename T>
T lexical_cast(const std::string& x)
{
	std::istringstream stream(x);
	T res;
	stream>>res;
	return res;
}

void usage()
{
		std::cout<<"Usage:"<<std::endl<<"kmeans <data_file> <means_file> <clusters_file> <k> <iterations>"<<std::endl<<"kmeans --generate <data_file> <size> <seed>"<<std::endl;
}

void save_results(const std::string& means_file_name, const std::string& clusters_file_name, const means_t& means, const data_t& data)
{
	FILE* f;
	fopen_s(&f, means_file_name.c_str(), "wb");
	//FILE* f = fopen(means_file_name.c_str(),"wb");
	if (!f) throw std::runtime_error("cannot open file for writing");
	for(means_t::const_iterator it=means.begin();it!=means.end();++it)
	{
		if (!fwrite(&it->x,sizeof(value_t),1,f)) throw std::runtime_error("value cannot be written");
		if (!fwrite(&it->y,sizeof(value_t),1,f)) throw std::runtime_error("value cannot be written");
	}
	if (fclose(f)) throw std::runtime_error("closing the file failed");


	fopen_s(&f, clusters_file_name.c_str(), "wb");
	//f = fopen(clusters_file_name.c_str(),"wb");
	if (!f) throw std::runtime_error("cannot open file for writing");
	for(data_t::const_iterator it=data.begin();it!=data.end();++it)
	{
		if (!fwrite(&it->cluster,sizeof(cluster_t),1,f)) throw std::runtime_error("value cannot be written");
	}
	if (fclose(f)) throw std::runtime_error("closing the file failed");
}

using namespace tbb;
using namespace std;

namespace parallel
{
	typedef unsigned long long value_t;
	typedef unsigned char cluster_t;

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
		center() {}
	};
	inline __m128i zero()
	{
		return _mm_setzero_si128();
	}
	inline __m128i add(__m128i a, __m128i b)
	{
		return _mm_add_epi32(a, b);
	}
	inline __m128i sub(__m128i a, __m128i b)
	{
		return _mm_sub_epi32(a, b);
	}
	inline __m128i mul(__m128i a, __m128i b)
	{
		return _mm_mul_epi32(a, b);
	}
	inline __m128i pow2(__m128i a)
	{
		return _mm_mul_epu32(a, a);
	}
	value_t sum(__m128i a)
	{
		value_t x[2];
		a = add(a, a);
		_mm_storeu_si128((__m128i *)&x, a);

		return x[0];
	}
	/*inline distance_t distance(__m128i a, __m128i b)
	{
		return std::sqrt(sum(pow2(sub(b, a))));
	}*/
	inline value_t pow2(value_t x) { return x * x;  }
	inline distance_t distance(const point& a, const center& b)
	{
		return std::sqrt(pow2(b.x - a.x) + pow2(b.y - a.y));
		//return std::sqrt((double)((b.x - a.x)*(b.x - a.x)) + ((b.y - a.y)*(b.y - a.y)));
	}

	point *points;
	size_t points_count = 1024;
	center *centers;
	cluster_t centers_count = 10;

	struct points_to_clusters
	{
		void operator()(const blocked_range<size_t> &range) const 
		{
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
			}
		}
	};
	struct zero_clusters
	{
		void operator()(const blocked_range<cluster_t> &range) const
		{
			for (cluster_t i = range.begin(); i != range.end(); ++i)
			{
				centers[i].x = 0;
				centers[i].y = 0;
			}
		}
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
		parallel_for(blocked_range<size_t>(0, points_count, 128), points_to_clusters());

	}
	void calculate(std::string points_file_name, std::string means_file_name, std::string clusters_file_name, size_t k, size_t interations)
	{
		centers_count = k;
		load_points(points_file_name);

		for (size_t i = 0; i < interations; ++i)
		{
			calculate_iteration();
		}

		save_results(means_file_name, clusters_file_name);
	}
}

template< typename F>
void measure(F f)
{
	double time = omp_get_wtime();
	std::cout << "BEGIN" << std::endl;

	//tick_count t0 = tick_count::now();

	f();

	//tick_count t1 = tick_count::now();
	//printf("time for action = %g seconds\n", (t1 - t0).seconds());

	//// END
	std::cout << "END" << std::endl;
	time = omp_get_wtime() - time;
	std::cout << "execution time: " << time << " s" << std::endl;
}
float *alinged_aloc(size_t size, size_t alignment)
{
	size_t real_size = size + 4;
	float *mem = new float[real_size];
	void *ptr = (void *)mem;
	float *aligned_mem = (float*)std::align(alignment, size, ptr, real_size);

	return aligned_mem;
}
void test1(size_t count)
{
	float *data1 = alinged_aloc(count, 16);
		//new value_t[count + 2];

	for (size_t i = 0; i < count; ++i)
		data1[i] = 1;

	count >>= 2;
	__m128 *data = (__m128*)data1;
		//new __m128i[count];
	/*for (int i = 0; i < count; ++i)
	{
		data[i] = _mm_set_epi32(0, 1, 0, 1);
	}*/

	__m128 sum = _mm_setzero_ps();
		//parallel::zero();
	//distance_t sum = 0;
	for (size_t i = 0; i < count; ++i)
	{
		sum = _mm_add_ps(sum, data[i]);
			//parallel::add(sum, data[i]);
			//parallel::distance(data[i], data[i + 1]);
	}
	//cout << sum << endl;
	float total;// = parallel::sum(sum);

	__m128 b = _mm_hadd_ps(sum, sum);
	__m128 c = _mm_hadd_ps(b, b);
	_mm_store_ss(&total, c);
	
	cout << total << endl;
}
void test2(size_t count)
{
	float *data = new float[count];
	//point *data = new point[count];
	for (size_t i = 0; i < count; ++i)
	{
		data[i] = 1;
		//data[i].x = 0;
		//data[i].y = 1;
	}

	float sum = 0;
	//distance_t sum = 0;
	for (size_t i = 0; i < count; ++i)
	{
		sum += data[i];
		//distance(&data[i], &data[i + 1]);
	}
	cout << sum << endl;
}



int main(int argc, char **argv)
{
	std::string file_name(argv[1]);
	std::string means_file_name(argv[2]);
	std::string clusters_file_name(argv[3]);
	std::string s_k(argv[4]);
	std::string s_iterations(argv[5]);
	std::size_t k = lexical_cast<std::size_t>(s_k);
	std::size_t iterations = lexical_cast<std::size_t>(s_iterations);

	measure([=] { parallel::calculate(file_name, means_file_name, clusters_file_name, k, iterations); });
	

#ifndef __GCC__
	std::system("pause");
#endif
	return 0;
}

/*int main(int argc, const char* argv[])
{
	if (argc==5)
	{
		std::string option(argv[1]);
		if (option!="--generate") { usage(); return 1; }
		std::string file_name(argv[2]);
		std::string s_size(argv[3]);
		std::string s_seed(argv[4]);
		std::size_t size=lexical_cast<std::size_t>(s_size);
		std::size_t seed=lexical_cast<std::size_t>(s_seed);
		data_t data;
		generate(size,seed,data);
		save(file_name,data);
		return 0;
	}

	if (argc==6)
	{
		std::string file_name(argv[1]);
		std::string means_file_name(argv[2]);
		std::string clusters_file_name(argv[3]);
		std::string s_k(argv[4]);
		std::string s_iterations(argv[5]);
		std::size_t k=lexical_cast<std::size_t>(s_k);
		std::size_t iterations=lexical_cast<std::size_t>(s_iterations);

		data_t data;
		load(file_name,data);

		assert(data.size()>=k);

		data_t means(data.begin(),data.begin()+k);

		while(iterations--)
		{
			assign_to_clusters(data,means);
			compute_means(data,means);
		}

		save_results(means_file_name,clusters_file_name,means,data);
		return 0;
	}
	usage();
	return 1;
}*/
