/*
 * test_wrapper.hpp
 *
 *  Created on: 25 May 2012
 *      Author: simon
 */
//#ifdef INSIDE
#include <Rcpp.h>
//#include <RInside.h>

//RcppExport SEXP test_wrapper( SEXP X ){
RcppExport SEXP test_wrapper(){
	BEGIN_RCPP

	//Rprintf("%i\n",0);
	//Rcpp::S4 obj("hyperSpec");
	//Rcpp::S4 obj("eg");

	Rprintf("%i\n",1);
	//Rcpp::S4 obj(X);
	Rcpp::Language hobcall("new", "hyperSpec");
	//SEXP x = call.eval();

	Rprintf("%ia\n",1);
	Rcpp::S4 obj( hobcall.eval() );

	Rcpp::NumericVector X(10);
	Rcpp::NumericMatrix Y(1,10);
	short NROWS = 1;
	int i = 0;
	for( Rcpp::NumericVector::iterator vit = X.begin(); vit != X.end(); ++vit ){
		*vit = i++;
	}
	i = 0;
	for( Rcpp::NumericMatrix::iterator mit = Y.begin(); mit != Y.end(); mit += NROWS ){
		*mit = (2 * i);
		++i;
	}
	/*
	Rcpp::DataFrame::create(Rcpp::Named("a")=a,
					  Rcpp::Named("b")=b,
					  Rcpp::Named("c")=c);	*/
	Rprintf("%i\n",2);
	obj.slot("wavelength") = X;
	Rprintf("%ia\n",3);

	Rcpp::DataFrame df = Rcpp::DataFrame::create( Rcpp::Named( "spc" ) = Rcpp::NumericVector(1) );
	//Rcpp::DataFrame df = Rcpp::DataFrame::create( Rcpp::Named( "spc" ) = YM );
	Rprintf("%ib\n",3);
	//Rcpp::NumericVector t(1);
	//t(0) = 10;
	df[0] = Y;
	Rprintf("%i\n",4);
	obj.slot("data") = df;
	//obj.slot("spc") = YM;
	//obj.slot("b") = "testing";
	Rprintf("%i\n",5);
	/*
	SEXP an = Rcpp::Language("ncol", obj);
	Rprintf("%i\n",5);
	Rcpp::NumericVector n(an);
	for(int i = 0; i < n.size(); ++i ){
		Rprintf("%i", n(i));
	}
	Rprintf("\n");
	Rprintf("%i\n",6);
	*/
	/*
	 SEXP Rcpp::wrap( const S4& HyObj ){
	         Rcpp::Language call( "new", Rcpp::Symbol( "S4" ), object.Rcpp::get_i() ) ;
	         return call.eval() ;
	     }
	*/

	 //Rcpp::Language call("ncol",df.attr("spc"));
	 //Rcpp::Language call("length",obj.slot("wavelength"));
	//Rcpp::Language call("names",obj.slot("data"));

	//SEXP x = obj.slot("data");
	//return x;

	return obj;
	END_RCPP

}


//#endif
