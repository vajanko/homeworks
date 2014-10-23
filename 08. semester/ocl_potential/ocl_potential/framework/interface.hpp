#ifndef OCL_POTENTIAL_FRAMEWORK_INTERFACE_HPP
#define OCL_POTENTIAL_FRAMEWORK_INTERFACE_HPP


#include "data.hpp"
#include "bpp/exception.hpp"

#include <CL/cl.hpp>

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>


/**
 * \brief Exception for reporting user exceptions form the implementation.
 *
 * This exception may be used by the students for debugging.
 */
class UserException : public bpp::RuntimeError
{
public:
	UserException() : RuntimeError() {}
	UserException(const char *msg) : RuntimeError(msg) {}
	UserException(const std::string &msg) : RuntimeError(msg) {}
	virtual ~UserException() throw() {}

	/*
	 * Overloading << operator that uses stringstream to append data to mMessage.
	 * Note that this overload is necessary so the operator returns object of exactly this class.
	 */
	template<typename T> UserException& operator<<(const T &data)
	{
		RuntimeError::operator<<(data);
		return *this;
	}
};



/*
 * Class for representing source code of a kernel.
 * Source code can be either input directly by a string or as a path to a file.
 */
class KernelSource
{
private:
	std::string mSource;
	
public:
	KernelSource() {}

	KernelSource(const char *str) : mSource(str) {}

	KernelSource(const char *str, bool fileName)
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
class OCLProgram
{
protected:
	/**
	 * \brief List of kernel sources required by the implementation.
	 * The derived class SHOULD fill this variable correctly in the constructor.
	 */
	std::map<std::string, KernelSource> mKernelSources;

	// OCL parameters
	const cl::Context *mContext;
	const std::vector<cl::Device> *mDevices;
	std::map<std::string, cl::Kernel> *mKernels;

	// Physical model parameters
	real_t mVertexRepulsion;
	real_t mVertexMass;
	real_t mEdgeCompulsion;
	real_t mSlowdown;
	real_t mTimeQuantum;

	bool mVerbose;	///< Whether verbose (debugging mode) is turned on.


public:
	/**
	 * \brief Provides an interface, through which the application announces the framework,
	 * which kernels it wishes to use.
	 *
	 * Key of the map is name of the kernel used for identification and in the source code.
	 * Value is the source code of the kernel. Either as a string or as a path to a file.
	 * Note that there must be exactly one function (with proper name) in the kernel file.
	 */
	const std::map<std::string, KernelSource>& getKernelList() const
	{
		return mKernelSources;
	}


	/*
	 * \brief Pass on references of initialized OCL entities to the implementation
			and the physical model parameters. This method is called prior to initialization
			and it SHOULD NOT be modified by the implementation.
	 * \param context The OCL context in which the application operates.
	 * \param devices List of available OCL devices (GPUs).
	 * \param kernels Compiled kernels that were provided as kernel source in mKernelSources.
	 * \param vertexRepulsion 
	 * \param vertexMass
	 * \param edgeCompulsion
	 * \param slowdown
	 * \param timeQuantum
	 * \param verbose
	 */
	void preinitialize(const cl::Context &context, const std::vector<cl::Device> &devices,
		std::map<std::string, cl::Kernel> &kernels, real_t vertexRepulsion, real_t vertexMass,
		real_t edgeCompulsion, real_t slowdown, real_t timeQuantum, bool verbose)
	{
		mContext = &context;
		mDevices = &devices;
		mKernels = &kernels;
		mVertexRepulsion = vertexRepulsion;
		mVertexMass = vertexMass;
		mEdgeCompulsion = edgeCompulsion;
		mSlowdown = slowdown;
		mTimeQuantum = timeQuantum;
		mVerbose = verbose;
	}


	/*
	 * \brief Initialization that is specific to the implementation.
	 * \param points Number of points in the graph.
	 * \param edges The edge data (how the points are interconnected). Note that edges are symmetric.
	 * \param lengths Lengts of the edges (i-th value corresponds to i-th edge).
	 * \param iterations How many iterations will be performed.
	 */
	virtual void initialize(index_t points, const std::vector<Edge>& edges,
		const std::vector<length_t> &lengths, index_t iterations) = 0;


	/*
	 * \brief Compute one iteration that update point values.
	 * \note It is guaranteed that points yielded by one iteration are
	 *		passed on to the following iteration.
	 * \param points Vector where the point values are stored.
	 */
	virtual void iteration(std::vector<Point> &points) = 0;


	/**
	 * \brief Return the internal velocities.
	 * \param velocities A vector where the results should be stored.
	 */
	virtual void getVelocities(std::vector<Point> &velocities) = 0;
};


#endif
