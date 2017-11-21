#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>


__global__ void increment(float *x, float *y) {
	int me = threadIdx.x;
	y[me] = x[me] += 1;
	__syncthreads();
}

int main(int argc, char** argv) {
	int n = 10;

	// arrays for host 
	float *h_input; 
	float *h_output; 

	// allocate space on host
	h_input = (float*)malloc(n * sizeof(float));
	h_output = (float*)malloc(n * sizeof(float));


	// arrays for device 
	float *d_input; 
	float *d_output; 

	// allocate space on device
	cudaMalloc((void**) &d_input, n * sizeof(float)); 
	cudaMalloc((void**) &d_output, n * sizeof(float)); 

	// populate host array
	for (int i = 0; i < n; i++) {
		h_input[i] = i;
	}

	// copy host input to device input array 
	cudaMemcpy(d_input, h_input, n * sizeof(float), cudaMemcpyHostToDevice);

	dim3 gridDim(1,1);
	dim3 gridBlock(n, 1);

	// invoke the increment kernel
	increment<<< gridDim, gridBlock >>>(d_input, d_output);

	// copy device output back to host output
	cudaMemcpy(h_output, d_output, n * sizeof(float), cudaMemcpyDeviceToHost);

	// print result 
	for (int i = 0; i < n; i++) {
		printf("%f\n", h_output[i]);
	}

	// free memory on GPU
	cudaFree(d_output); 
	cudaFree(d_input); 

	return 0;
}

