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

	std::vector<Edge> &mEdges;				///< Reference to the graph edges.
	std::vector<length_t> &mLengths;		///< Reference to the graph lengths of the edges.
	cl::Buffer mVelocities;
	cl::Buffer mForces;
	cl::Buffer mPoints;
	index_t mPointCount;

public:
	OCLProgram_Implementation() : mEdges(std::vector<Edge>()), mLengths(std::vector<length_t>())
	{
		// This is the place to initialize mKernelSources member field.
		// The path to the kernel file must be relative to the executable (i.e., to the main.cpp).
		// i.e. mKernelSources["kernelName"] = KernelSource("implementation/kernelFile.cl", true);
		mKernelSources["iteration"] = KernelSource("C:\\Projects\\Homeworks\\ocl_potential\\ocl_potential\\framework\\implementation\\iteration.cl", true);
	}

	// points = number of points in the simulation
	virtual void initialize(index_t points, const std::vector<Edge>& edges, const std::vector<length_t> &lengths, index_t iterations)
	{
		/*
		 * Initialization of the simulation.
		 * You may allocate buffers, transform or precompute data, or transfer initial values to the GPU.
		 */
		//mEdges = edges;
		//mLengths = lengths;

		// allocate buffers
		mPointCount = points;

		Point *velocities = new Point[points];
		for (size_t i = 0; i < points; ++i)
			velocities[i].x = velocities[i].y = 0;

		cl::Context context = *mContext;
		mPoints = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(Point)* points);
		mVelocities = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(Point)* points, velocities);
		mForces = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(Point)* points, velocities);
	
		cl::Kernel &kernel = mKernels->at("iteration");
		kernel.setArg(0, sizeof(cl::Buffer), &mPoints);
		kernel.setArg(1, sizeof(cl::Buffer), &mVelocities);
		kernel.setArg(2, sizeof(cl::Buffer), &mForces);
		kernel.setArg(3, mTimeQuantum);

		cl_int err;
		const cl::Device &dev = mDevices->at(0);
		cl::CommandQueue commandQueue(context, dev, 0, &err);
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

		commandQueue.enqueueWriteBuffer(mPoints, CL_TRUE, 0, sizeof(float)*mPointCount, points.data());

		cl::Kernel &kernel = mKernels->at("iteration");
		commandQueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(mPointCount), cl::NDRange(256), NULL, NULL);
		commandQueue.finish();

		err = commandQueue.enqueueReadBuffer(mPoints, CL_TRUE, 0, sizeof(Point)*mPointCount, points.data());
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
