typedef struct Point
{
	float x, y;
} point;

void computeForces(__global point *points, __global point *forces, int n)
{
	for (size_t i = 0; i < n; ++i)
		forces[i].x = forces[i].y = 0;

	// Compute repulsive forces between all vertices.
	for (size_t i = 1; i < n; ++i) {
		for (size_t j = 0; j < i; ++j)
			addRepulsiveForce(points, i, j, forces);
	}

	// Compute compulsive forces of the edges.
	//for (size_t i = 0; i < mEdges.size(); ++i)
	//	addCompulsiveForce(points, mEdges[i].p1, mEdges[i].p2, mLengths[i], forces);
}

__kernel void iteration(__global point *points, __global point *velocities, __global point *forces, float timeQuantum)
{
	int n = get_global_size(0);
	int p = get_global_id(0);

	for (int i = 0; i < n; ++i)
		velocities[i] = points[i];

	for (size_t i = 0; i < n; ++i)
	{
		points[i].x += velocities[i].x * timeQuantum;
		points[i].y += velocities[i].y * timeQuantum;
	}
}