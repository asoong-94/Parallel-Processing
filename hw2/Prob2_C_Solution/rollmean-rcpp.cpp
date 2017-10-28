
#include <Rcpp.h>

using namespace Rcpp; 
// [[Rcpp::export]]
// NumericVector rmRcpp(NumericVector dat, const int window) {
//   const int n = dat.size();
//   NumericVector ret(n-window+1);
//   double summed = 0.0;
//   for (int i=0; i < window; ++i) {
//     summed += dat[i];
//   }
//   ret[0] = summed / window;
//   for (int i=window; i < n; ++i) {
//     summed += dat[i] - dat[i-window];
//     ret[i-window+1] = summed / window;
//   }
//   return ret;
// }

// [[Rcpp::export]]
NumericVector omp_rollmean(NumericVector x, const int k) {
  const int n = x.size();
  NumericVector ret(n-k+1);
  double summed = 0.0;
  for (int i=0; i < k; ++i) {
    summed += x[i];
  }
  ret[0] = summed / k;
  for (int i=k; i < n; ++i) {
    summed += x[i] - x[i-k];
    ret[i-k+1] = summed / k;
  }
  return ret;
}

