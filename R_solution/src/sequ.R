data =  read.table("../data/twitter_combined.txt", sep = " ")
data = data[order(data[,1]),]

data= t(data.matrix(data))
data_rev = data[,c(2,1)]

score = 0 


for (i in 1:ncol(data)) {
		row = unlist(data[,i])
		

		# indexes = which(data[,1] == row[2])
		
		indexes = seq(1,100,1)

    if (i%%20000==0) {cat(i,"\n")}
		
		if (length(indexes) > 0) {
		  for (index in indexes) {
		    if (data[2,index] == row[1] ) {
		    score = score +1
		    break
		    }
		  }
		}
}


