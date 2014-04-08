#ifndef OCL_POTENTIAL_FRAMEWORK_IMPLEMENTATION_HPP
#define OCL_POTENTIAL_FRAMEWORK_IMPLEMENTATION_HPP

#include "../interface.hpp"
#include "../data.hpp"
#include "../serial.hpp"


/*
 * Final implementation of the tested OCL program.
 */
class OCLProgram_Implementation : public OCLProgram
{
private:
	SerialSimulator *mSimulator;

public:
	OCLProgram_Implementation() : mSimulator(NULL) {}
	
	~OCLProgram_Implementation()
	{
		if (mSimulator != NULL)
			delete mSimulator;
	}


	virtual void initialize(index_t points, const std::vector<Edge>& edges, const std::vector<length_t> &lengths, index_t iterations)
	{
		mSimulator = new SerialSimulator(points, edges, lengths,
			this->mVertexRepulsion, this->mVertexMass, this->mEdgeCompulsion, this->mSlowdown, this->mTimeQuantum); 
	}


	virtual void iteration(std::vector<Point> &points)
	{
		mSimulator->updatePoints(points);
	}


	virtual void getVelocities(std::vector<Point> &velocities)
	{
		velocities = mSimulator->getVelocities();
	}
};


#endif
