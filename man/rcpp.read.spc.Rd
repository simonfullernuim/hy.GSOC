\name{rcpp.read.spc}
\alias{rcpp.read.spc}
\title{Reads files from spc file format into R as hyperSpec object}
\usage{
  rcpp.read.spc(filename, keys.hdr2data = c('fexper', 'fres', 'fsource'), keys.log2data = FALSE )
}
\arguments{
  \item{filename}{file name}.
  \item{keys.hdr2data}{Header data to be returned in hyperSpec object.} 
  \item{keys.log2data}{Log data to be returned in hyperSpec object.}
}
\value{
  An object of class \code{hyperSpec}.
}
\description{
  Reads an spc file into R as a \code{hyperSpec} object. 
  Additional header and log data can be requested via \code{keys.hdr2data} and \code{keys.log2data}.
}
\source{

https://ftirsearch.com/features/converters/spcfileformat.htm}
\details{
	Handles different versions of spc, e.g. implicit wavelength, explicit wavelength, multiple spectra.
}

\examples{
ob <- rcpp.read.spc('/home/simon/SPC_files/s_xy.spc')
}
\author{ 
  Simon Fuller, C. Beleites
}