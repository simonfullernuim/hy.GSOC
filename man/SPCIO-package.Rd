\name{SPCIO}
\alias{SPCIO}
\docType{package}
\title{
File-IO interface to read SPC files into R as hyperSpec S4 objects.
}
\description{
Employs Rcpp to read SPC files into R as hyperSpec objects.
Both standard SPC and Kaiser structured formats are supported.
Also includes rcpp.collapse, a more efficient version of hyperSpec's collapse function.
}
\details{
\tabular{ll}{
Package: \tab SPCIO\cr
Type: \tab Package\cr
Version: \tab 1.0\cr
Date: \tab 2012-05-11\cr
License: \tab GPL (>=3)\cr
}
}
\author{
Simon Fuller, Claudia Beleites
Maintainer: Simon Fuller <simonfuller9@gmail.com>
}
\references{
See Claudia Beleites' hyperSpec package vignettes for more information on the hyperSpec package.
}
\keyword{ SPC, hyperSpec, Rcpp }
\seealso{
}
\examples{
ob1 <- rcpp.read.spc('/home/simon/SPC_files/s_xy.spc')
ob2 <- rcpp.read.Kaiser('/home/simon/spc.Kaisermap/*')
}
