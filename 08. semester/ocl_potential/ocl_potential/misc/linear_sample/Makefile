NAME=./linear_sample
CUDA_ROOT=/usr/local/cuda
CUDA_INC=${CUDA_ROOT}/include
CUDA_LIB=${CUDA_ROOT}/lib64

.PHONY: all clear clean

all: $(NAME)

$(NAME): data.hpp interface.hpp implementation.hpp stopwatch.hpp main.cpp
	g++ -Wall -O2 -I. -I$(CUDA_INC) -I/opt/intel/tbb/include -L/opt/intel/tbb/lib/intel64/cc4.1.0_libc2.4_kernel2.6.16.21 -lOpenCL -lnvidia-cfg -ltbb -o $(NAME) main.cpp

clear: clean

clean:
	@echo Removing program file
	-@rm -f $(NAME)
