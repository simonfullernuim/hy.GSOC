\name{rcpp.read.Kaiser}
\alias{rcpp.read.Kaiser}
\title{Imports sets of .spc files of Raman maps written by Kaiser Optical Systems' Hologram software.}
\usage{rcpp.read.Kaiser(filenames, keys.hdr2data=c(), keys.log2data=FALSE, glob=TRUE)}
\arguments{
  \item{filenames}{file names - either a list or a wild card expression}.
  \item{keys.hdr2data}{Header data to be returned in hyperSpec object.} 
  \item{keys.log2data}{Log data to be returned in hyperSpec object.}
  \item{glob}{If \code{TRUE} the filename is interpreted as a wildcard containing file name pattern and expanded to all matching file names.}
}
\value{
  An object of class \code{hyperSpec}.
}
\description{
  Imports sets of .spc files of Raman maps written by Kaiser Optical Systems' Hologram software, i.e. typically in the form of several files, each containing a spectrum, 
  which are combined into one hyperSpec object.
  Additional header and log data can be requested via \code{keys.hdr2data} and \code{keys.log2data}.
}
\source{
https://ftirsearch.com/features/converter/s/spcfileformat.htm
}
\details{
	Handles different versions of spc, e.g. implicit wavelength, explicit wavelength, multiple spectra.
}
\examples{
ob <- rcpp.read.Kaiser('/home/simon/spc.Kaisermap/*')
}
\author{ 
  Simon Fuller, C. Beleites
}