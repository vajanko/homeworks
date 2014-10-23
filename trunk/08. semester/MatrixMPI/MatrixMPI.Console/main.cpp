#include "..\MatrixMPI\matrix.hpp"
#include <fstream>

using namespace std;

int main(int argc, char **argv)
{
	size_t rows, cols;
	ifstream file("C:\\tmp\\m1.dat");
	matrix_read_size(file, rows, cols);

	matrix m = matrix_alloc(rows, cols);
	
	
	matrix_load(m, file, 0, 0, rows, cols, cols);
	matrix_print(m, rows, cols);

	return 0;
}
