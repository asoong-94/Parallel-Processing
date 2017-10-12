#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>


#define true 1
#define false 0

// find num of reciprocal pairs
int recippar(int **data, int N) {

	int score = 0;
	// int currentV1 = 0;
	// int currentV2 = 0; 
	// int tempV1 = 0;
	// int tempV2 = 0; 

	for (int i = 0; i < N; i++) {
		// current vertex to be compared
		int currentV1 = data[i][0];
		int currentV2 = data[i][1]; 
		for (int j = 0; j < N; j++ ) {
			int tempV1 = data[j][0];
			int tempV2 = data[j][1];
			if (currentV1 == tempV2 && currentV2 == tempV1) {
				score += 1;
				printf("match at: %d and %d\n", tempV1, tempV2);
			}
		}

	}
	return score / 2;
}

// create 2d array with N rows, and 2 columns of random numbers
int** generate_array(int N) {
	int **arr = (int**)malloc(N * sizeof(int*));

	for (int i = 0; i < N; i++) {
		arr[i] = (int*) malloc(2 * sizeof(int));
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < 2; j++) {
			arr[i][0] = rand() % 3;
			arr[i][1] = rand() % 3;
		}
	}

	return arr;
}


void main(int argc, char** argv) {


	int N = 10;
	int** data_arr = (int**)malloc(10 * sizeof(int*));
	for (int i = 0; i < N; i++) {
		data_arr[i] = (int*)malloc(2 * sizeof(int));
	}

	// for (int i = 0; i < N; i++) {
	// 	for (int j = 0; j < 2; j++) {
	// 		data_arr[i][j] = rand() % 5 + 1;
	// 	}
	// }

	data_arr[0][0] = 4; data_arr[0][1] = 2; 
	data_arr[1][0] = 2; data_arr[1][1] = 1; 
	data_arr[2][0] = 4; data_arr[2][1] = 5; 
	data_arr[3][0] = 0; data_arr[3][1] = 6; 
	data_arr[4][0] = 6; data_arr[4][1] = 0; 
	data_arr[5][0] = 7; data_arr[5][1] = 9; 
	data_arr[6][0] = 5; data_arr[6][1] = 8; 
	data_arr[7][0] = 2; data_arr[7][1] = 8; 
	data_arr[8][0] = 9; data_arr[8][1] = 7; 
	data_arr[9][0] = 8; data_arr[9][1] = 5; 

	for (int i = 0; i < N; i++) {
		printf("a: %d ", data_arr[i][0]);
		printf("b: %d\n", data_arr[i][1]);
	}

	int score = recippar(data_arr, N);
	printf("score: %d\n", score);

	return;
}




						
						
