#include <stdio.h>
#include <stdlib.h>
// #include <cuda.h>
#include <omp.h>

double mean(double *y, int s, int e) {
	double tot = 0;
	for (int i = s; i <= e; i++) { tot += y[i]; }
	return tot / (e - s + 1);
}



// __global__ void maxburst(float *x, int n, int k, int *startend, float *bigmax) {

// }

void burst (double *x, int nx, int k, int *startmax, int *endmax, double *maxval) {
	int nth; 
	#pragma omp parallel 
	{
		int perstart,	// period start
			perlen,		// period length
			perend, 	// perlen end
			pl1; 		// perlen - 1

		int mystartmax, myendmax; 
		double mymaxval;  
		double xbar; // scratch;

		int me; 

		#pragma omp single 
		{
			nth = omp_get_num_threads(); 
		}
		me = omp_get_thread_num(); 
		mymaxval = -1; 

		#pragma omp for 
		for (perstart = 0; perstart <= nx - k; perstart++) {
			for (perlen = k; perlen <= nx - perstart; perlen++) {
				perend = perstart + perlen - 1; 
				if (perlen == k) {
					xbar = mean(x, perstart, perend); 
				}
				else {
					// update old mean 
					pl1 = perlen - 1; 
					xbar = (pl1 * xbar + x[perend]) / perlen; 
				}
				if (xbar > mymaxval) {
					mymaxval = xbar; 
					mystartmax = perstart; 
					myendmax = perend;
				}
			}
		}
		#pragma omp critical 
		{
			if (mymaxval > *maxval) {
				*maxval = mymaxval; 
				*startmax = mystartmax; 
				*endmax = myendmax; 
			}
		}
	}
}



int main(int argc, char** argv) {
	int startmax, endmax; 
	double maxval; 
	double *x; 
	int k = atoi(argv[1]); 
	int i, nx; 
	nx = atoi(argv[2]); // length of x 
	x = (double*)malloc(nx * sizeof(double)); 

	for (i = 0; i < nx; i++) {
		x[i] = rand() / (double) RAND_MAX;
	}

	double starttime, endtime; 
	starttime = omp_get_wtime(); 
	burst(x, nx, k, &startmax, &endmax, &maxval); 
	endtime = omp_get_wtime(); 

	printf("elapsed time: %f\n", endtime - starttime); 
	printf("%d %d %f\n", startmax, endmax, maxval); 
	if (nx < 25) {
		for (int i = 0; i < nx; i++) {
			printf("%f\n", x[i]); 
		}
	}

	return 0;
}