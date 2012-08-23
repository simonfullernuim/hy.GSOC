/*
 * rcpp_read_spc.cpp
 *
 *  Created on: 1 Jun 2012
 *      Author: simon
 */



//#ifdef INSIDE


/*
 * 	Inheritance: Super_Class --> Basic_parser --> TMULTI_parser
 * 							 --> TXVALS_parser
 *							 --> TMULTI_TXVALS_parser
 *
 *							 Well, a 3D CUDA implemented circular buffer
also asynchronous multithreaded processing
those two are the keys to it
 */


/*
 * Ancilliary data: Label: TALABS==1 --> then we use the data in hdr...
 * So we can return: e.g.
 *
 */
#include <Rcpp.h>
#include <R.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <utility>
#include <sstream>

#include "WTypes.h"
#include "SPC.h"
#include "SPC_reader.h"
#include "Super_parser.h"
#include "Basic_parser.h"
#include "TXVALS_parser.h"
#include "TMULTI_parser.h"
#include "TMULTI_TXVALS_parser.h"
#include "TXYXYS_TXVALS_parser.h"
#include "TMULTI_TXYXYS_TXVALS_parser.h"
#include "main_subfunctions.h"

using namespace std;

RcppExport SEXP rcpp_read_spc( SEXP _file, SEXP _hdr2data, SEXP _log2data){
		//,SEXP _log_txt, SEXP _log_bin, SEXP _log_disk ){

	BEGIN_RCPP
	string file = Rcpp::as<string>(_file);
	vector<string> hdr2data = Rcpp::as< vector< string > >( _hdr2data );
	vector<string> log2data = Rcpp::as< vector< string > >( _log2data );
	//bool log_txt = (bool) Rcpp::as< int >( _log_txt );
	//bool log_bin = (bool) Rcpp::as< int >( _log_bin );
	//bool log_disk = (bool) Rcpp::as< int >( _log_disk );

	SPC_reader* spcrdr = new SPC_reader( file );
	spcrdr->read_proc_hdr();
	Rcpp::NumericVector X( spcrdr->hdr.fnpts );
	bool TORDRD_case =0;
	bool TALABS_case = 0;
	/*
	 * Okay, TORDRD does not seem to do anything thus far: so I am coralling it here and taking it out of the current switch statement..
	 * This is where we stop that happpening.
	 */
	BYTE switch_flg = spcrdr->hdr.ftflgs;
	switch_flag( spcrdr->hdr.ftflgs, switch_flg, TORDRD_case, TALABS_case);

	/* NBNBNBB:
	 * Probably nicer to roll this out into a discrimination net when finished..
	 */
	//switch( spcrdr->hdr.ftflgs ){	//we use polymorphism, but the matrices must be passed in from outside
	switch( switch_flg ){
		case 0 :{
			Rcpp::NumericMatrix Y(1, spcrdr->hdr.fnpts );
			spcrdr->parser = new Basic_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, X.begin(), Y.begin(), &log2data, &hdr2data );
			spcrdr->parser->parse_file();

			short list_sz = spcrdr->parser->labeltovalue.size()  + 2;
			Rcpp::List extralist( list_sz );//include hdr values here also!!!
			Rcpp::CharacterVector extranames( list_sz );	//include hdr values here also!!!
			spcrdr->parser->arrange_data( extranames, extralist, 1 );//1 dimension

			if(spcrdr->parser->want_log_but_no_log == 1){
			//	Rcpp:: Rcout << "WARNING: want log but no log" << endl;
			}//fi

			Rcpp::DataFrame data1 = Rcpp::DataFrame::create( extralist );
			Rcpp::List label = Rcpp::List::create( Rcpp::Named( ".wavelength" ) = Rcpp::ExpressionVector( SPC_reader::get_FX( spcrdr->hdr.fxtype ) ),
												   Rcpp::Named( "spc" )  = Rcpp::ExpressionVector( SPC_reader::get_FY( spcrdr->hdr.fytype ) ),
												   Rcpp::Named("z") = Rcpp::ExpressionVector( SPC_reader::get_FX( spcrdr->hdr.fxtype ) ),
												   Rcpp::Named("z.end") = Rcpp::ExpressionVector( SPC_reader::get_FX( spcrdr->hdr.fxtype ) ) );

			Rcpp::Language hyObj_call( "new", "hyperSpec",  Rcpp::Named("spc") =  Y, Rcpp::Named("wavelength") =  X,  Rcpp::Named("data") = data1, Rcpp::Named("labels") = label ); //log = log, label = lab

			delete spcrdr;
			return hyObj_call.eval();
			break;
		}//esac
		case TMULTI :{

			Rcpp::NumericMatrix Y(spcrdr->hdr.fnsub, spcrdr->hdr.fnpts );
			spcrdr->parser = new TMULTI_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, X.begin(), Y.begin(), &log2data, &hdr2data );
			spcrdr->parser->parse_file();

			short list_sz = spcrdr->parser->labeltovalue.size()  + 2;
			Rcpp::List extralist( list_sz );//include hdr values here also!!!
			Rcpp::CharacterVector extranames( list_sz );	//include hdr values here also!!!
			spcrdr->parser->arrange_data( extranames, extralist, spcrdr->hdr.fnsub );
					//SORT OUT WARNINGS!!!
			if(spcrdr->parser->want_log_but_no_log == 1){
				//Rcpp::Rcout << "WARNING: want log but no log" << endl;
			}//fi

			Rcpp::DataFrame data1 = Rcpp::DataFrame::create(extralist);
			Rcpp::List label = Rcpp::List::create( Rcpp::Named( ".wavelength" ) = Rcpp::ExpressionVector( SPC_reader::get_FX( spcrdr->hdr.fxtype ) ),
												   Rcpp::Named( "spc" )  = Rcpp::ExpressionVector( SPC_reader::get_FY( spcrdr->hdr.fytype ) ),
												   Rcpp::Named("z") = Rcpp::ExpressionVector( SPC_reader::get_FX( spcrdr->hdr.fxtype ) ),
												   Rcpp::Named("z.end") = Rcpp::ExpressionVector( SPC_reader::get_FX( spcrdr->hdr.fxtype ) ) );

			Rcpp::Language hyObj_call( "new", "hyperSpec",  Rcpp::Named("spc") =  Y, Rcpp::Named("wavelength") =  X,  Rcpp::Named("data") = data1, Rcpp::Named("labels") = label ); //log = log, label = lab

			delete spcrdr;
			return hyObj_call.eval();
			break;
		}

		case TXVALS :{

			Rcpp::NumericMatrix Y(1, spcrdr->hdr.fnpts );
			spcrdr->parser = new TXVALS_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, X.begin(), Y.begin(), &log2data, &hdr2data );
			spcrdr->parser->parse_file();
			//spcrdr->print_SPCHDR();
			short list_sz = spcrdr->parser->labeltovalue.size()  + 2;
			Rcpp::List extralist( list_sz );//include hdr values here also!!!
			Rcpp::CharacterVector extranames( list_sz );	//include hdr values here also!!!
			spcrdr->parser->arrange_data( extranames, extralist, 1 );
					//SORT OUT WARNINGS!!!
			if(spcrdr->parser->want_log_but_no_log == 1){
			//	Rcpp::Rcout << "WARNING: want log but no log" << endl;
			}//fi
			Rcpp::DataFrame data1 = Rcpp::DataFrame::create(extralist);
			Rcpp::List label = Rcpp::List::create( Rcpp::Named( ".wavelength" ) = Rcpp::ExpressionVector( SPC_reader::get_FX( spcrdr->hdr.fxtype ) ),
												   Rcpp::Named( "spc" )  = Rcpp::ExpressionVector( SPC_reader::get_FY( spcrdr->hdr.fytype ) ),
												   Rcpp::Named("z") = Rcpp::ExpressionVector( SPC_reader::get_FX( spcrdr->hdr.fxtype ) ),
												   Rcpp::Named("z.end") = Rcpp::ExpressionVector( SPC_reader::get_FX( spcrdr->hdr.fxtype ) ) );
			Rcpp::Language hyObj_call( "new", "hyperSpec",  Rcpp::Named("spc") =  Y, Rcpp::Named("wavelength") =  X,  Rcpp::Named("data") = data1, Rcpp::Named("labels") = label ); //log = log, label = lab

			delete spcrdr;
			return hyObj_call.eval();
			break;

		}
		case ( TMULTI | TXVALS ) :{
			Rcpp::Rcout << "Case: " << spcrdr->hdr.ftflgs << endl;
			if( spcrdr->hdr.fnsub < 1 ){
				Rcpp::Rcout << "FLAG FNSUB ERROR HERE" << endl;
				break;
			}
			Rcpp::NumericMatrix Y(spcrdr->hdr.fnsub, spcrdr->hdr.fnpts );
			spcrdr->parser = new TMULTI_TXVALS_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, X.begin(), Y.begin(), &log2data, &hdr2data );
			spcrdr->parser->parse_file();
			//spcrdr->print_SPCHDR();
			short list_sz = spcrdr->parser->labeltovalue.size()  + 2;
			Rcpp::List extralist( list_sz );//include hdr values here also!!!
			Rcpp::CharacterVector extranames( list_sz );	//include hdr values here also!!!
			spcrdr->parser->arrange_data( extranames, extralist, spcrdr->hdr.fnsub );
								//SORT OUT WARNINGS!!!
			if(spcrdr->parser->want_log_but_no_log == 1){
			//	Rcpp::Rcout << "WARNING: want log but no log" << endl;
			}//fi

			Rcpp::DataFrame data1 = Rcpp::DataFrame::create(extralist);
			Rcpp::List label = Rcpp::List::create( Rcpp::Named( ".wavelength" ) = Rcpp::ExpressionVector( SPC_reader::get_FX( spcrdr->hdr.fxtype ) ),
															   Rcpp::Named( "spc" )  = Rcpp::ExpressionVector( SPC_reader::get_FY( spcrdr->hdr.fytype ) ),
															   Rcpp::Named("z") = Rcpp::ExpressionVector( SPC_reader::get_FX( spcrdr->hdr.fxtype ) ),
															   Rcpp::Named("z.end") = Rcpp::ExpressionVector( SPC_reader::get_FX( spcrdr->hdr.fxtype ) ) );
			Rcpp::Language hyObj_call( "new", "hyperSpec",  Rcpp::Named("spc") =  Y, Rcpp::Named("wavelength") =  X,  Rcpp::Named("data") = data1, Rcpp::Named("labels") = label ); //log = log, label = lab

			delete spcrdr;
			return hyObj_call.eval();
			break;
		}
		//CASE: MULTI X's SEPARATE SPECTRA

		case ( TXYXYS | TXVALS ) :{
			SUBHDR subhdr;
			spcrdr->ifstr.read( (char*) &subhdr, SUBHSZ );
			Rcpp::NumericVector X( subhdr.subnpts );
			Rcpp::NumericMatrix Y(1, subhdr.subnpts );
			spcrdr->parser = new TXYXYS_TXVALS_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, X.begin(), Y.begin(), &log2data, &hdr2data, subhdr );

			spcrdr->parser->parse_file();
			short list_sz = spcrdr->parser->labeltovalue.size()  + 2;
			Rcpp::List extralist( list_sz );//include hdr values here also!!!
			Rcpp::CharacterVector extranames( list_sz );	//include hdr values here also!!!
			spcrdr->parser->arrange_data( extranames, extralist, spcrdr->hdr.fnsub );
					//SORT OUT WARNINGS!!!
			if(spcrdr->parser->want_log_but_no_log == 1){
				//Rcpp::Rcout << "WARNING: want log but no log" << endl;
			}//fi

			Rcpp::DataFrame data1 = Rcpp::DataFrame::create(extralist);
			Rcpp::List label = Rcpp::List::create( Rcpp::Named( ".wavelength" ) = Rcpp::ExpressionVector( SPC_reader::get_FX( spcrdr->hdr.fxtype ) ),
															   Rcpp::Named( "spc" )  = Rcpp::ExpressionVector( SPC_reader::get_FY( spcrdr->hdr.fytype ) ),
															   Rcpp::Named("z") = Rcpp::ExpressionVector( SPC_reader::get_FX( spcrdr->hdr.fxtype ) ),
															   Rcpp::Named("z.end") = Rcpp::ExpressionVector( SPC_reader::get_FX( spcrdr->hdr.fxtype ) ) );
			Rcpp::Language hyObj_call( "new", "hyperSpec",  Rcpp::Named("spc") =  Y, Rcpp::Named("wavelength") =  X,  Rcpp::Named("data") = data1, Rcpp::Named("labels") = label ); //log = log, label = lab

			delete spcrdr;
			return hyObj_call.eval();
			break;
		}
		case (TMULTI | TXYXYS | TXVALS) :{

			spcrdr->parser = new TMULTI_TXYXYS_TXVALS_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, &log2data, &hdr2data );
			//by default list is ordered
			Rcpp::List hyperSpecList( spcrdr->hdr.fnsub );
			Rcpp::NumericVector directory( spcrdr->hdr.fnsub );
			for(unsigned int i = 0; i < spcrdr->hdr.fnsub; ++i ){
				directory(i) = i+1;
			}

			SUBHDR subhdr;
			for(unsigned int i = 0; i < spcrdr->hdr.fnsub; ++i){
				spcrdr->ifstr.read( (char*) &subhdr, SUBHSZ );

				Rcpp::NumericVector Xv( subhdr.subnpts );
				Rcpp::NumericMatrix Y( 1, subhdr.subnpts );

				spcrdr->parser->parse_file( Xv.begin(), Y.begin(), subhdr.subnpts );
				Rcpp::List label = Rcpp::List::create( Rcpp::Named( ".wavelength" ) = Rcpp::ExpressionVector( SPC_reader::get_FX( spcrdr->hdr.fxtype ) ),
																   Rcpp::Named( "spc" )  = Rcpp::ExpressionVector( SPC_reader::get_FY( spcrdr->hdr.fytype ) ),
																   Rcpp::Named("z") = Rcpp::ExpressionVector( SPC_reader::get_FX( spcrdr->hdr.fxtype ) ),
																   Rcpp::Named("z.end") = Rcpp::ExpressionVector( SPC_reader::get_FX( spcrdr->hdr.fxtype ) ) );
				hyperSpecList[i] = Rcpp::List::create( Rcpp::Named( "spc" ) = Y, Rcpp::Named( "wavelength" ) =  Xv, Rcpp::Named("labels") = label);
			}//rof
			//NB: Should I flag an error here when subnpts is set????

			if( spcrdr->parser->has_directory() ){
				spcrdr->parser->parse_directory( directory.begin() );
			}

			short list_sz = spcrdr->parser->labeltovalue.size()  + 2;
			Rcpp::List extralist( list_sz );//include hdr values here also!!!
			Rcpp::CharacterVector extranames( list_sz );
			spcrdr->parser->arrange_data( extranames, extralist, 1 );	//NBNB: Sort out log2gdr for here!! -> can I add it later?
			if(spcrdr->parser->want_log_but_no_log == 1){
				//	Rcpp::Rcout << "WARNING: want log but no log" << endl;
			}//fi
			Rcpp::DataFrame data1 = Rcpp::DataFrame::create( extralist );

			delete spcrdr;
			return Rcpp::List::create( Rcpp::Named("spcs") = hyperSpecList, Rcpp::Named("data") = data1, Rcpp::Named("idx") = directory);

			break;
		}

		default :{
			Rcpp::Rcout << "No action. Case: " << (int) spcrdr->hdr.ftflgs << endl;
			delete spcrdr;
			return 0;
			break;
		}

	}


	Rcpp::Rcout <<"closing main"<<endl;


	END_RCPP
}


//#endif
