#include <omp.h>
#include <stdio.h> 
#include <stdlib.h>

void findsum(int *m, int* cs, int n) {
	int sum = 0; 
	int topofcol;
	int col; 
	int k; 

	for (col = 0; col < n; col++) {
		topofcol = col; 
		for (k = 0; k < n; k++) {
			sum += m[topofcol + k*n]; 
		}
		cs[col] = sum;
	}
}

int main(int argc, char** argv) {
	int n = 10000; 
	int *hcs; 
	// int msize = n * n * sizeof(int); 

	int *hm = (int*)malloc(n * n * sizeof(int)); 

	int t = 0;
	int i; // rows
	int j; // cols aka offset 

	for (i = 0; i < n; i ++) {
		for (j = 0; j < n; j++) {
			hm[i*n + j] = 1; 
		}
	}

	int cssize = n * sizeof(int); 
	hcs = (int*)malloc(cssize); 

	findsum(hm, hcs, n); 
	for (int i = 0; i < n; i++) {
		printf("%d\n", hcs[i]); 
	}

	free(hm); 
	free(hcs);
}