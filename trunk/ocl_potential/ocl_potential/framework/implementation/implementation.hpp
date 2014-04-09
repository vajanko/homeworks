#ifndef OCL_POTENTIAL_FRAMEWORK_IMPLEMENTATION_HPP
#define OCL_POTENTIAL_FRAMEWORK_IMPLEMENTATION_HPP

#include<algorithm>

#include "../interface.hpp"
#include "../data.hpp"

#include <CL/cl.hpp>


/*
 * Final implementation of the tested OCL program.
 */
class OCLProgram_Implementation : public OCLProgram
{
private:
	typedef float in_real_t;

	cl::Buffer mVelocities;
	cl::Buffer mForces;
	cl::Buffer mPoints;
	index_t mPointCount;
	cl::Buffer mEdges;
	index_t mEdgeCount;

	struct edge
	{
		unsigned int p1, p2, len;
		edge() : p1(0), p2(0), len(0) { }
	};

public:
	OCLProgram_Implementation()
	{
		// This is the place to initialize mKernelSources member field.
		// The path to the kernel file must be relative to the executable (i.e., to the main.cpp).
		// i.e. mKernelSources["kernelName"] = KernelSource("implementation/kernelFile.cl", true);
#ifndef __GNUC__
		mKernelSources["iteration"] = KernelSource("C:\\Projects\\Homeworks\\ocl_potential\\ocl_potential\\framework\\implementation\\iteration.cl", true);
#else
		mKernelSources["iteration"] = KernelSource("implementation/iteration.cl", true);		
#endif
	}

	length_t get_length(const std::vector<Edge>& edges, const std::vector<length_t> &lengths, size_t x, size_t y)
	{
		for (int i = 0; i < edges.size(); ++i)
		{
			if (edges[i].p1 == x && edges[i].p2 == y)
				return lengths[i];
		}
		return 0;
	}
	edge *transform_graph(const std::vector<Edge>& edges, const std::vector<length_t> &lengths, size_t points, size_t &width)
	{
		std::vector<size_t> neighbours(edges.size(), 0);
		for (size_t i = 0; i < edges.size(); ++i)
		{
			++neighbours[edges[i].p1];
		}
		size_t max_n = *std::max_element(neighbours.begin(), neighbours.end());

		edge *graph = new edge[points * max_n];
		for (size_t i = 0; i < points; ++i)
		{
			size_t k = 0;
			for (size_t j = 0; j < points; ++j)
			{
				graph[i * max_n + j].len = get_length(edges, lengths, i, j);
				if (graph[i * max_n + j].len != 0)
				{
					graph[i * max_n + j].p1 = i;
					graph[i * max_n + j].p2 = j;
				}
			}
		}

		width = max_n;
	}

	// points = number of points in the simulation
	virtual void initialize(index_t points, const std::vector<Edge>& edges, const std::vector<length_t> &lengths, index_t iterations)
	{
		/*
		 * Initialization of the simulation.
		 * You may allocate buffers, transform or precompute data, or transfer initial values to the GPU.
		 */
		// allocate buffers
		mPointCount = points;
		mEdgeCount = edges.size();

		Point *velocities = new Point[points];
		for (size_t i = 0; i < points; ++i)
			velocities[i].x = velocities[i].y = 0;

		//size_t width;
		//edge *graph = transform_graph(edges, lengths, points, width);

		edge *my_edges = new edge[mEdgeCount];
		for (size_t i = 0; i < mEdgeCount; ++i)
		{
			my_edges[i].p1 = edges[i].p1;
			my_edges[i].p2 = edges[i].p2;
			my_edges[i].len = lengths[i];
		}

		cl::Context context = *mContext;
		mPoints = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(Point)* points);
		mVelocities = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(Point)* points, velocities);
		mForces = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(Point)* points, velocities);
		mEdges = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(edge)* mEdgeCount, my_edges);
	
		cl::Kernel &kernel = mKernels->at("iteration");
		kernel.setArg(0, sizeof(cl::Buffer), &mPoints);
		kernel.setArg(1, sizeof(cl::Buffer), &mVelocities);
		kernel.setArg(2, sizeof(cl::Buffer), &mForces);
		kernel.setArg(3, sizeof(cl::Buffer), &mEdges);
		kernel.setArg(4, mEdgeCount);
		kernel.setArg(5, mVertexRepulsion);
		kernel.setArg(6, mVertexMass);
		kernel.setArg(7, mEdgeCompulsion);
		kernel.setArg(8, mSlowdown);
		kernel.setArg(9, mTimeQuantum);

		//cl_int err;
		//const cl::Device &dev = mDevices->at(0);
		//cl::CommandQueue commandQueue(context, dev, 0, &err);

		//commandQueue.enqueueWriteBuffer(mVelocities, CL_TRUE, 0, sizeof(Point)* mPointCount, velocities);
		//commandQueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(points), cl::NDRange(256), NULL, NULL);

		//commandQueue.enqueueWriteBuffer(mVelocities, CL_TRUE, 0, sizeof(float)*points * 2, velocities);
		//commandQueue.finish();
	}

	virtual void iteration(std::vector<Point> &points)
	{
		/*
		 * The main function that computes next version of the point locations
		 * and updates internal velocities.
		 */

		cl_int err;
		const cl::Device &dev = mDevices->at(0);
		cl::Context context = *mContext;
		cl::CommandQueue commandQueue(context, dev, 0, &err);

		commandQueue.enqueueWriteBuffer(mPoints, CL_TRUE, 0, sizeof(Point)*mPointCount, points.data());

		cl::Kernel &kernel = mKernels->at("iteration");
		commandQueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(mPointCount), cl::NDRange(256), NULL, NULL);
		commandQueue.finish();

		err = commandQueue.enqueueReadBuffer(mPoints, CL_TRUE, 0, sizeof(Point)*mPointCount, points.data());
		commandQueue.finish();
	}


	virtual void getVelocities(std::vector<Point> &velocities)
	{
		/*
		 * This function should only retrieve the internal velocity buffer.
		 * It does not have to be implemented efficiently, since its working
		 * time is not added to total measured time.
		 */
		cl_int err;
		const cl::Device &dev = mDevices->at(0);
		cl::Context context = *mContext;
		cl::CommandQueue commandQueue(context, dev, 0, &err);

		Point *vels = new Point[mPointCount];
		err = commandQueue.enqueueReadBuffer(mVelocities, CL_TRUE, 0, sizeof(Point)*mPointCount, vels);

		velocities.clear();
		std::vector<Point> tmp(vels, vels + mPointCount);
		velocities.assign(tmp.begin(), tmp.end());
	}
};


#endif
