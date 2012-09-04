
rcpp.collapse <- function(...){
	hyObjList<-list(...)
	if (length (hyObjList) == 1 && is.list (hyObjList [[1]]))
		hyObjList <- hyObjList [[1]]
	
	## check the arguments
	lapply (hyObjList, chk.hy)
	lapply (hyObjList, validObject)
	return (eval (.Call("collapse", hyObjList) ) )
}