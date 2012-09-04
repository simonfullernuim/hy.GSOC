##' loess smoothing interpolation for spectra
##' Spectra can be smoothed and interpolated on a new wavelength axis using
##' \code{\link[stats]{loess}}.
##' 
##' Applying \code{\link[stats]{loess}} to each of the spectra, an interpolation onto a new
##' wavelength axis is performed.  At the same time, the specta are smoothed in order to increase the
##' signal : noise ratio. See \code{\link[stats]{loess}} and \code{\link[stats]{loess.control}} on
##' the parameters that control the amount of smoothing.
##' 
##' @param spc the \code{hyperSpec} object
##' @param newx wavelengh axis tointrpolate on
##' @param enp.target,surface,\dots parameters for \code{\link[stats]{loess}} and
##' \code{\link[stats]{loess.control}}. 
##' @param short,user,date handed to \code{\link{logentry}}.
##' @return a new \code{hyperspec} object.
##' @rdname rcpp-loess
##' @export
##' @author S. Fuller, C. Beleites
##' @seealso \code{\link[stats]{loess}}, \code{\link[stats]{loess.control}}
##' @keywords manip datagen
##' @examples
##' 
##' plot (flu, col = "darkgray")
##' plot (rcpp.loess(flu, seq (420, 470, 5)), add = TRUE, col = "red")
##' 
##' flu [[3, ]] <- NA_real_
##' smooth <- rcpp.loess(flu, seq (420, 470, 5))
##' smooth [[, ]]
##' plot (smooth, add = TRUE, col = "blue")
##' 

rcpp.loess <- function (spc, newx, enp.target = nwl (spc) / 4,
		surface = "direct", ...,
		short = "spc.loess", user = "", date = ""){

		extra.args <- list (...)
		if ("normalize" %in% names (extra.args)){
			normalize <- extra.args[["normalize"]]
		}
		else{
			normalize <- TRUE
		}
		if ("parametric"%in% names (extra.args)){
			parametric <- extra.args[["parametric"]]
		}
		else{
			parametric <- FALSE
		}
		if ("drop.square"%in% names (extra.args)){
			drop.square <- extra.args[["drop.square"]]
		}
		else{
			drop.square <- FALSE
		}		
		if ("cell" %in% names (extra.args)){
			cell <- extra.args[["cell"]]
		}
		else{
			cell <- 0.2
		}
		loess.args <- loess.values (spc@data$spc[1,] ~ newx, enp.target = nwl (spc) / 4, surface=surface, ..., short=short)
		loess.ptr <-  getNativeSymbolInfo( "loess_raw" )$address
					#loess.dfitse.ptr <- getNativeSymbolInfo( "loess_dfitse" )$address	#se=FALSE is unchangeable in spc.loess
		##if i pass spc@data$spc directly to this function it changes it by reference - so i !!
		res_spc <-	.Call("rcpp_loess", 
					spc@data$spc,
					nrow (spc@data$spc),
					ncol (spc@data$spc),	
					loess.args,
					package="SPCIO"
						)
		
				#res <- spc
				#print("test")
				#res@data$spc <-	.Call("rcpp_loess", 
			#			res@data$spc,
			#			nrow (res@data$spc),
			#			ncol (res@data$spc),
			#			loess.args,
			#			normalize, parametric, drop.square, surface, cell,
			#			package="SPCIO"
			#	)		
				
		return( decomposition (spc, x = res_spc))	
}

