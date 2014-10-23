__kernel void vecMul(__global const int *x, __global const int *y, __global int *z)
{
	int i = get_global_id(0);
	z[i] = x[i] * y[i];
}
