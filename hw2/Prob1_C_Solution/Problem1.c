#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <string.h>
#include <assert.h>

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
	// printf("first element key, value is %d, %d\n", arr[0].key, arr[1].value);
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

int calculate_score(int *array, int num_elements) {
  int sum = 0;
  int i;
  for (i = 0; i < num_elements; i++) {
    sum += array[i];
		// printf("i = %d, value is %d", i, array[i]);
  }
  return sum;
}

int recippar(int *edges,int nrow)
{
	int N = nrow;
	tuple* tuples = to_tuple_array(N, edges);

	qsort(tuples, N, sizeof(tuples[0]), comparator_using_tuple);
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);

	int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
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

	int num_elements_per_proc = nrow/world_size;
	tuple* sub_tuple_arr = (tuple *)malloc(sizeof(tuple) * num_elements_per_proc);
	assert(sub_tuple_arr != NULL);
	// Send out the whole edges to all workers
	MPI_Bcast(tuples, num_elements_per_proc, stat_type, 0, MPI_COMM_WORLD);
	// Send out a subarrays (w/ size num_elements_per_proc) to all workers
	MPI_Scatter(tuples, num_elements_per_proc, stat_type, sub_tuple_arr,
              num_elements_per_proc, stat_type, 0, MPI_COMM_WORLD);

	// Find recippars
	int subarray_score = 0;
	int reflexive_nodes = 0;
	// printf("whole tuple length is %d\n", sizeof(tuples)/sizeof(tuples[0]));
	for (int i = 0; i < num_elements_per_proc; i++) {

			if(sub_tuple_arr[i].key != sub_tuple_arr[i].value) {	// Otherwise, the node is reflexive
				int index = iter_binarySearch(tuples, 0, N-1, sub_tuple_arr[i].value);
				// printf("value is %d at index %d\n", sub_tuple_arr[i].key, i );
				if (-1 != index) {
					// search right
					int curr_r = index;
					int curr_l = index;
					int found = 0;
					while( found == 0 && curr_r < N && tuples[curr_r].key == sub_tuple_arr[i].value ) {
						if( sub_tuple_arr[i].key == tuples[curr_r].value) {
							subarray_score += 1;
							found = 1;
						}
						curr_r++;
					}
					// curr = index;
					// search left
					while( found == 0 && curr_l >= 0 && tuples[curr_l].key == sub_tuple_arr[i].value ) {
						if( sub_tuple_arr[i].key == tuples[curr_l].value) {
							subarray_score += 1;
							found = 1;
						}
						curr_l--;
					}
				}
			} else {
				reflexive_nodes++;
			}
		}
	printf("node %d, subscore %d\n", world_rank, subarray_score);
	// Gather all partial scores down to all the processes
  int *subarray_scores = (int *)malloc(sizeof(int) * world_size);
  assert(subarray_scores != NULL);
  MPI_Allgather(&subarray_score, 1, MPI_INT, subarray_scores, 1, MPI_INT, MPI_COMM_WORLD);

	// Now that we have all of the partial averages, compute the
  // total average of all numbers. Since we are assuming each process computed
  // an average across an equal amount of elements, this computation will
  // produce the correct answer.
  int score = calculate_score(subarray_scores, world_size);

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
