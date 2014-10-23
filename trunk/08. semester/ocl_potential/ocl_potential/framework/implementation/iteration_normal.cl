typedef struct Point
{
	float x, y;
} point;
//typedef float2 point;
typedef struct Edge
{
	uint p1, p2, len;
} edge;

void addRepulsiveForce(__global const point *points, __global point *forces, float vertexRepulsion)
{
	int gid = get_global_id(0);
	int gsz = get_global_size(0);

	// Clear forces array for another run.
	forces[gid].x = forces[gid].y = 0;

	for (size_t j = 0; j < gid; ++j)
	{
		float dx = points[gid].x - points[j].x;
		float dy = points[gid].y - points[j].y;
		float sqLen = max(dx * dx + dy * dy, 0.0001f);
		float fact = vertexRepulsion / (sqLen * sqrt(sqLen));	// mul factor
		dx *= fact;
		dy *= fact;
		forces[gid].x += dx;
		forces[gid].y += dy;
	}
	for (size_t j = gid; j < gsz; ++j)
	{
		float dx = points[j].x - points[gid].x;
		float dy = points[j].y - points[gid].y;
		float sqLen = max(dx * dx + dy * dy, 0.0001f);
		float fact = vertexRepulsion / (sqLen * sqrt(sqLen));	// mul factor
		dx *= fact;
		dy *= fact;
		forces[gid].x -= dx;
		forces[gid].y -= dy;
	}
}
/*void addCompulsiveForce(__global const point *points, edge e,  __global point *forces, float edgeCompulsion)
{
	float dx = points[e.p2].x - points[e.p1].x;
	float dy = points[e.p2].y - points[e.p1].y;
	float sqLen = dx*dx + dy*dy;
	float fact = sqrt(sqLen) * edgeCompulsion / (float)(e.len);
	dx *= fact;
	dy *= fact;
	
	forces[e.p1].x += dx;
	forces[e.p1].y += dy;
	forces[e.p2].x -= dx;
	forces[e.p2].y -= dy;
}*/
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
			float dx = points[e.p2].x - points[gid].x;
			float dy = points[e.p2].y - points[gid].y;
			float sqLen = dx*dx + dy*dy;
			float fact = sqrt(sqLen) * edgeCompulsion / (float)(e.len);
			dx *= fact;
			dy *= fact;

			forces[gid].x += dx;
			forces[gid].y += dy;
		}
		if (e.p2 == gid)
		{
			float dx = points[gid].x - points[e.p1].x;
			float dy = points[gid].y - points[e.p1].y;
			float sqLen = dx*dx + dy*dy;
			float fact = sqrt(sqLen) * edgeCompulsion / (float)(e.len);
			dx *= fact;
			dy *= fact;

			forces[gid].x -= dx;
			forces[gid].y -= dy;
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
	velocities[gid].x = (velocities[gid].x + forces[gid].x * fact) * slowdown;
	velocities[gid].y = (velocities[gid].y + forces[gid].y * fact) * slowdown;

	// update points
	points[gid].x += velocities[gid].x * timeQuantum;
	points[gid].y += velocities[gid].y * timeQuantum;
}