#include <string>
#include <Rcpp.h>
#include "SPC_reader.h"
#include "Super_parser.h"
#include "Basic_parser.h"
#include "TXVALS_parser.h"

using namespace std;

void switch_flag( BYTE& hdrflag, BYTE& switch_flg, bool& TORDRD_case, bool& TALABS_case);

RcppExport SEXP rcpp_read_Kaiser( SEXP _files, SEXP _hdr2data, SEXP _log2data ){

BEGIN_RCPP

	Rcpp::List files( _files );
	vector<string> hdr2data = Rcpp::as< vector< string > >( _hdr2data );
	vector<string> log2data = Rcpp::as< vector< string > >( _log2data );

	string file_str =  Rcpp::as<string>( files(0) );

	SPC_reader* spcrdr = new SPC_reader( file_str );
	spcrdr->read_proc_hdr();

	SUBHDR subhdr;

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

	log2data.clear();	//do not collect any others.. as per Claudia's
	log2data.push_back( "Stage_X_Position" );
	log2data.push_back( "Stage_Y_Position" );
	log2data.push_back( "Stage_Z_Position" );

	bool TORDRD_case =0;
	bool TALABS_case = 0;
	BYTE switch_flg = spcrdr->hdr.ftflgs;
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
			Rprintf( "Invalid Format: %i - Exiting.\n", spcrdr->hdr.ftflgs );
			return 0;
			break;
		}
	}
	spcrdr->parser->readX( ncols );
	spcrdr->parser->read_proc_subhdr(subhdr);
	spcrdr->parser->readY(0, spcrdr->hdr.fnpts, spcrdr->hdr.fexp, nrows);
	spcrdr->parser->read_log();

	*X_pos_it = atoi( spcrdr->parser->labeltovalue["Stage_X_Position"].c_str() );
	*Y_pos_it = atoi( spcrdr->parser->labeltovalue["Stage_Y_Position"].c_str() );
	*Z_pos_it = atoi( spcrdr->parser->labeltovalue["Stage_Z_Position"].c_str() );
	++X_pos_it; ++Y_pos_it; ++Z_pos_it;

	spcrdr->ifstr.close();
	int idx = 1;
	for( Rcpp::List::iterator it = files.begin()+1; it != files.end(); ++it ){
		file_str = Rcpp::as<string>( *it );
		spcrdr->ifstr.open( file_str.c_str(), std::ios_base::in | std::ios_base::binary );
		spcrdr->read_proc_hdr();

		switch_flag( spcrdr->hdr.ftflgs, switch_flg, TORDRD_case, TALABS_case);
		switch( switch_flg ){
			case 0:{
				spcrdr->parser = new Basic_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, X.begin(), Y.begin(), &log2data, &hdr2data );
				spcrdr->ifstr.seekg( SUBHSZ, ios::cur );
				break;
			}
			case TXVALS:{
				spcrdr->parser = new TXVALS_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, X.begin(), Y.begin(), &log2data, &hdr2data );
				spcrdr->ifstr.seekg( ncols * 4 + SUBHSZ, ios::cur );
				break;
			}
			default:{
				Rprintf("Invalid Format: %i - Exiting.\n", spcrdr->hdr.ftflgs);
				return 0;
				break;
			}
		}//hctiws
		spcrdr->parser->readY(idx, spcrdr->hdr.fnpts, spcrdr->hdr.fexp, nrows);
		spcrdr->parser->read_log();

		*X_pos_it = atoi( spcrdr->parser->labeltovalue["Stage_X_Position"].c_str() );
		*Y_pos_it = atoi( spcrdr->parser->labeltovalue["Stage_Y_Position"].c_str() );
		*Z_pos_it = atoi( spcrdr->parser->labeltovalue["Stage_Z_Position"].c_str() );
		++X_pos_it; ++Y_pos_it; ++Z_pos_it;
		spcrdr->ifstr.close();
		++idx;
	}//rof

	//Hack to create a vector of character strings - only way??
	Rcpp::Language cfiles( "as.character", files );

	Rcpp::DataFrame data = Rcpp::DataFrame::create( Rcpp::Named("x") = X_pos, Rcpp::Named("y") = Y_pos, Rcpp::Named("z") = Z_pos, Rcpp::Named("file") = cfiles.eval() );

	Rcpp::List label = Rcpp::List::create( Rcpp::Named( ".wavelength" ) = Rcpp::ExpressionVector( SPC_reader::get_FX( spcrdr->hdr.fxtype ) ),
										   Rcpp::Named( "spc" )  = Rcpp::ExpressionVector( SPC_reader::get_FY( spcrdr->hdr.fytype ) ),
										   Rcpp::Named("z") = Rcpp::ExpressionVector( SPC_reader::get_FX( spcrdr->hdr.fxtype ) ),
										   Rcpp::Named("z.end") = Rcpp::ExpressionVector( SPC_reader::get_FX( spcrdr->hdr.fxtype ) ) );

	Rcpp::Language hyObj_call("new", "hyperSpec", Rcpp::Named("spc") = Y, Rcpp::Named("wavelength") = X, Rcpp::Named("data") = data, Rcpp::Named("labels") = label );

	return hyObj_call;//return unevaluated LANGSXP and evaluate in R

END_RCPP
}//cnuf


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
}//cnuf
