source("bin_search.R")

// test the binary function
n= 7
arr = seq(1,n,1)
bin_search(arr,1,n,2)


data =  read.table("../data/twitter_combined.txt", sep = " ")
data = data[order(data[,1]),]
col1= unlist(data[,1])

data = data.matrix(data)



score = 0 

N = nrow(data)

for (i in 1:nrow(data)) {
		row = unlist(data[i,])
		

		index = bin_search(col1,1,N,row[2])
		
		# indexes = seq(1,100,1)

    if (i%%20000==0) {cat(i,"\n")}
		
		# if (length(indexes) > 0) {
		#   for (index in indexes) {
		#     if (data[2,index] == row[1] ) {
		#     score = score +1
		#     break
		#     }
		#   }
		# }
}


