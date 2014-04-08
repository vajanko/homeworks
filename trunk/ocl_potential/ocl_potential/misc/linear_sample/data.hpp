/*
 * Exercise-specific definitions of input and output data structures.
 * It also provide means to acquire input data and verify results.
 */
#ifndef _DATA_HPP
#define _DATA_HPP

#include "interface.hpp"


/*
 * Structure representing all input data.
 */
struct DataIn
{
	int n;
	cl_int *x, *y;

	/*
	 * Generate random values into both input vectors.
	 * The constructor recieves command line arguments (without program name
	 * and without arguments that belonged directly to the framework).
	 * Processed arguments should be consumed (so that argc is decremented and argv pointer advanced).
	 * Remaining arguments (that are not consumed) are passed to DataOut constructor.
	 */
	DataIn(int &argc, char **&argv)
	{
		if (argc > 0) {
			n = atoi(argv[0]);
			if (n < 1024) n = 1024;
			if (n > 16*1024*1024) n = 16*1024*1024;
		} else
			n = 16*1024*1024;

		x = (cl_int*)malloc(n * sizeof(cl_int));
		y = (cl_int*)malloc(n * sizeof(cl_int));

		for (int i = 0; i < n; ++i) {
			x[i] = (rand() % 32768 + 1);
			y[i] = (rand() % 32768 + 1);
		}
	}
};


/*
 * Structure representing all output data.
 */
struct DataOut
{
	int n;
	cl_int *z;

	/*
	 * Prepare data structures for computed results.
	 * The constructor recieves command line arguments (without program name
	 * and without arguments that belonged directly to the framework).
	 * Processed arguments should be consumed (so that argc is decremented and argv pointer advanced).
	 * Remaining arguments (that are not consumed) are passed to DataOut constructor.
	 */
	DataOut(int &argc, char **&argv)
	{
		if (argc > 0) {
			n = atoi(argv[0]);
			if (n < 1024) n = 1024;
			if (n > 16*1024*1024) n = 16*1024*1024;
		} else
			n = 16*1024*1024;

		z = (cl_int*)malloc(n * sizeof(cl_int));
	}


	/*
	 * Check whether the results has been computed corectly.
	 */
	bool checkResult(const DataIn &dataIn)
	{
		for (int i = 0; i < n; ++i) {
			if (z[i] != dataIn.x[i] * dataIn.y[i])
				return false;
		}
		return true;
	}
};


/*
 * Program prototype class that contains also input and output data structures.
 * The implementation SHOULD be inherited directly from this class.
 */
class OCLProgram : public OCLProgram_Interface
{
protected:
	const DataIn &mDataIn;
	DataOut &mDataOut;

public:
	OCLProgram(const DataIn &dataIn, DataOut &dataOut) : OCLProgram_Interface(), mDataIn(dataIn), mDataOut(dataOut) {}
};

#endif
