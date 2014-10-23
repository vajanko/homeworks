/*
 * Main file of the testing framework.
 * It sets up OpenCL environment, prepare data, and executes tested algorithm while measuring its time.
 */
#define _CRT_SECURE_NO_WARNINGS

// internal files
#include "implementation/implementation.hpp"
#include "verifier.hpp"
#include "interface.hpp"
#include "serial.hpp"
#include "data.hpp"

#include "bpp/stopwatch.hpp"
#include "bpp/args.hpp"

// libraries
#include <CL/cl.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>



/**
 * \brief Extract program binary after OpenCL kompiles it and save it to a file.
 *		For instance, NVIDIA uses PTX intermediate code for kernel binaries,
 *		which is human readable.
 * \param program Compiled OpenCL program.
 * \param fileName Path where the binary will be saved.
 */
void save_program(cl::Program &program, const char *fileName)
{
	std::vector<size_t> sizes = program.getInfo<CL_PROGRAM_BINARY_SIZES>();
	std::vector<char*> binaries = program.getInfo<CL_PROGRAM_BINARIES>();
	if (binaries.empty())
		throw bpp::RuntimeError("No kernel binary found.");

	std::FILE *fp = fopen(fileName, "w");
	if (fp == NULL)
		throw (bpp::RuntimeError() << "Unable to open '" << fileName << "' file for writing.");
	fwrite(binaries[0], sizeof(char), sizes[0], fp);
	fclose(fp);
}



/*
 * Application Entry Point
 */
