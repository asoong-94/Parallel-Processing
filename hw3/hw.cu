#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>



#define MAX_THREADS_PER_BLOCK 1024 
#define RANDMAX 100

// __global__ void mean(float **subarr, int start, int end, float *out) {
// 	float tot = 0; 
// 	// for (int i = 0; i < end; i++) {
// 	// 	tot += *subarr[i]; 
// 	// }

// 	for (int i = 0; i < 10; i++0) {
// 	}
// 	__syncthreads(); 

// }

// __global__ void kernel(float *x) {
// 	// vsize
// 	extern __shared__ float sv[];
// 	int me = blockIdx.x * threadIdx.x; 



// }



// void maxburst(float *x, int n, int k, int *startend, float *bigmax) {
// 	int NUM_BLOCKS = n / MAX_THREADS_PER_BLOCK;
// 	int vsize = n * sizeof(float);

// 	float *d_x; // device array 
// 	cudaMalloc((void**) &d_x, vsize); // allocate memory on device 
// 	cudaMemcpy(d_x, x, vsize, cudaMemcpyHostToDevice);

// 	dim3 gridDim(NUM_BLOCKS,1); 
// 	dim3 blockDim(MAX_THREADS_PER_BLOCK,1,1);
	
// 	//kernel<<< gridDim, blockDim >>>(d_x);

// 	float res = 0;
// 	float res_out = 0;	
// 	cudaMalloc((void**) &res, sizeof(float));
// 	mean<<<gridDim, blockDim>>>(&d_x, 10, 15, res); 
// 	cudaMemcpy(&res_out, &res, sizeof(float), cudaMemcpyDeviceToHost);
// 	printf("mean: %f", res_out);
// }

__global__ void findMax(float *x, int n, int k, int *startend, float *bigmax) {
	// shared memory of size: input_size
	extern __shared__ float s[];

	// thread id
	int me;  

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

	me = blockDim.x * blockIdx.x + threadIdx.x;
	mymaxval = -1;







	__syncthreads();
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


	// free gpu memory 
	cudaFree(d_startend); 
	cudaFree(d_bigmax); 

	return 0; 
}



