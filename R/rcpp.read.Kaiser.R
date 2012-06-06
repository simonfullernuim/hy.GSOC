

rcpp.read.Kaiser <- function  (filenames,
		#keys.hdr2data = c('fexper', 'fres', 'fsource'), 
		keys.hdr2data = c(), 
		keys.log2data = FALSE, 
		glob = TRUE)	{
	require("Rcpp")
	if(glob){
		files<-Sys.glob(files)
	}

	return (eval ( .Call( "rcpp_read_Kaiser", 
					PACKAGE = "SPCIO", 
					#filenames = as.character (filenames), 
					filenames = as.list (filenames), 
					keys.hdr2data = as.character (keys.hdr2data),
					keys.log2data = as.character (keys.log2data)
				  )
		  )
	);
	
}