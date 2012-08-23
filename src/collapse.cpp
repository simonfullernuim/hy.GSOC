/*
 * collapse.cpp
 *
 *  Created on: 8 Jun 2012
 *      Author: simon fuller
 */

#include <map>
#include <string>
#include <iostream>
#include <Rcpp.h>
#include <Rmath.h>
#include "SPC.h"
#include "SPC_reader.h"	//avail of SPC_reader's static methods
#include "main_subfunctions.h"

using namespace std;

std::map<std::string, std::string >hdr_map;

RcppExport SEXP collapse( SEXP _hyObjList ){
	BEGIN_RCPP

	//SPC_reader * blank_spc_reader = new SPC_reader();
	map<double,int> wl_map;
	map<string, int> extra_data_map;
	Rcpp::List hyObjList( _hyObjList );
	Rcpp::List::iterator lit;
	string tmp_str;

	for( lit = hyObjList.begin(); lit != hyObjList.end(); ++lit ){
		Rcpp::S4 tmp( (SEXP) *lit );							//dirty cast to SEXP - only way? - all objects are validated in R prior to calling rcpp.collapse()
		Rcpp::NumericVector wl = tmp.slot( "wavelength" );
		Rcpp::Language datanames( "names", tmp.slot( "data" ) );
		Rcpp::CharacterVector charnames( datanames.eval() );	//stores data strings..
		for( Rcpp::CharacterVector::iterator cit = charnames.begin(); cit != charnames.end(); ++cit ){
			Rcpp::CharacterVector tmpcv( *cit );
			tmp_str = Rcpp::as< string >( tmpcv );	//here
			++extra_data_map[tmp_str];
		}//rof
		for( Rcpp::NumericVector::iterator it = wl.begin(); it != wl.end(); ++it ){
			++wl_map[*it];
		}//rof
	}//rof
	//index wavelengths to position in spc matrix
	Rcpp::NumericVector wavelength( wl_map.size() );
	int map_idx = 0;
	for( map<double,int>::iterator mit = wl_map.begin(); mit != wl_map.end(); ++mit ){
		mit->second = map_idx;
		wavelength( map_idx ) = mit->first;	//store wavelengths here
		++map_idx;
	}//rof
	extra_data_map.erase( "spc" );
	Rcpp::CharacterVector datanames( extra_data_map.size() );
	map_idx = 0;
	for( map<string,int>::iterator mit = extra_data_map.begin(); mit != extra_data_map.end(); ++mit ){
		mit->second = map_idx;
		datanames( map_idx ) = mit->first;
		++map_idx;
	}//rof
	int nrow = hyObjList.size();		//if we have several spcs for some objects we can simply count them when iterating through wavelengths by dimension calls - should be fairly cheap
	int ncol = wl_map.size();
	int ndata = extra_data_map.size();
	Rcpp::NumericMatrix spc( nrow, ncol );
	Rcpp::NumericMatrix extra_data( nrow, ndata );

	int pos, insert_pos;
	int mat_idx = 0;

	for( lit = hyObjList.begin(); lit != hyObjList.end(); ++lit ){
		Rcpp::S4 tmp( (SEXP) *lit);
		Rcpp::DataFrame df2 = tmp.slot( "data" );		//only one spectrum...
		Rcpp::List label = tmp.slot( "label" );
		Rcpp::NumericMatrix spec = df2["spc"];
		Rcpp::NumericVector wl = tmp.slot( "wavelength" );
		pos = 0;
		for( int i = 0; i < wl.size(); ++i ){
			insert_pos = wl_map[ wl(i) ];	//assigning NAs here rather than separate loop is preferable, in that some cases might not be sparse, and have few NAs
			while( pos < insert_pos ){
				spc( mat_idx, pos ) = NA_REAL;
				++pos;
			}//elihw
			spc( mat_idx, wl_map[ wl(i) ] ) = spec( 0, i );//NB assuming only one row in each spec - this can be changed
			++pos;
		}//rof
		Rcpp::Language datanames2( "names", df2 );
		Rcpp::CharacterVector data( datanames2.eval() );

		pos = 0;
		for( Rcpp::CharacterVector::iterator cit = data.begin(); cit != data.end(); ++cit ){
			Rcpp::CharacterVector tmpcv(*cit);
			tmp_str = Rcpp::as< string >( tmpcv );
			insert_pos = extra_data_map[ tmp_str ];
			while( pos < insert_pos ){
				extra_data( mat_idx, pos ) = NA_REAL;
				++pos;
			}//elihw
			if( tmp_str != "spc" ){
							//this only works for the numerics - change
				extra_data( mat_idx, insert_pos ) = Rcpp::as<double>(df2[ tmp_str ]);
			}//fi
			++pos;
		}//rof
		++mat_idx;
	}//rof
	Rcpp::DataFrame df( extra_data );
	df.names() = datanames;
	Rcpp::Language ob_call( "new", "hyperSpec", Rcpp::Named( "spc" ) = spc, Rcpp::Named( "wavelength" ) = wavelength, Rcpp::Named( "data" ) = df);
	return ob_call;

	END_RCPP
}//cnuf


//Putting these here for the moment - if we have more similar functions I will share them in a separate header

inline string convert_to_str(int number, stringstream& ss ){
   ss.clear();
   ss.str("");
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}//cnuf

inline string convert_to_str(double number, stringstream& ss ){
   ss.clear();
   ss.str("");
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

inline std::string convert_to_str( std::string str ){ return str; }

void set_hdr_map(){//nb check char versions..
	stringstream ss;
	hdr_map.insert(std::make_pair("ftflgs", ""));
	hdr_map.insert(std::make_pair("fversn", ""));
	hdr_map.insert(std::make_pair("fexper", ""));
	hdr_map.insert(std::make_pair("fexp", ""));
	hdr_map.insert(std::make_pair("fnpts", ""));
	hdr_map.insert(std::make_pair("ffirst", ""));
	hdr_map.insert(std::make_pair("flast", ""));
	hdr_map.insert(std::make_pair("fnsub", ""));
	hdr_map.insert(std::make_pair("fxtype", ""));
	hdr_map.insert(std::make_pair("fytype", ""));
	hdr_map.insert(std::make_pair("fztype", ""));
	hdr_map.insert(std::make_pair("fpost", ""));
	hdr_map.insert(std::make_pair("fdate", ""));
	hdr_map.insert(std::make_pair("fres",  ""));//     null terminated[]
	hdr_map.insert(std::make_pair("fsource", ""));// 	null terminated[]
	hdr_map.insert(std::make_pair("fpeakpt", ""));

	hdr_map.insert(std::make_pair("fspare", ""));//float[8]
	hdr_map.insert(std::make_pair("fcmnt", ""));//	null terminated[]

	hdr_map.insert(std::make_pair("fcatxt", ""));//[] fcatxt has 3 values separated by nulls
	hdr_map.insert(std::make_pair("flogoff", ""));
	hdr_map.insert(std::make_pair("fmods", ""));
	hdr_map.insert(std::make_pair("fprocs", ""));
	hdr_map.insert(std::make_pair("flevel", ""));
	hdr_map.insert(std::make_pair("fsampin", ""));
	hdr_map.insert(std::make_pair("ffactor", ""));
	hdr_map.insert(std::make_pair("fmethod", ""));//[]
	hdr_map.insert(std::make_pair("fzinc", ""));
	hdr_map.insert(std::make_pair("fwplanes", ""));
	hdr_map.insert(std::make_pair("fwinc", ""));
	hdr_map.insert(std::make_pair("fwtype", ""));
	// hdr_map.insert(std::make_pair("freserv", freserv)));	//reserved
}
