source("bin_search.R")


data =  read.table("../data/twitter_combined.txt", sep = " ")
data = data[order(data[,1]),]

data = data.matrix(data)

col1= unlist(data[,1])

score = 0 

N = nrow(data)

for (i in 1:nrow(data)) {
		row = unlist(data[i,])
		index = bin_search(col1,1,N,row[2])
		
  if (i%%20000==0) {cat(i,"\n")}
		
		if (index!=-1) { 
						found = FALSE
						i = index
						while (col1[i] == row[2] && i > 0) {
									if (data[i,2] == row[1]) {
												score = score +1
												found = TRUE
												break
									}
									i = i-1
						}
						
						if (!found) {
									i=index+1
									while (col1[i] == row[2] && i <=N) {
											if (data[i,2] == row[1]) {
														score = score +1
														break
													}
											i = i+1
									}
						}
		 }
}

score = score/2 -7 // 7 is the number of reflexive nodes

