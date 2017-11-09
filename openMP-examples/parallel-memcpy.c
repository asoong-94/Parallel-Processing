
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int onedim(int row, int col, int nCols) { return row * nCols + col; }

int *getCols(int *m, int nRowsM, int nColsM, int firstCol, int nColsSubM) {
   int *subMat;
   #pragma omp parallel
   {  int *startFrom,*startTo;

      #pragma omp single
      subMat = malloc(nRowsM * nColsSubM * sizeof(int));

      #pragma omp for
      for (int row = 0; row < nRowsM; row++) {
         startFrom = m + onedim(row,firstCol,nColsM);
         startTo = subMat + onedim(row,0,nColsSubM);
         memcpy(startTo, startFrom, nColsSubM * sizeof(int));
      }
   }
   return subMat;
}

int main() {
   // intended as a 4x3 matrix
   int z[12] = {5,12,13,6,2,22,15,3,1,2,3,4};
   omp_set_num_threads(2);
   // extract the last 2 cols
   int *outM = getCols(z,4,3,1,2);
   for (int i = 0; i < 4; i++) {
      printf("%d %d\n",outM[2*i],outM[2*i+1]);
   }
}