int main(int argc, char **argv)
{
	using namespace bpp;

	ProgramArguments args(1, 1);
	try {
		args.setNamelessCaption(0, "Path to binary graph file to be processed.");

		// General parameters
		args.registerArg(new ProgramArguments::ArgBool("verify", "Results of each iteration are verified by a serial algorithm."));
		args.registerArg(new ProgramArguments::ArgBool("verbose", "Print out debugging messages."));
		args.registerArg(new ProgramArguments::ArgInt("iterations", "Number of iterations performed.", false, 100, 1));

		// Physical model parameters.
		args.registerArg(new ProgramArguments::ArgFloat("vertex_repulsion", "Vertex repulsion force constant.", false, 0.1, 0.00001, 1000000000.0));
		args.registerArg(new ProgramArguments::ArgFloat("vertex_mass", "Vertex mass constant (affects the vertex momentum).", false, 1.0, 0.00001, 1000000000.0));
		args.registerArg(new ProgramArguments::ArgFloat("edge_compulsion", "Edge compulsion force constant.", false, 20.0, 0.00001, 1000000000.0));
		args.registerArg(new ProgramArguments::ArgFloat("slowdown", "Vertex velocity modificator per iteration (1.0 = no slowdown, 0.0 = no motion).", false, 0.995, 0.0, 1.0));
		args.registerArg(new ProgramArguments::ArgFloat("time_quantum", "Time quantum simulated by each iteration.", false, 0.001, 0.0, 1000000.0));

		// Other parameters
		args.registerArg(new ProgramArguments::ArgString("save_bin", "Save the kernel binary (or intermediate code) into a file."));

		args.process(argc, argv);
	}
	catch (ArgumentException &e) {
		std::cerr << "Argument error: " << e.what() << std::endl;
		args.printUsage(std::cerr);
		return 1;
	}
	bool verbose = args.getArgBool("verbose").getValue();


	/*
	 * Initialize OpenCL ..
	 */
	cl_int err;

	if (verbose) {
		std::cout << "OpenCL initialization ... ";
		std::cout.flush();
	}

	// Acquire First Available Platform
    std::vector<cl::Platform> platforms;
    err = cl::Platform::get(&platforms);
    if(err != CL_SUCCESS) {
        std::cerr << "Platform::get() failed (" << err << ")" << std::endl;
		return 2;
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
		return 3;
	}

	// Get List of Context Devices
	std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>(&err);
	if (err != CL_SUCCESS) {
		std::cerr << "Unable to get list of context devices! (" << err << ")" << std::endl;
		return 4;
	}

	if (devices.size() == 0) {
		std::cout << "There are no OpenCL devices." << std::endl;
		return 0;
	}

	if (verbose)
		std::cout << devices.size() << " GPU devices found." << std::endl;

	/*
	 * Prepare data and fetch tested program implementation.
	 */
	if (verbose)
		std::cout << "Loading data from file '" << args[0] << "' ..." << std::endl;
	Graph graph;
	graph.load(args[0]);
	if (verbose)
		std::cout << "Data loaded (" << graph.pointCount() << " points and " << graph.edgeCount() << " edges)." << std::endl;

	

	/*
	 * Prepare program instance, load and compile kernels.
	 */
	OCLProgram *program = new OCLProgram_Implementation;
	const std::map<std::string, KernelSource> &kernelList = program->getKernelList();

	if (verbose)
		std::cout << "Loading kernel sources ..." << std::endl;
	
	cl::Program::Sources programSources;
	std::vector<std::string> programSourcesList;
	std::map<std::string, cl::Kernel> kernels;

	if (!kernelList.empty()) {
		for (auto it = kernelList.begin(); it != kernelList.end(); ++it) {
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

			system("pause");
			return 5;
		}

		// Fetch compiled kernel objects.
		for (auto it = kernelList.begin(); it != kernelList.end(); ++it) {
			kernels[it->first] = cl::Kernel(kernelsProgram, it->first.c_str(), &err);
			if (err != CL_SUCCESS) {
				std::cerr << "Unable to acquire reference to '" << it->first << "' kernel! (" << err << ")" << std::endl;
				system("pause");
				return 6;
			}
			if (verbose)
				std::cout << "Kernel '" << it->first << "' sucessfully compiled." << std::endl;
		}

		if (args.getArgString("save_bin").isPresent())
			save_program(kernelsProgram, args.getArgString("save_bin").getValue().c_str());
	}

	// Set all necessary parameters of the program.
	program->preinitialize(context, devices, kernels,
		(real_t)args.getArgFloat("vertex_repulsion").getValue(),
		(real_t)args.getArgFloat("vertex_mass").getValue(),
		(real_t)args.getArgFloat("edge_compulsion").getValue(),
		(real_t)args.getArgFloat("slowdown").getValue(),
		(real_t)args.getArgFloat("time_quantum").getValue(),
		verbose);


	/*
	 * Solution Initialization
	 */
	index_t iterations = (index_t)args.getArgInt("iterations").getValue();

	if (verbose)
		std::cout << "Solution initialization ... " << std::endl;

	try {
		bpp::Stopwatch stopwatch(true);
		program->initialize(graph.pointCount(), graph.getEdges(), graph.getLengths(), iterations);
		stopwatch.stop();
		if (verbose)
			std::cout << "Elapsed time: " << stopwatch.getMiliseconds() << " ms" << std::endl;
		else
			std::cout << stopwatch.getMiliseconds() << std::endl;
	}
	catch (UserException &e) {
		std::cerr << "User Exception caught in initialization: " << e.what() << std::endl;
		system("pause");
		return 10;
	}
	
	
	/*
	 * Run Simulation
	 */
	double totalTime = 0.0;
	bool verify = args.getArgBool("verify").getValue();
	real_t tolerance = (real_t)0.05;
	Verifier *verifier = (verify)
		? (new VerifierFull(tolerance, verbose, 10))
		: (new Verifier(tolerance, verbose, 10));
	verifier->init(graph,
		(real_t)args.getArgFloat("vertex_repulsion").getValue(),
		(real_t)args.getArgFloat("vertex_mass").getValue(),
		(real_t)args.getArgFloat("edge_compulsion").getValue(),
		(real_t)args.getArgFloat("slowdown").getValue(),
		(real_t)args.getArgFloat("time_quantum").getValue());
	std::vector<Point> velocities;
	
	for (index_t iter = 0; iter < iterations; ++iter) {
		try {
			// Run the solution ...
			bpp::Stopwatch stopwatch(true);
			program->iteration(graph.getPoints());
			stopwatch.stop();
			totalTime += stopwatch.getMiliseconds();
			
			if (!verify) continue;

			// Perform verificaton ...
			program->getVelocities(velocities);
			if (!verifier->check(graph.getPoints(), velocities)) {
				if (verbose)
					std::cout << "Result verification failed in " << iter << "-th iteration" << std::endl;
				else
					std::cout << "FAILED" << std::endl;

				system("pause");
				return 20;
			}
		}
		catch (UserException &e) {
			std::cerr << "User Exception caught in iteration [" << iter << "]: " << e.what() << std::endl;
			system("pause");
			return 11;
		}
	}

	// Print total time.
	if (verbose)
		std::cout << "Time per iteration: ";
	std::cout << totalTime / (double)iterations << ((verbose) ? " ms" : "") << std::endl;

	if (verbose)
		verifier->printStats();
	delete verifier;

	system("pause");
	return 0;
}
