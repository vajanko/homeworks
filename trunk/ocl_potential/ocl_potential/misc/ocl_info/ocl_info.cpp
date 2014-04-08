/**
 * \file
 * Multi-threaded (OpenCL) version of matrix multiplication.
 */
#include <CL/cl.hpp>

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>


const cl_device_type deviceType = CL_DEVICE_TYPE_ALL;


int main(int argc, char **argv)
{
	/*
	 * Initialize OpenCL ..
	 */
	cl_int err;

    std::vector<cl::Platform> platforms;
    err = cl::Platform::get(&platforms);

	if(err != CL_SUCCESS)
    {
        std::cerr << "Platform::get() failed (" << err << ")" << std::endl;
		return 1;
    }

	// List all the platforms
	for (size_t pIdx = 0; pIdx < platforms.size(); ++pIdx) {
		std::string platformName;
		platforms[pIdx].getInfo(CL_PLATFORM_NAME, &platformName);
		
		std::string platformVersion;
		platforms[pIdx].getInfo(CL_PLATFORM_VERSION, &platformVersion);

		std::cout << "Platform #" << pIdx << ": " << platformName << " (" << platformVersion << ")" << std::endl;
		std::cout << "----------" << std::endl;

		std::vector<cl::Device> devices;
		err = platforms[0].getDevices(CL_DEVICE_TYPE_ALL, &devices);

		if (err != CL_SUCCESS || (devices.size() == 0)) {
			std::cerr << "Unable to get list of context devices!";
			return 2;
		}

		/*
		 * List devices info ...
		 */
		std::cout << "Total " << devices.size() << " devices found." << std::endl;

		for (unsigned i = 0; i < devices.size(); ++i) {
			std::string deviceName;
			devices[i].getInfo(CL_DEVICE_NAME, &deviceName);
			std::cout << "[Device #" << i << "]: " << deviceName << std::endl;

			cl_device_type type;
			devices[i].getInfo(CL_DEVICE_TYPE, &type);
			std::cout << "\tDevice type: " << type << std::endl;

			cl_uint units;
			devices[i].getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &units);
			std::cout << "\tMax. compute units: " << units << std::endl;

			cl_uint freq;
			devices[i].getInfo(CL_DEVICE_MAX_CLOCK_FREQUENCY, &freq);
			std::cout << "\tMax. clock frequency: " << freq << std::endl;

			cl_ulong memSize;
			devices[i].getInfo(CL_DEVICE_GLOBAL_MEM_SIZE, &memSize);
			std::cout << "\tGlobal mem size: " << memSize << std::endl;
			devices[i].getInfo(CL_DEVICE_LOCAL_MEM_SIZE, &memSize);
			std::cout << "\tLocal mem size: " << memSize << std::endl;

			cl_ulong maxAlloc;
			devices[i].getInfo(CL_DEVICE_MAX_MEM_ALLOC_SIZE, &maxAlloc);
			std::cout << "\tMax. alloc size: " << maxAlloc << std::endl;

			cl_uint dim;
			devices[i].getInfo(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, &dim);
			std::cout << "\tMax. work dimensions: " << dim << std::endl;

			size_t sizes[3];
			devices[i].getInfo(CL_DEVICE_MAX_WORK_ITEM_SIZES, &sizes);
			std::cout << "\tMax. work item sizes:";
			for (unsigned d = 0; d < dim; ++d) std::cout << " " << sizes[d];
			std::cout << std::endl;

			size_t gsize;
			devices[i].getInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE, &gsize);
			std::cout << "\tMax. work group size: " << gsize << std::endl;

			std::cout << std::endl;
		}
	}

	system("pause");
	return 0;
}

