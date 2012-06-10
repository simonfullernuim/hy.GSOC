/*
 * blas_test.cpp
 *
 *  Created on: 10 Jun 2012
 *      Author: simon
 */

/*
 * blas_test.cpp
 *
 *  Created on: 10 Jun 2012
 *      Author: simon
 */

//NB FLAG AND WRAP THIS IN AN #ifdef for compatibility with putenv etc

#include <cstdlib>
#include <string>
#include <iostream>
#include <Rcpp.h>
//#include "common.h"

RcppExport SEXP blas_test( SEXP _num ){
	BEGIN_RCPP
	int num = Rcpp::as< int >( _num );

	//openblas_set_num_threads( num );
	char cnum = (char) num + 48;
	std::string set_str = "OPENBLAS_NUM_THREADS= ";// + cnum;
	set_str = set_str + "" + cnum;
	Rprintf("%s\n", (char*) set_str.c_str() );
	int status = putenv( (char*) set_str.c_str() );
	return Rcpp::wrap( status );

	END_RCPP
}





