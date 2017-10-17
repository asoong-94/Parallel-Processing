#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>


typedef struct {
	int key;
	int value;
}tuple;

tuple* to_tuple_array(int N, char* file_path) {
	int num = 0, i = 0, j = 0;

	tuple* tuples = (tuple*)malloc(N* sizeof(tuple));

	FILE *fin;
	fin = fopen(file_path,"r");
	while (!feof(fin)) {
		fscanf(fin,"%d %d", &tuples[num].key, &tuples[num].value);
		num++;
		i = num/2;
		j = num % 2;
	}
	fclose(fin);

	return tuples;
}


int ** to_array(int N, char * file_path){
	int num = 0, i = 0, j = 0;

	int **arr = (int**) malloc(N*sizeof(int*));
	for (i=0;i<N; i++)
		arr[i] = (int*) malloc (2*sizeof(int));

	i=0;


	FILE *fin;
	fin = fopen(file_path,"r");
	while (!feof(fin)) {
		fscanf(fin,"%d",&arr[i][j]);
		num++;
		i = num/2;
		j = num % 2;
	}
	fclose(fin);

	return arr;
}

int comparator_using_tuple(const void *p, const void *q) {
	int l = ((tuple*)p)->key;
	int r = ((tuple*)q)->key;

	return (l - r);
}

int comparator_using_index(const void *p, const void *q) {
	// int l = ((struct tuple*) p)->key;
	// int r = ((struct tuple*) q)->key;

	int l = ((int*) p)[0];
	int r = ((int*) q)[0];

    return (l - r);
}

int comparator_using_array_index(const void *p, const void *q) {
	// int l = ((struct tuple*) p)->key;
	// int r = ((struct tuple*) q)->key;

	int* l = ((int*) p);
	int* r = ((int*) q);

    // both odd, put the greater of two first.
    if ((l[0]&1) && (r[0]&1))
        return (r[0]-l[0]);

    // both even, put the smaller of two first
    if ( !(l[0]&1) && !(r[0]&1) )
        return (l[0]-r[0]);

    // l is even, put r first
    if (!(l[0]&1))
        return 1;

    // l is odd, put l first
    return -1;
}


long long int*cantor(int** arr, int N) {
	long long int *data = (long long int*)malloc(N * sizeof(long long int));

	for (int i = 0; i < N; i++) {
		int x = arr[i][0];
		int y = arr[i][1];
		long long int res = 0.5 * (x + y) * (x + y + 1) + y;
		printf("%d\n", res);
	}

	return data;
}

int iter_binarySearch(tuple* arr, int l, int r, int x)
{
  while (l <= r)
  {
    int m = l + (r-l)/2;

    // Check if x is present at mid
    if (arr[m].key == x)
        return m;

    // If x greater, ignore left half
    if (arr[m].key < x)
        l = m + 1;

    // If x is smaller, ignore right half
    else
         r = m - 1;
  }

  // if we reach here, then element was not present
  return -1;
}

void main(int argc, char* argv[]) {

	char* file_path = "./twitter_combined.txt";
	int N = 1768149;
	// int** data = to_array(N, file_path);

	int score = 0;

	tuple* tuples = to_tuple_array(N, file_path);
	clock_t begin = clock();

	qsort(tuples, N, sizeof(tuples[0]),comparator_using_tuple);

	int reflexive_nodes = 0;
	#pragma omp parallel
	{
		#pragma omp for
		for (int i = 0; i < N; i++) {

			if(tuples[i].key != tuples[i].value) {	// Otherwise, the node is reflexive
				int index = iter_binarySearch(tuples, 0, N-1, tuples[i].value);
				if (-1 != index) {
					// search right
					int curr = index;
					int found = 0;
					while( found == 0 && curr < N && tuples[curr].key == tuples[i].value ) {
						if( tuples[i].key == tuples[curr].value) {
							score += 1;
							found = 1;
						}
						curr++;
					}
					curr = index;
					// search left
					while( found == 0 && curr >= 0 && tuples[curr].key == tuples[i].value ) {
						if( tuples[i].key == tuples[curr].value) {
							score += 1;
							found = 1;
						}
						curr --;
					}
				}
			} else {
				reflexive_nodes++;
			}
		}
	}
		printf("we have %d reflexive_nodes\n", reflexive_nodes);




	clock_t end = clock();
	double time = (double)(begin - end) / CLOCKS_PER_SEC;
	printf("time: %d\n", time);
	printf("score: %d\n", score/2);



	return;
}
