library(Rcpp)
library(zoo)

setwd("/Users/asoong/desktop/ecs/ecs158/Parallel-Processing/hw2/Prob2_C_Solution")
sourceCpp("Rcpp-tutorial.cpp")
hello()

sourceCpp("rollmean-rcpp.cpp")
rmRcpp()


y <- rnorm(1000000000)
omp_rollmean(y, 3)
