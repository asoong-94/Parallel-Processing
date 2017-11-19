#include <omp.h>
#include <math.h>
#include <stdio.h>

float root(float(*f)(float), float inita, float initb, int niters) {
	float curra = inita; 
	float currb = initb; 

	#pragma omp parallel 
	{
		int nth = omp_get_num_threads(); 
		int me = omp_get_thread_num(); 
		int iter; 
		for (iter = 0; iter < niters; iter++) {
			#pragma omp barrier 
			float subintwidth = (currb - curra) / nth; 
			float myleft = curra + me * subintwidth; 
			float myright = myleft + subintwidth; 
			if ((*f)(myleft) < 0 && (*f)(myright) > 0) {
				curra = myleft; 
				currb = myright; 
			}
		}
	}

	return curra; 
}

float testf(float x) {
	return pow(x - 2.1, 3);
}

void myFunc(int x) {
	printf("%d\n", x); 

}
int main(int argc, char** argv) {
	// printf("%f\n", root(testf, -4.1,4.1,1000));

	
	(*myFunc)(2);


}