#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>

#define N 1618
#define T 1024 

__global__ void vecAdd (int *in) {
	int me = blockIdx.x * blockDim.x + threadIdx.x;
	in[me] += 1;
	// printf("me: %d\n", me); 
}


int main(int argc, char** argv) {
	// int k = atoi(argv[1]); 
	int n = atoi(argv[1]); 

	int *h_x; // host array
	int *h_result; // host result
	int *d_x; // device array
	int *d_result; // device result

	// size_t arr_size = n * sizeof(int); 	// size of array 
	h_x = (int*) malloc(n * sizeof(int)); 		// allocate memory for host input
	h_result = (int*) malloc(n * sizeof(int));  // allocate memory on host result
	cudaMalloc((void**) &d_x, n * sizeof(int));	// allocate memory for device
	cudaMalloc((void**) &d_result, n * sizeof(int)); // allocate memory for device result

	for (int i = 0; i < n; i++) {
		h_x[i] = i;
	}

	// copy host input to device input
	cudaMemcpy(d_x, h_x, n * sizeof(int),cudaMemcpyHostToDevice);

	// allocate blocksizes and gridsizes
	dim3 gridDim(1, 1, 1); 
	dim3 blockDim(n, 1, 1); 

	// increment 1 to d_x write to d_result
	vecAdd<<<gridDim, blockDim>>>(d_x);//, d_result);

	// copy from device result to host result
	cudaMemcpy(h_result, d_x, n * sizeof(int), cudaMemcpyDeviceToHost);

	for (int i = 0; i < N; i++) {
		printf("h_result: %d: %d\n", i, h_result[i]);
	}

	// free memory of GPU
	cudaFree(d_x);
	cudaFree(d_result);
}