loess.values <- function (formula, data, weights, subset, na.action, model = FALSE,
		span = 0.75, enp.target, degree = 2, parametric = FALSE,
		drop.square = FALSE, normalize = TRUE,
		family = c ("gaussian", "symmetric"),
		method = c ("loess", "model.frame"),
		control = loess.control (...), ...){
	
	family <- match.arg(family)
	method <- match.arg(method)
	mf <- match.call(expand.dots=FALSE)
	mf$model <- mf$span <- mf$enp.target <- mf$degree <-
			mf$parametric <- mf$drop.square <- mf$normalize <- mf$family <-
			mf$method <- mf$control <- mf$... <- NULL
	mf[[1L]] <- as.name("model.frame")
	mf <- eval(mf, parent.frame())
	if (match.arg(method) == "model.frame") return(mf)
	mt <- attr(mf, "terms")
	y <- model.response(mf, "numeric")
	w <- model.weights(mf)
	if(is.null(w)) w <- rep(1, length(y))
	nmx <- as.character(attr(mt, "variables"))[-(1L:2)]
	x <- mf[, nmx, drop=FALSE]
	if(any(sapply(x, is.factor))) stop("predictors must all be numeric")
	x <- as.matrix(x)
	D <- ncol(x)
	nmx <- colnames(x)
	names(nmx) <- nmx
	drop.square <- match(nmx, nmx[drop.square], 0L) > 0L
	parametric <- match(nmx, nmx[parametric], 0L) > 0L
	if(!match(degree, 0L:2L, 0L)) stop("'degree' must be 0, 1 or 2")
	iterations <- if(family == "gaussian") 1 else control$iterations
	if(!missing(enp.target))
		if(!missing(span))
			warning("both 'span' and 'enp.target' specified: 'span' will be used")
		else {				# White book p.321
			tau <- switch(degree+1, 1, D+1, (D+1)*(D+2)/2) - sum(drop.square)
			span <- 1.2 * tau/enp.target
		}
	## Let's add sanity checks on control
	if(!is.list(control) || !is.character(control$surface) ||
			!is.character(control$statistics) || !is.character(control$trace.hat) ||
			!is.numeric(control$cell) || !is.numeric(iterations))
		stop("invalid 'control' argument")
	
	return (append	(simpleLoess.values(y, x, w, span, degree, parametric, drop.square,
							normalize, control$statistics, control$surface,
							control$cell, iterations, control$trace.hat),		##26 args
					list (mt, nmx, mf, model)			##4 extra args
			)	
	)
}


simpleLoess.values <-
		function(y, x, weights, span = 0.75, degree = 2, parametric = FALSE,
				drop.square = FALSE, normalize = TRUE,
				statistics = "approximate", surface = "interpolate",
				cell = 0.2, iterations = 1, trace.hat = "exact")
{
	## loess_ translated to R.
	
	D <- as.integer(NCOL(x))
	if (is.na(D)) stop("invalid NCOL(X)")
	if(D > 4) stop("only 1-4 predictors are allowed")
	N <- as.integer(NROW(x))
	if (is.na(N)) stop("invalid NCOL(X)")
	if(!N || !D)	stop("invalid 'x'")
	if(!length(y))	stop("invalid 'y'")
	x <- as.matrix(x)
	if(!is.double(x)) storage.mode(x) <- "double"
	if(!is.double(y)) storage.mode(y) <- "double"
	if(!is.double(weights)) storage.mode(weights) <- "double"
	max.kd <-  max(N, 200)
	robust <- rep(1, N)
	divisor<- rep(1, D)
	if(normalize && D > 1L) {
		trim <- ceiling(0.1 * N)
		divisor <- sqrt(apply (apply (x, 2L, sort)[seq(trim+1, N-trim), , drop = FALSE], 2L, var))
		x <- x/rep(divisor, rep(N, D))
	}
	sum.drop.sqr <- sum(drop.square)
	sum.parametric <- sum(parametric)
	nonparametric <- sum(!parametric)
	order.parametric <- order(parametric)
	x <- x[, order.parametric]
	order.drop.sqr <- (2 - drop.square)[order.parametric]
	if (degree==1 && sum.drop.sqr)
		stop("specified the square of a factor predictor to be dropped when degree = 1")
	if (D == 1 && sum.drop.sqr)
		stop("specified the square of a predictor to be dropped with only one numeric predictor")
	if (sum.parametric == D) stop("specified parametric for all predictors")
	
	robust <- weights * robust
	#if(j > 1) statistics <- "none"
	if (surface == "interpolate" && statistics == "approximate")
		statistics <- if(trace.hat == "exact") "1.approx"
				else "2.approx" # trace.hat == "approximate"
	surf.stat <- paste(surface, statistics, sep="/")
	if (length(span) != 1L) stop("invalid argument 'span'")
	if (length(cell) != 1L) stop("invalid argument 'cell'")
	if (length(degree) != 1L) stop("invalid argument 'degree'")
	return( list(
					y, x, weights, robust, D, N,
					as.double(span),
					as.integer(degree),
					as.integer(nonparametric),
					as.integer(order.drop.sqr),
					as.integer(sum.drop.sqr),
					as.double(span*cell),
					as.character(surf.stat),
					fitted.values = double(N),
					parameter = integer(7),
					a = integer(max.kd),
					xi = double(max.kd),
					vert = double(2*D),
					vval = double((D+1)*max.kd),
					diagonal = double(N),
					trL = double(1),
					delta1 = double(1),
					delta2 = double(1),
					as.integer(surf.stat == "interpolate/exact"),
					robust,	##extra arg returned
					divisor ##extra arg returned
			))
}