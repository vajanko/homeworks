NAME=./du3
CPP=icpc
CFLAGS=-Wall -O3 -std=c++0x -fp-model precise
CUDA_ROOT=/usr/local/cuda
INCLUDE=${CUDA_ROOT}/include
LIBDIRS=${CUDA_ROOT}/lib64 
LIBS=OpenCL rt
HEADERS=$(shell find . -name '*.hpp') $(shell find ./bpp -name '*.hpp') $(shell find ./implementation -name '*.hpp')

.PHONY: all clear clean purge

all: $(NAME)

$(NAME): main.cpp $(HEADERS)
	$(CPP) $(CFLAGS) $(addprefix -I,$(INCLUDE)) $(addprefix -L,$(LIBDIRS)) $(addprefix -l,$(LIBS)) -o $@ $<


clear: clean

purge: clean

clean:
	@echo Removing program file
	-@rm -f $(NAME)
