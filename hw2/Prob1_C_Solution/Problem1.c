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

	// Setup the custom MPI_datatype
	// Ref: https://stackoverflow.com/questions/18165277/how-to-send-a-variable-of-type-struct-in-mpi-send
	// (Nicola's answer)
	tuple _info;
	int count; //Says how many kinds of data your structure has
	count = 1; //1, 'cause you just have int

	// Says the type of every block
	MPI_Datatype array_of_types[count];
	// You just have int
	array_of_types[0] = MPI_INT;

	// Says how many elements for block
	int array_of_blocklengths[count];
	// You have 2 int
	array_of_blocklengths[0] = 2;

	/*Says where every block starts in memory, counting from the beginning of the struct.*/
	MPI_Aint array_of_displaysments[count];
	MPI_Aint address1, address2;
	MPI_Get_address(&_info,&address1);
	MPI_Get_address(&_info.key,&address2);
	array_of_displaysments[0] = address2 - address1;

	/*Create MPI Datatype and commit*/
	MPI_Datatype stat_type;
	MPI_Type_create_struct(count, array_of_blocklengths, array_of_displaysments, array_of_types, &stat_type);
	MPI_Type_commit(&stat_type);

	//Now we are ready to Scatter and Broadcast things
	// MPI_Send(&_info, 1, stat_type, dest, tag, comm),

	int num_elements_per_proc = 1000;
	tuple* sub_tuple_arr = (tuple *)malloc(sizeof(tuple) * num_elements_per_proc);
	// Send out the whole edges to all workers
	MPI_Bcast(edges, num_elements_per_proc, stat_type, 0, MPI_COMM_WORLD);
	// Send out a subarrays to all workers
	MPI_Scatter(edges, num_elements_per_proc, stat_type, sub_tuple_arr,
              num_elements_per_proc, stat_type, 0, MPI_COMM_WORLD);

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
