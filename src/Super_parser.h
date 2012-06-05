/*
 * super_parser.h
 *
 *  Created on: 16 May 2012
 *      Author: simon
 */

#ifndef SUPER_PARSER_H
#define SUPER_PARSER_H

#define BUFFER32 512
#define BUFFER16 1024

#include <fstream>
#include <sstream>
#include <cmath>
#include <map>
#include <utility>
#include <vector>
#include <string>

#include <Rcpp.h>

#include "SPC.h"

class Super_parser{
private:
protected:
	Rcpp::NumericVector::iterator X_ptr;
	Rcpp::NumericMatrix::iterator Y_ptr;
	std::map< std::string, bool > log2data;
	std::map<std::string, std::string >hdr_map;
public:
	std::ifstream* ifstr;
	SPC* reader_hdr;
	SUBHDR subhdr;
	SSFTC current_ssftc;
	std::vector<int> subexps;
	LOGSTC logstc;
	short tsprec_subval;
	short prec_shifts;
	char buffer[LINE_MAX];
	bool want_log_but_no_log;
	short logs_to_collect;
	short uncollected;
	std::map< std::string, std::string > labeltovalue;	////these involve a slight overhead in copying the hdr strings, but it is minor since amount is small


	bool hasDirectory;

	Super_parser( std::ifstream* ipt_ifstr, SPC* ipt_reader_hdr, short ipt_tsprec_subval,
		Rcpp::NumericVector::iterator ipt_X_ptr, Rcpp::NumericMatrix::iterator ipt_Y_ptr, std::vector< std::string >* ipt_log2data, std::vector< std::string >* ipt_hdr2data )
			:  ifstr( ipt_ifstr ), reader_hdr( ipt_reader_hdr ),tsprec_subval( ipt_tsprec_subval ),
			   X_ptr(ipt_X_ptr), Y_ptr(ipt_Y_ptr) {
		prec_shifts = tsprec_subval == 32 ? 5 : 4;
		get_hdr_data(ipt_hdr2data);
		check_log(ipt_log2data);
	}//rotcurtsnoc
	//2nd ctr - used for TXYXYS..
	Super_parser( std::ifstream* ipt_ifstr, SPC* ipt_reader_hdr, short ipt_tsprec_subval, std::vector< std::string >* ipt_log2data, std::vector< std::string >* ipt_hdr2data )
		:  ifstr( ipt_ifstr ), reader_hdr( ipt_reader_hdr ),tsprec_subval( ipt_tsprec_subval ){
		prec_shifts = tsprec_subval == 32 ? 5 : 4;
		get_hdr_data(ipt_hdr2data);
		check_log(ipt_log2data);
	}

	void check_log( std::vector< std::string >* check_log2data );

	void read_proc_subhdr( SUBHDR& shr ){
		ifstr->read( (char*) &shr, SUBHSZ );
	}
	virtual void parse_file(){}	//abstraction -> although e.g. TXVALS_parser could stand in here
	void parse_file( Rcpp::NumericVector::iterator _X_ptr, Rcpp::NumericMatrix::iterator _Y_ptr, unsigned int _npts ){
		X_ptr = _X_ptr;
		Y_ptr = _Y_ptr;
		subhdr.subnpts = _npts;
		parse_file();
	}
	virtual ~Super_parser(){}
	virtual void readX( int amt = 0);
	/*
	 * By default offset is reader_hdr->fnsubs --> for TMULTI_TXYXYS_TXVALS this is set to 1
	 */
	void readY(short row, int amt, char fexp, unsigned int _offset = 0);

	void parse_directory( Rcpp::NumericVector::iterator it );
	bool has_directory(){return hasDirectory;}

	short read_log();//DO
	void print_stored_log();
	void read_process_subhdr();
	void set_hdr_map();
	std::string get_fexper(short fexper);
	void get_hdr_data(std::vector< std::string>* ipt_hdr2data);
	void arrange_data( Rcpp::CharacterVector& extranames, Rcpp::List& extralist, short dimension );

	std::string convert_to_str(int n, std::stringstream& ss );
	std::string convert_to_str(double n, std::stringstream& ss);
	std::string convert_to_str(std::string str){ return str; }
};

#endif


