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
#include <Rcpp.h>
//#include <RInside.h>
#include <R.h>
using namespace std;




/*
 * RcppExport SEXP rcpp_read_spc( string _file, vector<string> _hdr2data, vector<string> _log2data,
		bool log_txt = true, bool log_bin = false, bool log_disk = false, bool no_object = false ){
 */
RcppExport SEXP rcpp_read_spc( SEXP _file, SEXP _hdr2data, SEXP _log2data,
		SEXP _log_txt, SEXP _log_bin, SEXP _log_disk, SEXP _no_object ){
	//hdr = list (), // <--- include this!!!
	BEGIN_RCPP
	string file = Rcpp::as<string>(_file);
	vector<string> hdr2data = Rcpp::as< vector< string > >( _hdr2data );
	vector<string> log2data = Rcpp::as< vector< string > >( _log2data );
	bool log_txt = (bool) Rcpp::as< int >( _log_txt );
	bool log_bin = (bool) Rcpp::as< int >( _log_bin );
	bool log_disk = (bool) Rcpp::as< int >( _log_disk );
	bool no_object = (bool) Rcpp::as< int >( _no_object );


	ifstream test;
	//string file = _file;
	//file = "/home/simon/College/GSOC/spc_sdk/Data/m_xyxy.spc";	// todo: 245 TSPREC | 4 | 16 | TALABS | TXYXYS | TXVALS
	//file = "/home/simon/College/GSOC/spc_sdk/Data/RAMAN.SPC";	//PASSED: Account for TALABS
	//file = "/home/simon/College/GSOC/spc_sdk/Data/nir.spc"; //PASSED: Attach Warnings()
	//file = "/home/simon/College/GSOC/spc_sdk/Data/ms.spc";		// PASSED I THINK - integer Y values - should these be X - anyway it parses intelligibly: 225: TSPREC | TALABS | TXYXYS | TXVALS
	//file = "/home/simon/College/GSOC/spc_sdk/Data/s_xy.spc";	//TXVALS	-> PROVIDE ERROR HANDLING for SUBNPTS != NULL
	//file = "/home/simon/College/GSOC/spc_sdk/Data/s_evenx.spc";	//DEFAULT ->PASSED
	//file = "/home/simon/College/GSOC/spc_sdk/Data/NMR_FID.SPC";	//PASSED WL and SPC
	//file = "/home/simon/College/GSOC/spc_sdk/Data/NMR_SPC.SPC";	//PASSED WL and SPC
	//file = "/home/simon/College/GSOC/spc_sdk/Data/Ft-ir.spc";


	SPC_reader* spcrdr = new SPC_reader( file );
	spcrdr->read_proc_hdr();
	Rcpp::NumericVector X( spcrdr->hdr.fnpts );
	bool TORDRD_case =0;
	bool TALABS_case = 0;
	/*
	 * Okay, TORDRD does not seem to do anything thus far: so I am coralling it here and taking it out of the current switch statement..
	 * This is where we stop that happpening.
	 */
	DWORD switch_flg = spcrdr->hdr.ftflgs;
	if( (spcrdr->hdr.ftflgs & TSPREC) == TSPREC ){
		switch_flg = switch_flg - 1;
	}
	if( (spcrdr->hdr.ftflgs & TORDRD) == TORDRD ){
		TORDRD_case = 1;
		switch_flg = switch_flg - TORDRD;
	}
	if( (spcrdr->hdr.ftflgs & TALABS) == TALABS ){
		TALABS_case = 1;
		switch_flg = switch_flg - TALABS;
	}


	/* NBNBNBB:
	 * Probably nicer to roll this out into a discrimination net when finished..
	 */
	//switch( spcrdr->hdr.ftflgs ){	//we use polymorphism, but the matrices must be passed in from outside
	switch( switch_flg ){
		case 0 :{
			//cout << "Case: " << (int) spcrdr->hdr.ftflgs << endl;
			Rcpp::NumericMatrix Y(1, spcrdr->hdr.fnpts );
			spcrdr->parser = new Basic_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, X.begin(), Y.begin(), &log2data, &hdr2data );
			spcrdr->parser->parse_file();
			//spcrdr->print_SPCHDR();
			short list_sz = spcrdr->parser->labeltovalue.size()  + 2;
			Rcpp::List extralist( list_sz );//include hdr values here also!!!
			Rcpp::CharacterVector extranames( list_sz );	//include hdr values here also!!!
			spcrdr->parser->arrange_data( extranames, extralist, 1 );//1 dimension
			//SORT OUT WARNINGS!!!
			if(spcrdr->parser->want_log_but_no_log == 1){
				cout << "WARNING: want log but no log" << endl;
			}//fi
			Rcpp::Language licall( "print", extralist );
			licall.eval();

			Rcpp::DataFrame data1 = Rcpp::DataFrame::create( extralist );
			Rcpp::Language dfcall("print",data1);
			dfcall.eval();

			Rcpp::Language hyObj_call( "new", "hyperSpec",  Rcpp::Named("spc") =  Y, Rcpp::Named("wavelength") =  X,  Rcpp::Named("data") = data1 ); //log = log, label = lab
			return hyObj_call.eval();


			break;
		}//esac
		case TMULTI :{
			//Rprintf("Case: %i\n",spcrdr->hdr.ftflgs);
			Rcpp::NumericMatrix Y(spcrdr->hdr.fnsub, spcrdr->hdr.fnpts );
			spcrdr->parser = new TMULTI_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, X.begin(), Y.begin(), &log2data, &hdr2data );
			spcrdr->parser->parse_file();
			//spcrdr->print_SPCHDR();
			/*
			cout << "Subexps: " << endl;
			for( vector<int>::const_iterator it = spcrdr->parser->subexps.begin(); it != spcrdr->parser->subexps.end(); ++it){
				cout << *it << endl;
			}//rof
			*/
			short list_sz = spcrdr->parser->labeltovalue.size()  + 2;
			Rcpp::List extralist( list_sz );//include hdr values here also!!!
			Rcpp::CharacterVector extranames( list_sz );	//include hdr values here also!!!
			spcrdr->parser->arrange_data( extranames, extralist, spcrdr->hdr.fnsub );
					//SORT OUT WARNINGS!!!
			if(spcrdr->parser->want_log_but_no_log == 1){
				cout << "WARNING: want log but no log" << endl;
			}//fi


			Rcpp::DataFrame data1 = Rcpp::DataFrame::create(extralist);
			Rcpp::Language hyObj_call( "new", "hyperSpec",  Rcpp::Named("spc") =  Y, Rcpp::Named("wavelength") =  X,  Rcpp::Named("data") = data1 ); //log = log, label = lab
			return hyObj_call.eval();
			break;
		}

		case TXVALS :{
			cout << "Case: " << (int) spcrdr->hdr.ftflgs << endl;
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
				cout << "WARNING: want log but no log" << endl;
			}//fi
			//NB: Should I flag an error here when subnpts is set????
			//Rprintf("SUBNPTS: %i\n", spcrdr->parser->subhdr.subnpts);

			Rcpp::DataFrame data1 = Rcpp::DataFrame::create(extralist);
			Rcpp::Language hyObj_call( "new", "hyperSpec",  Rcpp::Named("spc") =  Y, Rcpp::Named("wavelength") =  X,  Rcpp::Named("data") = data1 ); //log = log, label = lab
			return hyObj_call.eval();
			break;

		}
		case ( TMULTI | TXVALS ) :{
			cout << "Case: " << spcrdr->hdr.ftflgs << endl;
			if( spcrdr->hdr.fnsub < 1 ){
				cout << "FLAG FNSUB ERROR HERE" << endl;
				break;
			}
			Rcpp::NumericMatrix Y(spcrdr->hdr.fnsub, spcrdr->hdr.fnpts );
			spcrdr->parser = new TMULTI_TXVALS_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, X.begin(), Y.begin(), &log2data, &hdr2data );
			spcrdr->parser->parse_file();
			spcrdr->print_SPCHDR();
			short list_sz = spcrdr->parser->labeltovalue.size()  + 2;
			Rcpp::List extralist( list_sz );//include hdr values here also!!!
			Rcpp::CharacterVector extranames( list_sz );	//include hdr values here also!!!
			spcrdr->parser->arrange_data( extranames, extralist, spcrdr->hdr.fnsub );
								//SORT OUT WARNINGS!!!
			if(spcrdr->parser->want_log_but_no_log == 1){
				cout << "WARNING: want log but no log" << endl;
			}//fi

			Rcpp::DataFrame data1 = Rcpp::DataFrame::create(extralist);
			Rcpp::Language hyObj_call( "new", "hyperSpec",  Rcpp::Named("spc") =  Y, Rcpp::Named("wavelength") =  X,  Rcpp::Named("data") = data1 ); //log = log, label = lab
			return hyObj_call.eval();
			break;
		}
		//CASE: MULTI X's SEPARATE SPECTRA

		case ( TXYXYS | TXVALS ) :{
			//CHICKEN AND EGG PROBLEM HERE...
			SUBHDR subhdr;
			spcrdr->ifstr.read( (char*) &subhdr, SUBHSZ );
			Rcpp::NumericVector X( subhdr.subnpts );
			Rcpp::NumericMatrix Y(1, subhdr.subnpts );
			spcrdr->parser = new TXYXYS_TXVALS_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, X.begin(), Y.begin(), &log2data, &hdr2data, subhdr );

			spcrdr->parser->parse_file();
			//spcrdr->print_SPCHDR();
			short list_sz = spcrdr->parser->labeltovalue.size()  + 2;
			Rcpp::List extralist( list_sz );//include hdr values here also!!!
			Rcpp::CharacterVector extranames( list_sz );	//include hdr values here also!!!
			spcrdr->parser->arrange_data( extranames, extralist, spcrdr->hdr.fnsub );
					//SORT OUT WARNINGS!!!
			if(spcrdr->parser->want_log_but_no_log == 1){
				cout << "WARNING: want log but no log" << endl;
			}//fi

			Rcpp::DataFrame data1 = Rcpp::DataFrame::create(extralist);
			Rcpp::Language hyObj_call( "new", "hyperSpec",  Rcpp::Named("spc") =  Y, Rcpp::Named("wavelength") =  X,  Rcpp::Named("data") = data1 ); //log = log, label = lab
			return hyObj_call.eval();
			break;
		}
		case (TMULTI | TXYXYS | TXVALS) :{

			spcrdr->parser = new TMULTI_TXYXYS_TXVALS_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, &log2data, &hdr2data );
			//by default list is ordered
			Rcpp::List hyperSpecList( spcrdr->hdr.fnsub + 1 );	//extra slot for directory
			Rcpp::NumericVector directory( spcrdr->hdr.fnsub );
			for(unsigned int i = 0; i < spcrdr->hdr.fnsub; ++i ){
				directory(i) = i+1;
			}
			hyperSpecList[0] = directory;
			for(unsigned int i = 1; i <= spcrdr->hdr.fnsub; ++i){
				SUBHDR subhdr;
				spcrdr->ifstr.read( (char*) &subhdr, SUBHSZ );

				Rcpp::NumericVector Xv( subhdr.subnpts );
				Rcpp::NumericMatrix Y( 1, subhdr.subnpts );
				Rcpp::DataFrame df = Rcpp::DataFrame::create( Rcpp::Named( "spc" ) = Rcpp::NumericVector(1), Rcpp::Named( "wavelength") = Rcpp::NumericVector( subhdr.subnpts) );

				spcrdr->parser->parse_file( Xv.begin(), Y.begin(), subhdr.subnpts );

				df["spc"] = Y;
				df["wavelength"] = Xv;//change when running from R

				short list_sz = spcrdr->parser->labeltovalue.size()  + 2;
				Rcpp::List extralist( list_sz );//include hdr values here also!!!
				Rcpp::CharacterVector extranames( list_sz );	//include hdr values here also!!!
				spcrdr->parser->arrange_data( extranames, extralist, 1 );
									//SORT OUT WARNINGS!!!
				if(spcrdr->parser->want_log_but_no_log == 1){
				//	cout << "WARNING: want log but no log" << endl;
				}//fi
				Rcpp::DataFrame data1 = Rcpp::DataFrame::create(extralist);
				Rcpp::Language hyObj_call( "new", "hyperSpec",  Rcpp::Named("spc") =  Y, Rcpp::Named("wavelength") =  Xv,  Rcpp::Named("data") = data1 );
				hyperSpecList[i] = hyObj_call.eval();	//BE AWARE THAT THIS CLONES Y VALUES AT THE MOMENT...DUE TO UNREALISTIC DF Structure

				//cout << "completed: " << i+1 << endl;
			}//rof
			/*
			if( spcrdr->parser->has_directory() ){
				spcrdr->parser->parse_directory( directory.begin() );
				hyperSpecList[spcrdr->hdr.fnpts] = directory;
			}
			*/
			/*
			Rcpp::List::iterator lit = hyperSpecList.begin()+1;
			for( ; lit != hyperSpecList.end(); ++lit ){
				Rcpp::Language sumcall("summary", *lit);
				SEXP s = sumcall.eval();
				Rcpp::Language prints("print", s);
				prints.eval();
			}
			Rcpp::Language callv("eval", *hyperSpecList.begin());
			Rcpp::Language printv("print", callv.eval());
			printv.eval();
			cout << endl;
			*/
			return hyperSpecList;
			break;
		}

		default :{
			cout << "No action. Case: " << (int) spcrdr->hdr.ftflgs << endl;
			break;
		}

	}

	delete spcrdr;

	cout <<"closing main"<<endl;

	SEXP blah;
	return blah;

	END_RCPP
}



//#endif

