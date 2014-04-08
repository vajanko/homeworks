#ifndef _INTERFACE_HPP
#define _INTERFACE_HPP

#include <CL/cl.hpp>

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>


/*
 * Class for representing source code of a kernel.
 * Source code can be either input directly by a string or as a path to a file.
 */
class Source
{
private:
	std::string mSource;
	
public:
	Source() {}

	Source(const char *str) : mSource(str) {}

	Source(const char *str, bool fileName)
	{
		std::ifstream file;
		file.open(str, std::ios_base::in);
		if (file.fail()) {
			std::cerr << "Unable to open file '" << str << "'!" << std::endl;
			return;
		}
		
		std::string buf;
		while (!file.eof() && !file.fail()) {
			std::getline(file, buf);
			mSource.append(buf);
			mSource.append("\n");
		}
	}

	const std::string& getSource() const
	{
		return mSource;
	}
};


/*
 * Abstract class for all tested OCL applications.
 * The application provide list of kernels 
 */
class OCLProgram_Interface
{
protected:
	std::map<std::string, Source> mKernels;

public:
	/*
	 * Provides an interface, through which the application announces the framework,
	 * which kernels it wishes to use.
	 * Key of the map is name of the kernel used for identification and in the source code.
	 * Value is the source code of the kernel. Either as a string or as a path to a file.
	 * Note that there must be exactly one function (with proper name) in the kernel file.
	 */
	const std::map<std::string, Source>& getKernelList() const
	{
		return mKernels;
	}


	/*
	 * Main method of the object.
	 * This method is executed by the framework while time is measured.
	 * Working context is provided (so the buffers can be created), available devices are provided
	 * (so the command queues can be attached) and map of compiled kernel objects is provided.
	 * Kernel objects are identified by their names (provided by getKernelList).
	 * The verbose flag indicates, whether the program runs in verbose mode
	 * (i.e., you may write out some debugging outputs).
	 * If the main method terminates successfully, it SHOULD return 0. Otherwise, it may return
	 * user-specific error code.
	 */
	virtual int run(const cl::Context &context, const std::vector<cl::Device> &devices,
		std::map<std::string, cl::Kernel> &kernels, bool verbose) = 0;
};


#endif
