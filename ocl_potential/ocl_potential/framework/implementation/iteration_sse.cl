
typedef float2 point;
typedef struct Edge
{
	uint p1, p2, len;
} edge;

void addRepulsiveForce(__global const point *points, __global point *forces, float vertexRepulsion)
{
	int gid = get_global_id(0);
	int gsz = get_global_size(0);

	// Clear forces array for another run.
	forces[gid] = 0;

	for (size_t j = 0; j < gid; ++j)
	{
		point dx = points[gid] - points[j];
		dx *= dx;
		float sqLen = max(dx.x + dx.y, 0.0001f);
		float fact = vertexRepulsion / (sqLen * sqrt(sqLen));	// mul factor
		dx *= fact;
		forces[gid] += dx;
	}
	for (size_t j = gid; j < gsz; ++j)
	{
		point dx = points[j] - points[gid];
		dx *= dx;
		float sqLen = max(dx.x + dx.y, 0.0001f);
		float fact = vertexRepulsion / (sqLen * sqrt(sqLen));	// mul factor
		dx *= fact;
		forces[gid] -= dx;
	}
}
void computeForces(__global point *points, __global point *forces, __global const edge *edges, uint edgeCount,
	float vertexRepulsion, float vertexMass, float edgeCompulsion, float slowdown, float timeQuantum)
{
	int gid = get_global_id(0);
	int gsz = get_global_size(0);

	// Compute repulsive forces between all vertices.
	addRepulsiveForce(points, forces, vertexRepulsion);

	// Compute compulsive forces of the edges.
	for (size_t i = 0; i < edgeCount; ++i)
	{
		edge e = edges[i];
		if (e.p1 == gid)
		{
			point dx = points[e.p2] - points[gid];
			dx *= dx;
			float sqLen = dx.x + dx.y;
			float fact = sqrt(sqLen) * edgeCompulsion / (float)(e.len);
			dx *= fact;

			forces[gid] += dx;
		}
		if (e.p2 == gid)
		{
			point dx = points[gid] - points[e.p1];
			dx *= dx;
			float sqLen = dx.x + dx.y;
			float fact = sqrt(sqLen) * edgeCompulsion / (float)(e.len);
			dx *= fact;

			forces[gid] -= dx;
		}
	}

	//size_t step = edgeCount / gsz;
	//for (size_t i = 0; i < step; ++i)
	//	addCompulsiveForce(points, edges[gid * step + i], forces, edgeCompulsion);
}

__kernel void iteration(__global point *points, __global point *velocities, __global point *forces, __global const edge *edges, 
	uint edgeCount,
	float vertexRepulsion, float vertexMass, float edgeCompulsion, float slowdown, float timeQuantum)
{
	int gid = get_global_id(0);

	// compute forces
	computeForces(points, forces, edges, edgeCount, vertexRepulsion, vertexMass, edgeCompulsion, slowdown, timeQuantum);

	// update velocities
	float fact = timeQuantum / vertexMass;		// v = Ft/m  => t/m is mul factor for F
	velocities[gid] = (velocities[gid] + forces[gid] * fact) * slowdown;

	// update points
	points[gid] += velocities[gid] * timeQuantum;
}