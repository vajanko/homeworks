#include<mpi.h>
#include<iostream>
#include<fstream>
#include<string>
#include<string.h>

#define ROOT 0

using namespace std;

int read_number(const string &file) {
	ifstream input(file);

	int number = 0;
	input >> number;
	input.close();
	
	return number;
}
bool file_exists(const string &file) {
	ifstream input(file);
	bool result = input.good();
	input.close();

	return result;
}

int start(int id, int gsize, const string &file) {
	
	bool exists = file_exists(file); 
	int msg = exists ? -1 : id;
	// contains a list of process id's where file doesn't exist
	int *msgs = new int[gsize];

	int err;
	err = MPI_Allgather(&msg, 1, MPI_INT, msgs, 1, MPI_INT, MPI_COMM_WORLD);
	if (err) return err;

	bool err_found = false;
	for (int i = 0; i < gsize; ++i) {
		if (msgs[i] >= 0) {
			if (id == ROOT) {
				if (!err_found)
					cout << "File '" << file << "' is missing on: ";
				cout << msgs[i] << " ";
			}

			err_found = true;
		}
	}
	delete[] msgs;

	if (err_found) {
		if (id == ROOT)
			cout << endl;
		// there was an error on any of the nodes
		// do not continue with calculation
		return 0;
	}

	int result = read_number(file);

	int sum = 0;
	err = MPI_Reduce(&result, &sum, 1, MPI_INT,MPI_SUM, ROOT, MPI_COMM_WORLD);
	if (err) return err;

	if (id == ROOT)
		cout << sum << endl;

	return 0;
}
int abort(int err) {
	MPI_Finalize();
	exit(err);
}

int main(int argc, char **argv) {
	// start mpi
	int err;
	err = MPI_Init(&argc, &argv);
	if (err) abort(err);

	// get current node id
	int id;
	err = MPI_Comm_rank(MPI_COMM_WORLD, &id);
	if (err) abort(err);

	int gsize;
	err = MPI_Comm_size(MPI_COMM_WORLD, &gsize);
	if (err) abort(err);

	// get input on the root node
	string file(1024, '\0');
	if (id == ROOT)
		cin >> file;
		//file = "/mnt/home/stud_12/du4/num.txt";

	err = MPI_Bcast(const_cast<char *>(file.data()), 256, MPI_INT, ROOT, MPI_COMM_WORLD);
	if (err) abort(err);

	err = start(id, gsize, file);
	if (err) abort(err);
	
	err = MPI_Finalize();
	return err;
}
