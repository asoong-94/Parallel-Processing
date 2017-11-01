#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int onedim(int n, int i, int j) {
	return n*i + j;
}

void transpose_memcpy(int *m, int n) {
	#pragma omp parallel 
	{
		int i;
		int j;
		int tmp;
		#pragma omp for
		for(i = 0; i < n; i++) {
			for(j = i + 1; j < n; j++) {
				memcpy(&tmp, &m[onedim(n,i,j)], 1);
				memcpy(&m[onedim(n,i,j)], &m[onedim(n,j,i)], 1);
				memcpy(&m[onedim(n,j,i)], &tmp, 1);
			}
		}	
	}
	for(int i = 0; i < 16; i++) {
		printf("%d\n", m[i]);
	}
}


void transpose_temp(int *m, int n) {
	#pragma omp parallel 
	{
		int i;
		int j;
		int temp;
		#pragma omp for
		for(i = 0; i < n; i++) {
			for(j = i + 1; j < n; j++) {
				temp = m[onedim(n, i, j)];
				m[onedim(n, i, j)] = m[onedim(n, j, i)];
				m[onedim(n, j, i)] = temp; 
			}
		}	
	}
	for(int i = 0; i < 16; i++) {
		printf("%d\n", m[i]);
	}
}

 
int main() {
  int array[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
  transpose_temp(array, 4);
  transpose_memcpy(array, 4);
 	
  return 0;
}