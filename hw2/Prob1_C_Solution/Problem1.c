#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <string.h>

typedef struct {
	int key;
	int value;
}tuple;


int * to_array(int N, char * file_path){
	int num = 0, i = 0, j = 0;

	int *arr = (int*) malloc(N*2*sizeof(int));

	FILE *fin;
	fin = fopen(file_path,"r");
	while (!feof(fin)) {
		fscanf(fin,"%d",(arr+i*2+j));
		num++;
		i = num/2;
		j = num % 2;
	}
	fclose(fin);

	return arr;
}


tuple* to_tuple_array(int N, int *edges) {
	int num = 0, i = 0, j = 0;

	tuple* tuples = (tuple*)malloc(N* sizeof(tuple));

 for (i=0; i < N;i++)
	{
  tuples[i].key = *(edges + i*2);
		tuples[i].value = *(edges + i*2 +1);
	}
	return tuples;
}


int comparator_using_tuple(const void *p, const void *q) {
	int l = ((tuple*)p)->key;
	int r = ((tuple*)q)->key;

	return (l - r);
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

int recippar(int *edges,int nrow)
{
	int N = nrow;

	int score = 0;

  int reflexive_nodes = 0;

	tuple* tuples = to_tuple_array(N, edges);

	qsort(tuples, N, sizeof(tuples[0]), comparator_using_tuple);

  // Initialize the MPI environment
  MPI_Init(NULL, NULL);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d"
           " out of %d processors\n",
           processor_name, world_rank, world_size);

    // Finalize the MPI environment.
  MPI_Finalize();

	printf("score: %d\n", score/2);
	return score/2;
}

// Compile: mpicc -g Problem1.c
// Run (with CSIF nodes): mpiexec -f hosts3 -n 3 ./a.out 1768149 twitter_combined.txt
//     (locally)        : mpiexec ./a.out 1768149 twitter_combined.txt
// Note: - hosts3 is the file containing host lists
//       - -n 3 indicates the number of node being used
//       * Make sure that a.out is available on all the nodes in hosts3

int main(int argc, char *argv[])
{
  int N = atoi(argv[1]);
  char file_path[100] = "../../data/";
  strcat(file_path,argv[2]);
  printf("file path: %s\n", file_path);
  int * data = to_array(N, file_path);
  int score = recippar(data, N);
  return 0;
}
