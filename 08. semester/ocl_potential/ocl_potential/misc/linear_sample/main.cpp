/*
 * Main file of the testing framework.
 * It sets up OpenCL environment, prepare data, and executes tested algorithm while measuring its time.
 */
#define _CRT_SECURE_NO_WARNINGS

// external files that are supposed to be added ...
#include <implementation.hpp>
#include <data.hpp>

// interface specifications and supporting classes
#include "interface.hpp"
#include "stopwatch.hpp"

// libraries
#include <CL/cl.hpp>

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>


/*
 * Application Entry Point
 * The application may recieve "-v" as verbose=on switch as first argument.
 * Remaining arguments are directly passed to DataIn class, so it may generate data, load files etc.
 * Arguments that are not processed even by DataIn are passed to DataOut class.
 */
int main(int argc, char **argv)
{
	// Skip program name ...
	++argv; --argc;

	// Check for verbose switch
	bool verbose = false;
	if (argc > 0 && std::string(argv[0]) == std::string("-v")) {
		++argv; --argc;
		verbose = true;
	}


	/*
	 * Initialize OpenCL ..
	 */
	cl_int err;

	// Acquire First Available Platform
    std::vector<cl::Platform> platforms;
    err = cl::Platform::get(&platforms);
    if(err != CL_SUCCESS)
    {
        std::cerr << "Platform::get() failed (" << err << ")" << std::endl;
		return 1;
    }

	if (platforms.size() == 0) {
		std::cout << "There are no OpenCL platforms available" << std::endl;
		return 0;
	}

	// Create Working Context for All OCL Devices
	cl_context_properties cps[] = {
		CL_CONTEXT_PLATFORM,
		(cl_context_properties)(platforms[0]()),
		0	// NULL terminated list
	};
	cl::Context context(CL_DEVICE_TYPE_GPU, cps, NULL, NULL, &err);
	if (err != CL_SUCCESS) {
		std::cerr << "Creation of context failed! (" << err << ")" << std::endl;
		return 2;
	}

	// Get List of Context Devices
	std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>(&err);
	if (err != CL_SUCCESS) {
		std::cerr << "Unable to get list of context devices! (" << err << ")" << std::endl;
		return 3;
	}

	if (devices.size() == 0) {
		std::cout << "There are no OpenCL devices." << std::endl;
		return 0;
	}


	/*
	 * Prepare data and fetch tested program implementation.
	 */
	if (verbose)
		std::cout << "Preparing data ..." << std::endl;
	DataIn dataIn(argc, argv);
	DataOut dataOut(argc, argv);

	OCLProgram *program = new OCLProgram_Implementation(dataIn, dataOut);
	const std::map<std::string, Source> &kernelList = program->getKernelList();
	

	/*
	 * Load and compile kernels.
	 */
	if (verbose)
		std::cout << "Loading sources ..." << std::endl;
	cl::Program::Sources programSources;
	std::vector<std::string> programSourcesList;
	for (std::map<std::string, Source>::const_iterator it = kernelList.begin(); it != kernelList.end(); ++it)
	{
		programSources.push_back(std::make_pair(it->second.getSource().c_str(), it->second.getSource().length()));
	}

	// Build program
	if (verbose)
		std::cout << "Compiling kernels ..." << std::endl;
	cl::Program kernelsProgram(context, programSources);
	err = kernelsProgram.build(devices);
	if (err != CL_SUCCESS) {
		// Show error log...
		std::cerr << "Compilation FAILED!" << std::endl << "Build log:" << std::endl;

		// Note that we acquire log only for the first device (since we expect all devices to be the same).
		std::string log;
		kernelsProgram.getBuildInfo(devices[0], CL_PROGRAM_BUILD_LOG, &log);
		std::cerr << log << std::endl << std::endl;

		return 4;
	}

	// Fetch kernels
	std::map<std::string, cl::Kernel> kernels;
	for (std::map<std::string, Source>::const_iterator it = kernelList.begin(); it != kernelList.end(); ++it)
	{
		kernels[it->first] = cl::Kernel(kernelsProgram, it->first.c_str(), &err);
		if (err != CL_SUCCESS) {
			std::cerr << "Unable to acquire reference to '" << it->first << "' kernel! (" << err << ")" << std::endl;
			return 5;
		}
	}


	/*
	 * Run Tested Program
	 */
	
	// Start time measuring
	if (verbose)
		std::cout << "Running main program ..." << std::endl;
	Stopwatch stopwatch(true);

	err = program->run(context, devices, kernels, verbose);
	if (err != 0) {
		std::cerr << "Program terminated with user error " << err << std::endl;
		return 6;
	}

	// Stop time measuring
	stopwatch.stop();
	if (verbose)
		std::cout << "Elapsed time: " << stopwatch.getMiliseconds() << " ms" << std::endl;

	/*
	 * Results Verification
	 */
	if (verbose)
		std::cout << "Verify results ... ";
	if (!dataOut.checkResult(dataIn)) {
		std::cout << "FAILED!" << std::endl;
		return 7;
	}

	if (verbose)
		std::cout << "OK" << std::endl;
	else
		std::cout << stopwatch.getMiliseconds() << std::endl;

	return 0;
}
