#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>



#define MAX_THREADS_PER_BLOCK 1024 
#define RANDMAX 100

__device__ float mean(float *y, int s, int e) {
	int i; 
	float total = 0; 
	for (int i = s; i < e; i++) {
		total += y[i];
	}
	return total / (e - s + 1);
}

__global__ void findMax(float *x, int n, int k, int *startend, float *bigmax) {
	// shared memory of size: input_size
	extern __shared__ float s[];

	// thread id
	int me = blockDim.x * blockIdx.x + threadIdx.x;
	printf("me: %d", me);
	// copy global data to shared data 
	s[me] = x[me];

	int perstart; // period start
	int perlen;   // period length 
	int perend;   // perlen end 
	int pl1;      // perlen - 1

	// for best found by this thread so far 
	int mystartmax; // start location 
	int myendmax;   // end location 

	float mymaxval; // max value 

	float xbar; 	// scratch variable 

	mymaxval = -1;

	for (perstart = me; perstart <= n - k; perstart ++) {
		for (perlen = k; perlen <= n - perstart; perlen ++) {
			if (perlen == k) {
				xbar = mean(s, perstart, perend); 
			}
			else {
				// update hold mean 
				pl1 = perlen - 1; 
				xbar = (pl1 * xbar + s[perend]) / perlen; 
			}

			if (xbar > mymaxval) {
				mymaxval = xbar; 
				mystartmax = perstart; 
				myendmax = perend; 
			}
		}
	}

	__syncthreads();
	if (mymaxval > *bigmax) {
		*bigmax = mymaxval; 
		startend[0] = mystartmax; 
		startend[1] = myendmax; 
	}
}


int main(int argc, char** argv) {
	int n = atoi(argv[1]); // array input size
	int k = atoi(argv[2]); // window size

	// size of input array
	int input_size = n * sizeof(float); 

	// host input and output array
	float *h_in;

	// allocate memory for host arrays 
	h_in = (float*)malloc(input_size);

	// fill host array with random numbers 
	for (int i = 0; i < n; i++) {
		h_in[i] = rand() / (float)RANDMAX; 
	}

	// device input and output array 
	float *d_in; 

	// allocate memory for device arrays 
	cudaMalloc((void**) &d_in, input_size);

	// copy host input to device input 
	cudaMemcpy(d_in, h_in, input_size, cudaMemcpyHostToDevice);

	// other host parameters
	int *h_startend; 
	h_startend = (int*) malloc(2 * sizeof(int));
	float *h_bigmax = 0;

	// other device parameters 
	int *d_startend;
	cudaMalloc((void**) &d_startend, 2 * sizeof(int));
	float *d_bigmax = 0;

	// create kernel invocation parameters
	int NUM_BLOCKS = n / MAX_THREADS_PER_BLOCK;
	dim3 dimGrid(NUM_BLOCKS, 1); 
	dim3 dimBlock(MAX_THREADS_PER_BLOCK, 1);

	// kernel invocation
	findMax <<< dimGrid, dimBlock, input_size >>>(d_in, n, k, d_startend , d_bigmax); 

	// copy result from device to host
	cudaMemcpy(d_startend, h_startend, input_size, cudaMemcpyDeviceToHost);
	cudaMemcpy(d_bigmax, h_bigmax, input_size, cudaMemcpyDeviceToHost);

	printf("dstartend.1: %d\n", h_startend[0]);
	printf("dstartend.2: %d\n", h_startend[1]);
	printf("d_bigmax: %f\n", h_bigmax);

	// free gpu memory 
	cudaFree(d_startend); 
	cudaFree(d_bigmax); 

	return 0; 
}




