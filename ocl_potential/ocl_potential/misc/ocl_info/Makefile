NAME=./ocl_info
CUDA_ROOT=/usr/local/cuda
#OCL_INC=${CUDA_ROOT}/include
#OCL_LIB=${CUDA_ROOT}/lib64
OCL_INC=/opt/intel/opencl/include
OCL_LIB=/opt/intel/opencl/lib64

.PHONY: all clear clean

all: $(NAME)

$(NAME): ocl_info.cpp
	icpc -Wall -O2 -I. -I$(OCL_INC) -I/opt/intel/tbb/include -L/opt/intel/tbb/lib/intel64/cc4.1.0_libc2.4_kernel2.6.16.21 -L$(OCL_LIB) -lOpenCL -ltbb -o $(NAME) ocl_info.cpp

clear: clean

clean:
	@echo Removing program file
	-@rm -f $(NAME)
