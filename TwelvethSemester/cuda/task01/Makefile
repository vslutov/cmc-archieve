PROJECT=GPUTask01
SOURCES=main.cu nlm.cu

NVCC_FLAGS=-Xcompiler -fopenmp -lSOIL --std=c++11
NVCC=nvcc

all : ${PROJECT}

% : %.cu
	${NVCC} ${NVCC_FLAGS} $^ -o $@

clean:
	rm ${PROJECT}
