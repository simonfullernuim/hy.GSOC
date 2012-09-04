#include <Rcpp.h>
#include <R.h>
//#include <RInternals.h>
#include <string>
#include <utility>
#include <vector>
//#include "loessc.h"
using namespace std;

//http://lists.r-forge.r-project.org/pipermail/rcpp-devel/2010-June/000754.html
typedef union {void *p; DL_FUNC fn;} fn_ptr;

DL_FUNC R_ExternalPtrAddrFn(SEXP s){
      fn_ptr tmp;
      tmp.p =  EXTPTR_PTR(s);
      return tmp.fn;
}

//functions from loess(stats) reincluded in this package to be run from this function
extern "C" {
	void loess_raw2(double *y, double *x, double *weights, double *robust, Sint *d,
		  Sint *n, double *span, Sint *degree, Sint *nonparametric,
		  Sint *drop_square, Sint *sum_drop_sqr, double *cell,
		  char **surf_stat, double *surface, Sint *parameter,
		  Sint *a, double *xi, double *vert, double *vval, double *diagonal,
		  double *trL, double *one_delta, double *two_delta, Sint *setLf);
}//nretxe

extern "C" {
	void loess_dfit2(double *y, double *x, double *x_evaluate, double *weights,
	   double *span, Sint *degree, Sint *nonparametric,
	   Sint *drop_square, Sint *sum_drop_sqr,
	   Sint *d, Sint *n, Sint *m, double *fit);
}


RcppExport SEXP rcpp_loess( SEXP _spc, SEXP _nrow, SEXP _ncol, SEXP _arg_list ){
		//SEXP loess_ptr, SEXP loess_dfit_ptr, SEXP loess_ifit_ptr ){	//SEXP externalptr
	BEGIN_RCPP

	Rcpp::NumericMatrix spc( Rcpp::clone( _spc ) );
	int nrow = Rcpp::as<int>( _nrow );
	int ncol = Rcpp::as<int>( _ncol );
	Rcpp::List arg_list( _arg_list );

   // int nrow = spc.nrow();
   // int ncol = spc.ncol();

 //   Rcpp::NumericMatrix res( nrow, ncol);
  //  Rcpp::Rcout << "Getting Func Pointers " << endl;
//    DL_FUNC loess_raw = R_ExternalPtrAddrFn( loess_ptr );
 //   DL_FUNC loess_dfit = R_ExternalPtrAddrFn( loess_dfit_ptr );
 //   DL_FUNC loess_ifit = R_ExternalPtrAddrFn( loess_ifit_ptr );

    //Get the contents of arg_list
//    Rcpp::Rcout << "Resolving arg list" << endl;
    Rcpp::NumericVector arg2((SEXP)arg_list(1));
    Rcpp::NumericVector arg3((SEXP)arg_list(2));
    Rcpp::NumericVector arg4((SEXP)arg_list(3));
    int arg5 = Rcpp::as<int>(arg_list(4));
    int arg6 = Rcpp::as<int>(arg_list(5));
    double arg7 = Rcpp::as<double>(arg_list(6));
    int arg8 = Rcpp::as<int>(arg_list(7));
    int arg9 = Rcpp::as<int>(arg_list(8));	//NB: paramtetric/nonparametric can be vectors but here only a bool(1/0) is supported for now.
    int arg10 = Rcpp::as<int>(arg_list(9));
    int arg11 = Rcpp::as<int>(arg_list(10));
    double arg12 = Rcpp::as<double>(arg_list(11));
    string arg13 = Rcpp::as<string>(arg_list(12));
    Rcpp::NumericVector arg14((SEXP)arg_list(13));

    Rcpp::IntegerVector arg15((SEXP)arg_list(14));
    Rcpp::IntegerVector arg16((SEXP)arg_list(15));
    Rcpp::NumericVector arg17((SEXP)arg_list(16));
    Rcpp::NumericVector arg18((SEXP)arg_list(17));
    Rcpp::NumericVector arg19((SEXP)arg_list(18));
    Rcpp::NumericVector arg20((SEXP)arg_list(19));
    double arg21 = Rcpp::as<double>(arg_list(20));
    double arg22 = Rcpp::as<double>(arg_list(21));
    double arg23 = Rcpp::as<double>(arg_list(22));
    int arg24 = Rcpp::as<int>(arg_list(23));

    double * arg2d = &arg2[0];
    double * arg3d = &arg3[0];
    double * arg4d = &arg4[0];
    char * arg13d = (char*) arg13.c_str();
    double * arg14d = &arg14[0];
    int * arg15d = &arg15[0];
    int * arg16d = &arg16[0];
    double * arg17d = &arg17[0];
    double * arg18d = &arg18[0];
    double * arg19d = &arg19[0];
    double * arg20d = &arg20[0];

    double * tmp = new double[nrow];
    double * fit = new double[nrow];
    long copysize = sizeof(double) * nrow;
    for( int i = 0; i < nrow; ++i ){
		for( int j = 0; j < ncol; ++j ){
			tmp[j] = spc(i,j);
		}//rof
		loess_raw2( (double*) tmp, (double*) arg2d, (double*) arg3d,
					(double*) arg4d, (Sint*) &arg5, (Sint*) &arg6, (double*) &arg7, (Sint*) &arg8, (Sint*) &arg9, (Sint*) &arg10,
					(Sint*) &arg11, (double*) &arg12, (char**) &arg13d, (double*) arg14d, (Sint*) arg15d, (Sint*) arg16d,
					(double*) arg17d, (double*) arg18d, (double*) arg19d, (double*) arg20d,
					(double*) &arg21, (double*) &arg22, (double*) &arg23, (Sint*) &arg24 );

		for( int j = 0; j < ncol; ++j ){
			spc(i,j) = arg14d[j];
		}//rof
	}//rof
	delete [] fit;
	delete [] tmp;
	return Rcpp::wrap (spc);
	END_RCPP
}
