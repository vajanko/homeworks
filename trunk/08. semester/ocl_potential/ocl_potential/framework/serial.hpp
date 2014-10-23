#ifndef OCL_POTENTIAL_FRAMEWORK_SERIAL_HPP
#define OCL_POTENTIAL_FRAMEWORK_SERIAL_HPP

#include "data.hpp"
#include "bpp/exception.hpp"
#include <vector>
#include <algorithm>
#include <cmath>


/**
 * \brief Serial implementation of the physical model.
 */
class SerialSimulator
{
private:
	typedef float in_real_t;

	const std::vector<Edge> &mEdges;		///< Reference to the graph edges.
	const std::vector<length_t> &mLengths;	///< Reference to the graph lengths of the edges.
	std::vector<Point> mVelocities;			///< Point velocity vectors.
	std::vector<Point> mForces;				///< Preallocated buffer for force vectors.

	// Model configuration
	real_t mVertexRepulsion;
	real_t mVertexMass;
	real_t mEdgeCompulsion;
	real_t mSlowdown;
	real_t mTimeQuantum;


	/**
	 * \brief Add repulsive force that affects selected points.
	 *		This function updates internal array mForces.
	 * \param points Current point coordinates.
	 * \param p1 One of the points for which the repulsive force is computed.
	 * \param p2 One of the points for which the repulsive force is computed.
	 * \param forces Vector where forces affecting points are being accumulated.
	 */
	void addRepulsiveForce(const std::vector<Point> &points, index_t p1, index_t p2, std::vector<Point> &forces)
	{
		in_real_t dx = (in_real_t)points[p1].x - (in_real_t)points[p2].x;
		in_real_t dy = (in_real_t)points[p1].y - (in_real_t)points[p2].y;
		in_real_t sqLen = std::max<real_t>(dx*dx + dy*dy, (in_real_t)0.0001);
		in_real_t fact = mVertexRepulsion / (sqLen * (real_t)std::sqrt(sqLen));	// mul factor
		dx *= fact;
		dy *= fact;
		forces[p1].x += dx;
		forces[p1].y += dy;
		forces[p2].x -= dx;
		forces[p2].y -= dy;
	}


	/**
	 * \brief Add compulsive force that affects selected points connected with an edge.
	 *		This function updates internal array mForces.
	 * \param points Current point coordinates.
	 * \param p1 One of the points adjacent to the edge.
	 * \param p2 One of the points adjacent to the edge.
	 * \param length Length of the edge.
	 * \param forces Vector where forces affecting points are being accumulated.
	 */
	void addCompulsiveForce(const std::vector<Point> &points, index_t p1, index_t p2, length_t length, std::vector<Point> &forces)
	{
		in_real_t dx = (in_real_t)points[p2].x - (in_real_t)points[p1].x;
		in_real_t dy = (in_real_t)points[p2].y - (in_real_t)points[p1].y;
		in_real_t sqLen = dx*dx + dy*dy;
		in_real_t fact = (in_real_t)std::sqrt(sqLen) * mEdgeCompulsion / (in_real_t)(length);
		dx *= fact;
		dy *= fact;
		forces[p1].x += dx;
		forces[p1].y += dy;
		forces[p2].x -= dx;
		forces[p2].y -= dy;
	}


	/**
	 * \brief Update velocities based on current forces affecting the points.
	 */
	void updateVelocities(const std::vector<Point> &forces)
	{
		real_t fact = mTimeQuantum / mVertexMass;	// v = Ft/m  => t/m is mul factor for F.
		for (size_t i = 0; i < mVelocities.size(); ++i) {
			mVelocities[i].x = (mVelocities[i].x + (real_t)forces[i].x * fact) * mSlowdown;
			mVelocities[i].y = (mVelocities[i].y + (real_t)forces[i].y * fact) * mSlowdown;
		}
	}




public:
	SerialSimulator(index_t pointCount, const std::vector<Edge> &edges, const std::vector<length_t> &lengths,
		real_t vertexRepulsion, real_t vertexMass, real_t edgeCompulsion, real_t slowdown, real_t timeQuantum)
		: mEdges(edges), mLengths(lengths), mVertexRepulsion(vertexRepulsion), mVertexMass(vertexMass),
		mEdgeCompulsion(edgeCompulsion), mSlowdown(slowdown), mTimeQuantum(timeQuantum)
	{
		mVelocities.resize(pointCount);
		mForces.resize(pointCount);
	}

	real_t vertexRepulsion() const	{ return mVertexRepulsion; }
	real_t vertexMass() const		{ return mVertexMass; }
	real_t edgeCompulsion() const	{ return mEdgeCompulsion; }
	real_t slowdown() const			{ return mSlowdown; }
	real_t timeQuantum() const		{ return mTimeQuantum; }


	/**
	 * \brief Reset velocities to zero.
	 */
	void resetVelocities()
	{
		for (size_t i = 0; i < mVelocities.size(); ++i)
			mVelocities[i].x = mVelocities[i].y = (real_t)0.0;
	}

	/**
	 * \brief Return current velocities.
	 */
	const std::vector<Point>& getVelocities() const		{ return mVelocities; }

	/**
	 * \brief Override internal velocities with velocities in given buffer.
	 * \param velocities The buffer with new velocities.
	 *		The buffer must have the same size as the internal buffer.
	 */
	void setVelocities(const std::vector<Point> &velocities)
	{
		mVelocities = velocities;
	}

	/**
	 * \brief Swap internal velocities buffer with given buffer.
	 * \param velocities The buffer with new velocities.
	 *		The buffer must have the same size as the internal buffer.
	 */
	void swapVelocities(std::vector<Point> &velocities)
	{
		if (mVelocities.size() != velocities.size())
			throw (bpp::RuntimeError() << "Cannot swap internal velocity buffer with a buffer of a different size."
			<< "Current model uses " << mVelocities.size() << " points, but the buffer has " << velocities.size() << " points.");
		mVelocities.swap(velocities);
	}


	void computeForces(std::vector<Point> &points, std::vector<Point> &forces)
	{
		forces.resize(points.size());

		// Clear forces array for another run.
		for (size_t i = 0; i < forces.size(); ++i) {
			forces[i].x = forces[i].y = (real_t)0.0;
		}

		// Compute repulsive forces between all vertices.
		for (size_t i = 1; i < forces.size(); ++i) {
			for (size_t j = 0; j < i; ++j)
				addRepulsiveForce(points, i, j, forces);
		}

		// Compute compulsive forces of the edges.
		for (size_t i = 0; i < mEdges.size(); ++i)
			addCompulsiveForce(points, mEdges[i].p1, mEdges[i].p2, mLengths[i], forces);
	}


	/**
	 * \brief The main method that computes another version of point positions.
	 * \param points Point positions that are updated by the function.
	 * \note The function updates internal array with velocities.
	 */
	void updatePoints(std::vector<Point> &points)
	{
		if (points.size() != mVelocities.size())
			throw (bpp::RuntimeError() << "Cannot compute next version of point positions."
				<< "Current model uses " << mVelocities.size() << " points, but the given buffer has " << points.size() << " points.");

		computeForces(points, mForces);
		updateVelocities(mForces);

		// Update point positions.
		for (size_t i = 0; i < mVelocities.size(); ++i) {
			points[i].x += mVelocities[i].x * mTimeQuantum;
			points[i].y += mVelocities[i].y * mTimeQuantum;
		}
	}
};

#endif
