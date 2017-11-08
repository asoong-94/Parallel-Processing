#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
		int z; 

	#pragma omp parallel 
	{

		int n = 16; 
		int array[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

		#pragma omp for reduction(+:z)		
		for (int i = 0; i < n; i++) {

			z += array[i];
		}
	}

	printf("%d\n",z);
}