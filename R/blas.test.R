

blas.test <- function( n ){
	max_threads <- 12
	if (n < 1){
		print ("Invalid value\n")
		return()
	}
	if (n > max_threads){	#have some sort of maximum
		warning ("Value reset to 12")
	}

	status <- .Call("blas_test", 
					PACKAGE="SPCIO",
					n = as.integer(n)
			)
	return(status)
}