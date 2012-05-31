
read.spc <- function  (filename,
						keys.hdr2data = c('fexper', 'fres', 'fsource'), keys.hdr2log = c('fdate', 'fpeakpt'),
						keys.log2data = FALSE, keys.log2log = TRUE,
						log.txt = TRUE, log.bin = FALSE, log.disk = FALSE,
						hdr = list (),
						no.object = FALSE)	{
	
	require ("Rcpp");
	#require("SPCIO") #i.e. this!
	#use new for object...
	.Call( "SPCIO_entrance", 
			PACKAGE = "SPCIO", 
			filename = as.character (filename), 
			keys.hdr2data = as.character (keys.hdr2data),
			keys.log2data = as.character (keys.log2data),
			log.txt = as.logical (log.txt),
			hdr = as.list (hdr),
			no.object = as.logical (no.object) );
	
}