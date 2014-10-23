#ifndef _IMPLEMENTATION_HPP
#define _IMPLEMENTATION_HPP

#include "interface.hpp"
#include "data.hpp"
#include "stopwatch.hpp"

#include <CL/cl.hpp>

#include <string>
#include <iostream>


/*
 * Final implementation of the tested OCL program.
 */
class OCLProgram_Implementation : public OCLProgram
{
public:
	OCLProgram_Implementation(const DataIn &dataIn, DataOut &dataOut) : OCLProgram(dataIn, dataOut)
	{
		// This is the best place to initialize mKernels member field.
		mKernels["vecMul"] = Source("vecMul.cl", true);
	}

	virtual int run(const cl::Context &context, const std::vector<cl::Device> &devices,
		std::map<std::string, cl::Kernel> &kernels, bool verbose)
	{
		/*
		 * Implementation of the main algorithm.
		 * Note that you have mDataIn and mDataOut member fields at your disposal.
		 */

		cl_int err;
		Stopwatch stopwatch(true);
		// Prepare data buffers ...
		cl::Buffer buf[] = { 
			cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int)*mDataIn.n, mDataIn.x),
			cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int)*mDataIn.n, mDataIn.y),
			cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_int)*mDataIn.n),
		};
		stopwatch.stop();
		if (verbose) {
			std::cout << "Creating buffers: " << stopwatch.getMiliseconds() << " ms" << std::endl;
		}

		// Set kernel arguments
		cl::Kernel &kernel = kernels["vecMul"];
		for (cl_uint i = 0; i < 3; ++i) {
			err = kernel.setArg(i, buf[i]);
			if (err != CL_SUCCESS) {
				std::cerr << "Can not assing argument " << i << " of the kernel! (" << err << ")" << std::endl;
				return 2;
			}
		}

		// Create command queue for the first available device
		cl::CommandQueue commandQueue(context, devices[0], 0, &err);
		if (err != CL_SUCCESS) {
			std::cerr << "Command queue can not be created! (" << err << ")" << std::endl;
			return 3;
		}

		// Run the kernel
		stopwatch.start();
		err = commandQueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(mDataIn.n), cl::NDRange(256), NULL, NULL);
		if (err != CL_SUCCESS) {
			std::cerr << "Kernel enqueue failed! (" << err << ")" << std::endl;
			return 4;
		}
		commandQueue.finish();
		stopwatch.stop();
		if (verbose) {
			std::cout << "Running kernel: " << stopwatch.getMiliseconds() << " ms" << std::endl;
		}

		// Read results from GPU device
		stopwatch.start();
		err = commandQueue.enqueueReadBuffer(buf[2], CL_TRUE, 0, sizeof(cl_int)*mDataIn.n, mDataOut.z);
		if (err != CL_SUCCESS) {
			std::cerr << "Reading buffer failed! (" << err << ")" << std::endl;
			return 5;
		}
		stopwatch.stop();
		if (verbose) {
			std::cout << "Retrieving results: " << stopwatch.getMiliseconds() << " ms" << std::endl;
		}

		return 0;
	}
};


#endif
