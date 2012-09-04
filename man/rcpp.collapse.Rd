\name{rcpp.collapse}
\alias{rcpp.collapse}
\title{Collapses a list of individual hyperSpec objects into a single one.}
\usage{
	rcpp.collapse(...)
}
\arguments{
  \item{...}{A list of hyperSpec objects.}.
}
\value{
  An object of class \code{hyperSpec}.
}
\description{
	An optimized version of the \code{hyperSpec} \code{collapse} function.
	Takes in a list of hyperSpec objects and  and binds them into one object with number of spectra equal to the sum of the spectra of the arguments, inserting NAs where required.
	Returns a hyperSpec object with ascending wavelength.
}
\source{
}
\details{
}

\examples{
ob1 <- rcpp.read.spc ('/home/simon/SPC_files/RAMAN.SPC')
ob2 <- rcpp.read.spc ('/home/simon/SPC_files/s_evenx.spc')
ob3 <- rcpp.collapse (ob1, ob2)
}
\author{ 
  Simon Fuller, C. Beleites
}