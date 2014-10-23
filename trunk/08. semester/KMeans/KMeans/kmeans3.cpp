#include <vector>
#include <cmath>
#include <cassert>
#include <iterator>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <stdexcept>
#include <memory>

#include<fstream>

#include <tbb\task_scheduler_init.h>
#include <tbb\parallel_reduce.h>
#include <tbb\parallel_for.h>
#include <tbb\blocked_range.h>
#include <tbb\tick_count.h>

//#include "serial.hpp"
///#include "common.hpp"

using namespace tbb;
using namespace std;

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
			thread_clusters = new center[centers_count];
			//thread_clusters = parallel::clusters[thread_index];
		}
			//thread_clusters(new center[centers_count]), thread_index(0) { }
		kmeans_single_thread(kmeans_single_thread& s, split) : thread_index((s.thread_index + 1) % thread_count) 
		{
			thread_clusters = new center[centers_count];
			//thread_clusters = parallel::clusters[thread_index];
		}
			//thread_clusters(new center[centers_count]), thread_index(s.thread_index + 1) { }
		~kmeans_single_thread() { delete[] thread_clusters; }
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
	iterations = 30;

	cout << "iteratrions: " << iterations << endl;
	cout << "clusters:    " << k << endl;

	cout << endl << "parallel: " << endl;
	measure([=] { parallel::calculate(file_name, means_file_name + ".par", clusters_file_name + ".par", k, iterations); });

	cout << "serial:" << endl;
	//measure([=] { serial1::calculate(file_name, means_file_name + ".ser" , clusters_file_name + ".ser", k, iterations); });
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