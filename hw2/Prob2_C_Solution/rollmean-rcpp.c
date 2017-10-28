
#include <Rcpp.h>

using namespace std; 

// [[Rcpp::export]]
NumericVector rmRcpp(NumericVector dat, const int window) {
  const int n = dat.size();
  NumericVector ret(n-window+1);
  double summed = 0.0;
  for (int i=0; i < window; ++i) {
    summed += dat[i];
  }
  ret[0] = summed / window;
  for (int i=window; i < n; ++i) {
    summed += dat[i] - dat[i-window];
    ret[i-window+1] = summed / window;
  }
  return ret;
}