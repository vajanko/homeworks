#ifndef common_hdr
#define common_hdr

#include<iostream>
#include<memory>
#include<fstream>
#include<vector>
#include<sstream>
#include<string>
#include <stdexcept>

template<typename T>
T lexical_cast(const std::string& x)
{
	std::istringstream stream(x);
	T res;
	stream >> res;
	return res;
}

void usage()
{
	std::cout << "Usage:" << std::endl << "kmeans <data_file> <means_file> <clusters_file> <k> <iterations>" << std::endl << "kmeans --generate <data_file> <size> <seed>" << std::endl;
}

template< typename F>
void measure(F f)
{
	tick_count t0 = tick_count::now();

	f();

	tick_count t1 = tick_count::now();
	printf("time for action = %g seconds\n", (t1 - t0).seconds());
}
float *alinged_aloc(size_t size, size_t alignment)
{
	size_t real_size = size + 4;
	float *mem = new float[real_size];
	void *ptr = (void *)mem;
	float *aligned_mem = (float*)std::align(alignment, size, ptr, real_size);

	return aligned_mem;
}

bool compare(const std::string& lFilePath, const std::string& rFilePath)
{
	const size_t BUFFER_SIZE = 1024;

	std::ifstream lFile(lFilePath.c_str(), std::ifstream::in | std::ifstream::binary);
	std::ifstream rFile(rFilePath.c_str(), std::ifstream::in | std::ifstream::binary);

	if (!lFile.is_open() || !rFile.is_open())
	{
		return false;
	}

	char *lBuffer = new char[BUFFER_SIZE]();
	char *rBuffer = new char[BUFFER_SIZE]();

	do {
		lFile.read(lBuffer, BUFFER_SIZE);
		rFile.read(rBuffer, BUFFER_SIZE);
		auto numberOfRead = lFile.gcount();//I check the files with the same size

		if (std::memcmp(lBuffer, rBuffer, numberOfRead) != 0)
		{
			memset(lBuffer, 0, numberOfRead);
			memset(rBuffer, 0, numberOfRead);
			return false;
		}
	} while (lFile.good() || rFile.good());

	delete[] lBuffer;
	delete[] rBuffer;
	return true;
}

void generate(std::size_t count, unsigned int seed, std::vector<unsigned long long> &data)
{
	srand(seed);
	data.reserve(count);
	while (count--)
	{
		data.push_back(rand());
		//data.push_back(point(rand(), rand()));

	}
}
void save(const std::string& file_name, const std::vector<unsigned long long> &data)
{
	FILE* f;
	fopen_s(&f, file_name.c_str(), "wb");
	//FILE* f = fopen(file_name.c_str(),"wb");
	if (!f) throw std::runtime_error("cannot open file for writing");
	for (auto value : data)
	{
		fwrite(&value, sizeof(value), 1, f);
	}
	/*for (data_t::const_iterator it = data.begin(); it != data.end(); ++it)
	{
		if (!fwrite(&it->x, sizeof(value_t), 1, f)) throw std::runtime_error("value cannot be written");
		if (!fwrite(&it->y, sizeof(value_t), 1, f)) throw std::runtime_error("value cannot be written");
	}*/
	if (fclose(f)) throw std::runtime_error("closing the file failed");
}

int generate_file(char **argv)
{
	std::string option(argv[1]);
	if (option != "--generate") { usage(); return 1; }
	std::string file_name(argv[2]);
	std::string s_size(argv[3]);
	std::string s_seed(argv[4]);
	std::size_t size = lexical_cast<std::size_t>(s_size);
	unsigned int seed = lexical_cast<unsigned int>(s_seed);
	//serial1::data_t data;
	std::vector<unsigned long long> data;
	generate(size * 2, seed, data);
	save(file_name, data);

	return 0;
}

#endif