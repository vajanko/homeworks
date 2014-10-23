#ifndef OCL_POTENTIAL_FRAMEWORK_DATA_HPP
#define OCL_POTENTIAL_FRAMEWORK_DATA_HPP

#include "bpp/exception.hpp"

#include <vector>
#include <string>
#include <cstdint>
#include <cstdio>



/**
 * \brief Specific exception thrown when data loading/management fails.
 */
class DataException : public bpp::RuntimeError
{
public:
	DataException() : RuntimeError() {}
	DataException(const char *msg) : RuntimeError(msg) {}
	DataException(const std::string &msg) : RuntimeError(msg) {}
	virtual ~DataException() throw() {}

	/*
	 * Overloading << operator that uses stringstream to append data to mMessage.
	 * Note that this overload is necessary so the operator returns object of exactly this class.
	 */
	template<typename T> DataException& operator<<(const T &data)
	{
		RuntimeError::operator<<(data);
		return *this;
	}
};



/*
 * Typdefs for basic data types.
 */

typedef float real_t;			///< Type representing real number (either float or double).
typedef std::uint32_t index_t;	///< Type representing vertex (and other) array indices.
typedef std::uint32_t length_t;	///< Type representing the edge length value.


/**
 * \brief Structure representing 2D point with real coordinates.
 */
struct Point
{
	real_t x, y;
	Point() : x(), y() {}
	Point(real_t _x, real_t _y) : x(_x), y(_y) {}
};



/**
 * \brief Structure representing one edge (indices to adjacent vertices).
 */
struct Edge
{
	index_t p1, p2;
	Edge() {}
	Edge(index_t _p1, index_t _p2) : p1(_p1), p2(_p2) {}
};



/**
 * \brief Class that encapsulates all necessary data and their loading from a file.
 */
class Graph
{
private:
	std::vector<Point> mPoints;			///< List of points.
	std::vector<Edge> mEdges;			///< List of edges (indices to points array).
	std::vector<length_t> mLengths;		///< Length values (i-th value corresponds to i-th edge).

public:
	Graph() {}


	/**
	 * \brief Load all graph data from a file.
	 */
	void load(const std::string &fileName)
	{
		std::FILE *fp = std::fopen(fileName.c_str(), "rb");
		if (fp == NULL)
			throw (DataException() << "Unable to open file '" << fileName << "' for reading.");

		index_t sizes[2];
		if (std::fread(sizes, sizeof(index_t), 2, fp) != 2)
			throw (DataException() << "Unable to read header from '" << fileName << "' file.");

		mPoints.resize(sizes[0]);
		mEdges.resize(sizes[1]);
		mLengths.resize(sizes[1]);

		if (std::fread(&mPoints[0], sizeof(Point), mPoints.size(), fp) != mPoints.size())
			throw (DataException() << "Unable to read points from '" << fileName << "' file.");

		if (std::fread(&mEdges[0], sizeof(Edge), mEdges.size(), fp) != mEdges.size())
			throw (DataException() << "Unable to read edges from '" << fileName << "' file.");

		if (std::fread(&mLengths[0], sizeof(length_t), mLengths.size(), fp) != mLengths.size())
			throw (DataException() << "Unable to read lengths from '" << fileName << "' file.");

		std::fclose(fp);
	}


	/*
	 * Internal Data Accessors
	 */
	index_t pointCount() const						{ return (index_t)mPoints.size(); }
	index_t edgeCount() const						{ return (index_t)mEdges.size(); }

	const Point& getPoint(index_t i) const			{ return mPoints[i]; }
	Point& getPoint(index_t i)						{ return mPoints[i]; }
	const std::vector<Point>& getPoints() const		{ return mPoints; }
	std::vector<Point>& getPoints()					{ return mPoints; }

	const Edge& getEdge(index_t i) const			{ return mEdges[i]; }
	Edge& getEdge(index_t i)						{ return mEdges[i]; }
	const std::vector<Edge>& getEdges() const		{ return mEdges; }
	std::vector<Edge>& getEdges()					{ return mEdges; }

	length_t getLength(index_t i) const				{ return mLengths[i]; }
	const std::vector<length_t>& getLengths() const	{ return mLengths; }
	std::vector<length_t>& getLengths()				{ return mLengths; }
};

#endif
