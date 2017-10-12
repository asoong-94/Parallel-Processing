#include <omp.h>
#include <stdlib.h>
#include <stdio.h>


#define true 1
#define false 0

// tuple that holds data
typedef struct {
	int v1;
	int v2;
	int found_match;
} tuple;


// if match, add 1 to score
int compare(tuple *t1, tuple *t2) {
	if ((t1->v1 == t2->v1) && (t1->v2 == t2->v2)) {
		// t1->found_match = true;
		// t2->found_match = true;
		return 1;
	}

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

	tuple* tuples = (tuple*) malloc(N * sizeof(tuple));
	#pragma omp parallel
	{
		int nth = omp_get_num_threads();
		int me = omp_get_thread_num();

		// populate array of tuples
		#pragma omp for
		for (int i = 0; i < N; i++) {
			tuples[i].v1 = data[i][0];
			tuples[i].v2 = data[i][1];
			tuples[i].found_match = false;
		}

		for (int i = 0; i < N; i++) {
			tuple *current = &tuples[i];// (tuple*) malloc(sizeof(tuple));
			// current = &tuples[i];
			if(current->found_match == false && !(current->v1 == current->v2)) {
				current = reverse(&tuples[i]);
				#pragma omp for
				for (int j = 0; j < N; j++) {
					if (compare(current, &tuples[j]) == 1) {
						tuples[j].found_match = true;
						tuples[i].found_match = true;
						score += 1;
					}
				}
			}
		}
	}

	return score;
}

// create 2d array with N rows, and 2 columns of random numbers
int** generate_array(int N) {
	int **arr = (int**)malloc(N * sizeof(int*));

	for (int i = 0; i < N; i++) {
		arr[i] = (int*) malloc(2 * sizeof(int));
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < 2; j++) {
			arr[i][0] = rand() % 5;
			arr[i][1] = rand() % 5;
		}
	}

	return arr;
}


void main(int argc, char** argv) {

	int N = 5;
	int** data_arr = generate_array(N);
	for (int i = 0; i < N; i++) {
		printf("a: %d ", data_arr[i][0]);
		printf("b: %d\n", data_arr[i][1]);
	}

	int score = recippar(data_arr, N);
	printf("score: %d\n", score);

	return;
}
