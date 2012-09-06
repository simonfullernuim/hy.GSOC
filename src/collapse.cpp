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
	map<double,int> wl_map;
	map<string, int> extra_data_map;
	Rcpp::List hyObjList( _hyObjList );
	Rcpp::List::iterator lit;
	string tmp_str;
	vector<int> num_spec_rows;	//num spectra in each object
	int total_num_spec_rows = 0;
	for( lit = hyObjList.begin(); lit != hyObjList.end(); ++lit ){
		Rcpp::S4 tmp( (SEXP) *lit );
		Rcpp::NumericVector wl = tmp.slot( "wavelength" );
		Rcpp::Language datanames( "names", tmp.slot( "data" ) );
		Rcpp::CharacterVector charnames( datanames.eval() );	//stores data strings..
		for( Rcpp::CharacterVector::iterator cit = charnames.begin(); cit != charnames.end(); ++cit ){
			Rcpp::CharacterVector tmpcv( *cit );
			tmp_str = Rcpp::as< string >( tmpcv );
			++extra_data_map[tmp_str];
		}//rof
		for( Rcpp::NumericVector::iterator it = wl.begin(); it != wl.end(); ++it ){
			++wl_map[*it];
		}//rof
		Rcpp::Language nrow( "nrow", tmp );
		int r = Rcpp::as< int >( nrow.eval() );
		num_spec_rows.push_back(r);
		total_num_spec_rows += r;
	}//rof
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
	int nrow = total_num_spec_rows;		//if we have several spcs for some objects we can simply count them when iterating through wavelengths by dimension calls - should be fairly cheap
	int ncol = wl_map.size();
	int ndata = extra_data_map.size();
	Rcpp::NumericMatrix spc( nrow, ncol );
	Rcpp::NumericMatrix extra_data( nrow, ndata );

	int pos, insert_pos;
	int mat_idx = 0;
	vector<int>::const_iterator nrow_it = num_spec_rows.begin();
	for( lit = hyObjList.begin(); lit != hyObjList.end(); ++lit ){
		Rcpp::S4 tmp( (SEXP) *lit);
		Rcpp::DataFrame df2 = tmp.slot( "data" );		//only one spectrum...
		Rcpp::List label = tmp.slot( "label" );
		Rcpp::NumericMatrix spec = df2["spc"];

		Rcpp::NumericVector wl = tmp.slot( "wavelength" );
		for( int j = 0; j < *nrow_it; ++j ){
			for( int i = 0; i < ncol; ++i ){
				spc( mat_idx + j, i ) = NA_REAL;
			}
			for( int i = 0; i < wl.size(); ++i ){
				insert_pos = wl_map[ wl(i) ];	//assigning NAs here rather than separate loop is preferable, in that some cases might not be sparse, and have few NAs
				spc( mat_idx + j, insert_pos ) = spec( j, i );//NB assuming only one row in each spec - this can be changed
			}//rof
		}//rof

		Rcpp::Language datanames2( "names", df2 );

		Rcpp::CharacterVector data( datanames2.eval() );

		pos = 0;
		for( Rcpp::CharacterVector::iterator cit = data.begin(); cit != data.end(); ++cit ){

			Rcpp::CharacterVector tmpcv(*cit);
			tmp_str = Rcpp::as< string >( tmpcv );
			Rcpp::NumericVector datavec = df2[ tmp_str ];
			insert_pos = extra_data_map[ tmp_str ];

			while( pos < insert_pos ){
				extra_data( mat_idx, pos ) = NA_REAL;
				++pos;
			}//elihw
			if( tmp_str != "spc" ){
				for( int i = 0; i < *nrow_it; ++i ){	//add row for all spectra
					extra_data( mat_idx + i, insert_pos ) = datavec( i );
				}//rof
			}//fi
			++pos;
		}//rof
		mat_idx += *nrow_it;
	}//rof
	Rcpp::DataFrame df( extra_data );
	df.names() = datanames;
	Rcpp::Language ob_call( "new", "hyperSpec", Rcpp::Named( "spc" ) = spc, Rcpp::Named( "wavelength" ) = wavelength, Rcpp::Named( "data" ) = df);
	return ob_call;

	END_RCPP
}//cnuf


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
