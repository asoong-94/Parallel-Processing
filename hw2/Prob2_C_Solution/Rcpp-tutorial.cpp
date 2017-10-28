#include <Rcpp.h>

using namespace Rcpp;
// Rcpp attribute

// [[Rcpp::export]]
void hello()
{
	Rprintf("Hello, world!\n");
}

int main() {


	return 0;
}