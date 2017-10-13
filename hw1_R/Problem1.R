# For each edge, we are comparing if there's an opposite edge.
recippar <- function(edges) {
  list_length <- length(edges)
  result <- 0

  for(i in 1:nrow(edges)) {

    if(edges[i,1] != edges[i,2]) {
      for(j in 1:nrow(edges)) {
        if(i != j && edges[i,1] == edges[j,2] && edges[i,2] == edges[j,1]) {
          result <- result + 1
        }
      }
    }

  }

  return(result/2)
}

# Use the small sample input provided by the Prof.
arr1 <- rbind(c(1,6),
              c(2,1),
              c(3,3),
              c(1,2),
              c(1,3),
              c(2,4),
              c(4,2),
              c(3,1),
              c(6,3))

recippar(arr1)
