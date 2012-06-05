
rcpp.readlist.spc <- function  (filename,
		keys.hdr2data = c('fexper', 'fres', 'fsource'), 
		keys.log2data = FALSE, 
		log.txt = TRUE, log.bin = FALSE, log.disk = FALSE,
		no.object = FALSE)	{
	#hdr = list (), <--reinclude
	require ("Rcpp");
	#require("SPCIO") #i.e. this!
	#use new for object...
	li <- .Call( "rcpp_read_spc", 
					PACKAGE = "SPCIO", 
					filename = as.character (filename), 
					keys.hdr2data = as.character (keys.hdr2data),
					keys.log2data = as.character (keys.log2data),
					log.txt = as.logical (log.txt),
					#hdr = as.list (hdr),
					log.bin = as.logical(log.bin), 
					log.disk = as.logical(log.disk),
					no.object = as.logical (no.object) );
	
		#	return( lapply(li[[1]], function(x)  new("hyperSpec", spc=x[[1]], wavelength=x[[2]], data=x[[3]]) ));
	len <- length(li[[3]])
	ret_li <- vector("list", len)
	print(li[[3]])
	for(i in 1:len){
		
		
		ret_li[[ li[[3]][i] ]] <- new("hyperSpec", spc=li[[1]][[i]][[1]], wavelength=li[[1]][[i]][[2]], data=li[[2]]); 
	#	ret_li[[ li[[2]][i] ]] <- new("hyperSpec", spc=li[[1]][[i]][[1]], wavelength=li[[1]][[i]][[2]]); 
	}
	return(ret_li);
	#return( lapply(li[[1]], eval ) )
}