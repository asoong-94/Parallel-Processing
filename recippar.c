#include <omp.h>
#include <stdlib.h>
#include <stdio.h>


#define true 1
#define false 0

// tuple that holds data
typedef struct {
	int v1; 
	int v2; 
} tuple; 


// if match, add 1 to score
int compare(tuple *t1, tuple *t2) {
	if ((t1->v1 == t2->v1) && (t1->v2 == t2->v2))
		return 1;

	return 0;
}

// reverse tuple
tuple* reverse(tuple *t) {
	tuple *t2 = (tuple*) malloc(sizeof(tuple));
	t2->v1 = t->v2; 
	t2->v2 = t->v1; 

	return t2; 
}

// find num of reciprocal pairs
int recippar(int **data, int N) {

	int score = 0;
	tuple* tuples; 
	tuples = (tuple*) malloc(N * sizeof(tuple));

	// populate array of tuples
	for (int i = 0; i < N; i++) {
		tuples[i].v1 = data[i][0];
		tuples[i].v2 = data[i][1];
	}

	for (int i = 0; i < N; i++) {
		tuple *current = (tuple*) malloc(sizeof(tuple)); 
		current = reverse(&tuples[i]);
		for (int j = 0; j < N; j++) {
			score += compare(current, &tuples[j]);
		}
		
	}
	return score/2;
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
	// int data[9][2] = {
	// 		{1,2}, 	// match 1
	// 		{2,3},	// match 2
	// 		{4,5}, 
	// 		{2,1},  // match 1
	// 		{3,2},	// match 2
	// 		{8,6},
	// 		{9,4},
	// 		{2,6},	// match 3
	// 		{6,2},	// match 3
	// 	};

	int N = 10;
	int** data_arr = generate_array(N);
	for (int i = 0; i < N; i++) {
		printf("a: %d ", data_arr[i][0]);
		printf("b: %d\n", data_arr[i][1]);
	}

	int score = recippar(data_arr, N);
	printf("score: %d\n", score);

	return;
}







