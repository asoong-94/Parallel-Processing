#include <omp.h>
#include <stdlib.h>
#include <stdio.h>


typedef struct {
	int v1; 
	int v2; 
} tuple; 

int compare(tuple *t1, tuple *t2) {
	if ((t1->v1 == t2->v1) && (t1->v2 == t2->v2))
		return 0;

	return -1;
}

tuple* reverse(tuple *t) {
	tuple *t2 = (tuple*) malloc(sizeof(tuple));
	t2->v1 = t->v2; 
	t2->v2 = t->v1; 

	return t2; 
}


void recippar(int data[9][2]) {

	tuple* tuples; 
	tuples = (tuple*) malloc(9 * sizeof(tuple));

	for (int i = 0; i < 9; i++) {
		tuples[i].v1 = data[i][0];
		tuples[i].v2 = data[i][1];
	}

	// testing to see tuple insertion 
	for (int i = 0; i < 9; i++) {
		printf("data.v1: %d ", tuples[i].v1);
		printf("data.v2: %d\n", tuples[i].v2);
	}
	
	return;
}

void main(int argc, char** argv) {
	int data[9][2] = {
			{1,2},
			{2,3},
			{4,5}, 
			{2,1},
			{3,2},
			{8,6},
			{9,4},
			{2,6},
			{6,2},
		};

	// recippar(data);

	// testing out reverse tuples
	tuple *t = (tuple*) malloc(sizeof(tuple));
	t->v1 = 1; 
	t->v2 = 10; 

	tuple *t2 = t;

	if (compare(t, t2) == 0)
		printf("true\n");
	else
		printf("false\n");
	// t2 = reverse(t); 

	// printf ("%d, %d", t->v1, t->v2);
	// printf ("\n");
	// printf ("%d, %d", t2->v1, t2->v2);
	// printf ("\n");


	return;
}







