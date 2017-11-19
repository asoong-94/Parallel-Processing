#include <iostream> 
#include <math.h>

void add (int n, float *x, float *y) {
	for (int i = 0; i < n; i++) {
		y[i] = x[i] + y[i];
	}
}

__global__ void add(int n, float *x, float *y) {
  for (int i = 0; i < n; i++)
      y[i] = x[i] + y[i];
}

int main() {
	long N  = 1 << 30; 
	// float *x = new float[N];
	// float *y = new float[N];
	float *x; 
	float *y; 
	cudaMallocManaged(&x, N*sizeof(float));
	cudaMallocManaged(&y, N*sizeof(float));



	for (int i = 0; i < N; i++) {
		x[i] = 1.0f; 
		y[i] = 2.0f; 
	}

	// add(N, x, y); 

	add <<<1, 1>>>(N, x, y);
	 // Check for errors (all values should be 3.0f)
	
	float maxError = 0.0f;
	for (int i = 0; i < N; i++)
	maxError = fmax(maxError, fabs(y[i]-3.0f));
	std::cout << "Max error: " << maxError << std::endl;

	// Free memory
	delete []x;
	delete []y;

	return 0;
}