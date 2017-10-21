library(parallel)

cls <- makePSOCKcluster(rep("localhost",2))

n =10 
data = seq(1,n,1)
data[7] = 1

par_search = function(cls,data) {
rowgrps <- splitIndices(length(data), length(cls))
  
grpsearch  <- function(grp) 
  
{
  return(sum(data[grp]==1))
}  

mout <- clusterApply(cls, rowgrps, grpsearch)
Reduce(c, mout)

}

par_search(cls,data)

