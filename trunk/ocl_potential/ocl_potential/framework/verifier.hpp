#ifndef OCL_POTENTIAL_FRAMEWORK_VERIFIER_HPP
#define OCL_POTENTIAL_FRAMEWORK_VERIFIER_HPP

#include "data.hpp"
#include "serial.hpp"

#include <memory>
#include <vector>
#include <algorithm>
#include <cmath>


/**
 * \brief Base class for all result verifiers which also defines the interface.
 */
class Verifier
{
protected:
	// Verification Parameters
	real_t mTolerance;			///< Relative tolerance for float numbers.
	bool mVerbose;				///< Whether additional information should be reported.
	size_t mMaxReportError;		///< Maximal number of errors fully reported.

public:
	Verifier(real_t tolerance, bool verbose, size_t maxReportError)
		: mTolerance(std::fabs(tolerance)), mVerbose(verbose), mMaxReportError(maxReportError) {}
	virtual ~Verifier() {}

	/**
	 * \brief Initialize the verifier (saves data for serial verification).
	 */
	virtual void init(const Graph &graph, real_t vertexRepulsion, real_t vertexMass, real_t edgeCompulsion, real_t slowdown, real_t timeQuantum) {}


	/**
	 * \brief Check next iteration. Computes results serially and checks with given results.
	 * \param points Next version of point positions computed by the tested solution.
	 * \param velocities Next version of velocities computed by the tested solution.
	 * \return True if the results checked out, false otherwise. Note that any errors
	 *		are printed immediately, if in verbose mode.
	 */
	virtual bool check(const std::vector<Point> &points, std::vector<Point> &velocities)	{ return true; }


	/**
	 * \brief Print statistics accumulated by the verifier (e.g., greatest deviation from the correct results).
	 */
	virtual void printStats() const {}
};





/**
 * \brief Full version of the verifier that implements prescribed interface using serial simulation.
 */
class VerifierFull : public Verifier
{
public:
	/**
	 * \brief Compare two floats and return relative deviation.
	 * \param value1 One of the values being compared.
	 * \param value2 One of the values being compared.
	 * \return Relative deviation (e.g., value 0.05 means difference ~ 5% of absolute values).
	 */
	static double compare(real_t value1, real_t value2)
	{
		double v1 = (double)value1;
		double v2 = (double)value2;
		
		double len = std::fabs(v1 - v2);
		double greater = std::max<double>(std::fabs(v1), std::fabs(v2));

		if (greater < 0.000001) {
			// Cheating -- if the absolute values are too small, we take absolute difference.
			return len;
		}
		return len / greater;
	}



private:
	// Data computed by serial version.
	std::unique_ptr<SerialSimulator> mSerial;
	std::vector<Point> mPoints;

	// Statistics
	double mPointsMaxDeviation;
	double mPointsDeviationSum;
	size_t mPointsDeviationCount;

	double mVelocitiesMaxDeviation;
	double mVelocitiesDeviationSum;
	size_t mVelocitiesDeviationCount;


	/**
	 * \brief Compare two vectors of points or velocities item by item. The errors are directly
	 *		printed to stderr (if verbose is set) up to the max errors limit.
	 * \param vec1 Vector of coordinates to be compared.
	 * \param vec2 Vector of coordinates to be compared.
	 * \param vectorName Name of the compared vectors (so the errors can be reported properly).
	 * \param deviationMax Variable were the maximal deviation is kept (for statistics).
	 * \param deviationSum Variable were the sum of all deviations is accumulated (for statistics).
	 * \param deviationCount Variable where the number of sumed deviations is kept (for statistics).
	 * \param errors Number of encountered errors (incremented with each error).
	 */
	void compareVectors(const std::vector<Point>& vec1, const std::vector<Point>& vec2,
		const char *vectorName, double &deviationMax, double &deviationSum, size_t &deviationCount, size_t &errors)
	{
		if (vec1.size() != vec2.size())
			throw (bpp::RuntimeError() << "Error in vector verification. Vectors of different lengths ("
				<< vec1.size() << " and " << vec2.size() << ") were passed on for testing.");

		for (size_t i = 0; i < vec1.size(); ++i) {
			// Get deviations (and update statistics);
			double dx = compare(vec1[i].x, vec2[i].x);
			double dy = compare(vec1[i].y, vec2[i].y);
			double maxD = std::max<double>(dx, dy);
			deviationMax = std::max<double>(deviationMax, maxD);
			deviationSum += dx + dy;
			deviationCount += 2;

			if (maxD > mTolerance) {
				if (mVerbose && errors < mMaxReportError) {
					std::cerr << "Coordinates in " << vectorName << "[" << i << "] are out of tolerance. ("
						<< vec1[i].x << ", " << vec1[i].y << ") =/= (" << vec2[i].x << ", " << vec2[i].y << ")" << std::endl;
				}
				++errors;
			}
		}
	}


public:
	VerifierFull(real_t tolerance, bool verbose, size_t maxReportError) : Verifier(tolerance, verbose, maxReportError),
		mPointsMaxDeviation(0.0), mPointsDeviationSum(0.0), mPointsDeviationCount(0),
		mVelocitiesMaxDeviation(0.0), mVelocitiesDeviationSum(0.0), mVelocitiesDeviationCount(0)
	{}


	virtual void init(const Graph &graph, real_t vertexRepulsion, real_t vertexMass, real_t edgeCompulsion, real_t slowdown, real_t timeQuantum)
	{
		mSerial = std::unique_ptr<SerialSimulator>(new SerialSimulator(graph.pointCount(), graph.getEdges(), graph.getLengths(),
			vertexRepulsion, vertexMass, edgeCompulsion, slowdown, timeQuantum));
		mPoints.assign(graph.getPoints().begin(), graph.getPoints().end());
	}


	virtual bool check(const std::vector<Point> &points, std::vector<Point> &velocities)
	{
		mSerial->updatePoints(mPoints);

		size_t errors = 0;	// total error counter (one point out of tolerance ~ one error)
		compareVectors(mPoints, points, "points", mPointsMaxDeviation, mPointsDeviationSum, mPointsDeviationCount, errors);
		compareVectors(mSerial->getVelocities(), velocities, "velocities", mVelocitiesMaxDeviation, mVelocitiesDeviationSum, mVelocitiesDeviationCount, errors);

		// Check error results.
		if (errors > 0) {
			if (mVerbose)
				std::cerr << "Total " << errors << " errors found!" << std::endl;
			return false;
		}

		// Update verifier for the next iteration.
		mPoints = points;
		mSerial->swapVelocities(velocities);
		return true;
	}


	virtual void printStats() const
	{
		if (!mVerbose) return;

		std::cout << "Points max diff: " << mPointsMaxDeviation << ", avg diff: " << (mPointsDeviationSum / (double)mPointsDeviationCount) << std::endl;
		std::cout << "Velocities max diff: " << mVelocitiesMaxDeviation << ", avg diff: " << (mVelocitiesDeviationSum / (double)mVelocitiesDeviationCount) << std::endl;
	}
};

#endif
