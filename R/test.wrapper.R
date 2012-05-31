
#test.wrapper<-function(X, Y){
	#spc <- new ("hyperSpec", spc, wavelength, data, labels)
	#spc <- .Call("test_wrapper", X = as.numeric(X), Y = as.numeric(Y), PACKAGE="SPCIO")
	#return (spc)
#}

set.test.class<-function(){
	setClass ("eg",
			representation (
					a = "numeric",     # spectral abscissa
					b = "character"       # data: spectra & information related to each spectrum
			)
	)
}

#test.wrapper<-function(X="eg"){
	#spc <- new ("hyperSpec", spc, wavelength, data, labels)
#	spc <-  .Call("test_wrapper", X, PACKAGE="SPCIO");
#	return (spc);
#}

test.wrapper<-function(){
	spc <-  .Call("test_wrapper", PACKAGE="SPCIO");
	return (spc);
}
# res <- test.wrapper( new( "hyperSpec", 
#							 wavelength = as.numeric(c(), data = data.frame(), label = list(), log = data.frame() ) )