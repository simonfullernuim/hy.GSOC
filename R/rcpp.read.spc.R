
rcpp.read.spc <- function  (filename,
						keys.hdr2data = c('fexper', 'fres', 'fsource'), 
						keys.log2data = FALSE, 
						log.txt = TRUE, log.bin = FALSE, log.disk = FALSE,
						no.object = FALSE)	{
							#hdr = list (), <--reinclude
	require ("Rcpp");
	#require("SPCIO") #i.e. this!
	#use new for object...
	return( .Call( "rcpp_read_spc", 
			PACKAGE = "SPCIO", 
			filename = as.character (filename), 
			keys.hdr2data = as.character (keys.hdr2data),
			keys.log2data = as.character (keys.log2data),
			log.txt = as.logical (log.txt),
					#hdr = as.list (hdr),
			log.bin = as.logical(log.bin), 
			log.disk = as.logical(log.disk),
			no.object = as.logical (no.object) )
	);
	
}