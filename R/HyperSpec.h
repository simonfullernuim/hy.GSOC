/*
 * HyperSpec.h
 *
 *  Created on: 24 May 2012
 *      Author: simon
 */

#ifndef HYPERSPEC_H_
#define HYPERSPEC_H_

#include <Rcpp.h>
#include <R.h>
#include <S4.h>
#include <RObject.h>
//template <int RTYPE>

class HyperSpec {
private:
protected:
public:
	Rcpp::DataFrame data;
	Rcpp::NumericMatrix X;
	Rcpp::NumericMatrix Y;
	Rcpp::List label;		//this right?? etc...


};


#endif /* HYPERSPEC_H_ */
