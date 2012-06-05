#include <string>
#include <Rcpp.h>
#include "SPC_reader.h"
#include "Super_parser.h"
#include "Basic_parser.h"
#include "TXVALS_parser.h"
//#include "main_functions.hpp"
using namespace std;


void switch_flag( BYTE& hdrflag, BYTE& switch_flg, bool& TORDRD_case, bool& TALABS_case);

//return StageX,Y,Z positions from log
RcppExport SEXP rcpp_read_Kaiser( SEXP _files, SEXP _hdr2data, SEXP _log2data,
		SEXP _log_txt, SEXP _log_bin, SEXP _log_disk, SEXP _no_object ){

BEGIN_RCPP
	Rprintf("begin\n");
	Rcpp::CharacterVector files( _files );

	vector<string> hdr2data = Rcpp::as< vector< string > >( _hdr2data );
	vector<string> log2data = Rcpp::as< vector< string > >( _log2data );
	bool log_txt = (bool) Rcpp::as< int >( _log_txt );
	bool log_bin = (bool) Rcpp::as< int >( _log_bin );
	bool log_disk = (bool) Rcpp::as< int >( _log_disk );
	bool no_object = (bool) Rcpp::as< int >( _no_object );

	string file_str =  Rcpp::as<string>( files(0) );
	SPC_reader* spcrdr = new SPC_reader( file_str );
	spcrdr->read_proc_hdr();

	unsigned short nrows = files.size();
	unsigned short ncols = spcrdr->hdr.fnpts;

	Rcpp::NumericVector X( ncols );
	Rcpp::NumericMatrix Y( nrows, ncols );
	Rcpp::NumericVector X_pos( nrows );	//collect positions for data
	Rcpp::NumericVector Y_pos( nrows );
	Rcpp::NumericVector Z_pos( nrows );
	Rcpp::NumericVector::iterator X_pos_it, Y_pos_it, Z_pos_it;
	X_pos_it = X_pos.begin();
	Y_pos_it = Y_pos.begin();
	Z_pos_it = Z_pos.begin();

	log2data.clear();	//do not collect any others..
	log2data.push_back("Stage_X_Position");
	log2data.push_back("Stage_Y_Position");
	log2data.push_back("Stage_Z_Position");

	bool TORDRD_case =0;
	bool TALABS_case = 0;
	BYTE switch_flg = spcrdr->hdr.ftflgs;
	switch_flag( spcrdr->hdr.ftflgs, switch_flg, TORDRD_case, TALABS_case);

	switch(switch_flg){
		case 0:{
			spcrdr->parser = new Basic_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, X.begin(), Y.begin(), &log2data, &hdr2data );
			break;
		}
		case TXVALS:{
			spcrdr->parser = new TXVALS_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, X.begin(), Y.begin(), &log2data, &hdr2data );
			break;
		}
		default:{
			Rprintf("Invalid Format: %i - Exiting.\n", spcrdr->hdr.ftflgs);
			return 0;
			break;
		}
	}
	spcrdr->parser->readX();
	spcrdr->parser->readY(0, spcrdr->hdr.fnpts, spcrdr->hdr.fnpts, ncols);
	spcrdr->parser->read_log();
	*X_pos_it = atoi(spcrdr->parser->labeltovalue["Stage_X_Position"].c_str());
	*Y_pos_it = atoi(spcrdr->parser->labeltovalue["Stage_Y_Position"].c_str());
	*Z_pos_it = atoi(spcrdr->parser->labeltovalue["Stage_Z_Position"].c_str());
	++X_pos_it; ++Y_pos_it; ++Z_pos_it;

	spcrdr->ifstr.close();
	int idx = 1;
	for( Rcpp::CharacterVector::iterator it = files.begin()+1; it != files.end(); ++it ){
		file_str = Rcpp::as<string>( *it );
		spcrdr->ifstr.open( file_str.c_str(), std::ios_base::in | std::ios_base::binary );
		spcrdr->read_proc_hdr();

		switch_flag( spcrdr->hdr.ftflgs, switch_flg, TORDRD_case, TALABS_case);
		switch( switch_flg ){
			case 0:{
				spcrdr->parser = new Basic_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, X.begin(), Y.begin(), &log2data, &hdr2data );
				break;
			}
			case TXVALS:{
				spcrdr->parser = new TXVALS_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, X.begin(), Y.begin(), &log2data, &hdr2data );
				break;
			}
			default:{
				Rprintf("Invalid Format: %i - Exiting.\n", spcrdr->hdr.ftflgs);
				return 0;
				break;
			}
		}//hctiws
		spcrdr->parser->readX();
		spcrdr->parser->readY(0, spcrdr->hdr.fnpts, spcrdr->hdr.fnpts, ncols);
		spcrdr->parser->read_log();
		*X_pos_it = atoi(spcrdr->parser->labeltovalue["Stage_X_Position"].c_str());
		*Y_pos_it = atoi(spcrdr->parser->labeltovalue["Stage_Y_Position"].c_str());
		*Z_pos_it = atoi(spcrdr->parser->labeltovalue["Stage_Z_Position"].c_str());
		++X_pos_it; ++Y_pos_it; ++Z_pos_it;
	}//rof
	Rcpp::DataFrame data = Rcpp::DataFrame::create( Rcpp::Named("x") = X_pos, Rcpp::Named("y") = Y_pos, Rcpp::Named("z") = Z_pos, Rcpp::Named("file") = files );
	Rcpp::Language hyObj_call("new", "hyperSpec", Rcpp::Named("spc") = Y, Rcpp::Named("wavelength") = X, Rcpp::Named("data") = data );
	return hyObj_call.eval();

END_RCPP
}



void switch_flag( BYTE& hdrflag, BYTE& switch_flg, bool& TORDRD_case, bool& TALABS_case){
if( (hdrflag & TSPREC) == TSPREC ){
		switch_flg = switch_flg - 1;
	}
	if( (hdrflag & TORDRD) == TORDRD ){
		TORDRD_case = 1;
		switch_flg = switch_flg - TORDRD;
	}
	if( (hdrflag & TALABS) == TALABS ){
		TALABS_case = 1;
		switch_flg = switch_flg - TALABS;
	}
}
