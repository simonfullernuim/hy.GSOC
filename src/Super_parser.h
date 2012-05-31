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
#include <cmath>
#include <map>
#include <utility>
#include <vector>
#include <string>

#include <Rcpp.h>

#include "SPC.h"

//#include "SPC_reader.h"

//class SPC_reader;

class Super_parser{
private:
protected:
	Rcpp::NumericVector::iterator X_ptr;
	Rcpp::NumericMatrix::iterator Y_ptr;
	//std::map< std::string, bool > hdr2data;
	std::map< std::string, bool > log2data;
	std::map< std::string, std::string > log_labeltovalue;

public:
	//SPC_reader * holder_reader;
	std::ifstream* ifstr;
	SPC* reader_hdr;
	SUBHDR subhdr;
	std::vector<int> subexps;
	LOGSTC logstc;
	short tsprec_subval;
	short prec_shifts;
	char buffer[LINE_MAX];
	bool want_log_but_no_log;
	short logs_to_collect;
	short uncollected;

	Super_parser( std::ifstream* ipt_ifstr, SPC* ipt_reader_hdr, short ipt_tsprec_subval,
		Rcpp::NumericVector::iterator ipt_X_ptr, Rcpp::NumericMatrix::iterator ipt_Y_ptr, std::vector< std::string >* ipt_log2data )
			:  ifstr( ipt_ifstr ), reader_hdr( ipt_reader_hdr ),tsprec_subval( ipt_tsprec_subval ),
			   X_ptr(ipt_X_ptr), Y_ptr(ipt_Y_ptr) {
		prec_shifts = tsprec_subval == 32 ? 5 : 4;
		check_log(ipt_log2data);
	}//rotcurtsnoc
	//2nd ctr - used for TXYXYS..
	Super_parser( std::ifstream* ipt_ifstr, SPC* ipt_reader_hdr, short ipt_tsprec_subval, std::vector< std::string >* ipt_log2data )
		:  ifstr( ipt_ifstr ), reader_hdr( ipt_reader_hdr ),tsprec_subval( ipt_tsprec_subval ){
		prec_shifts = tsprec_subval == 32 ? 5 : 4;
		check_log(ipt_log2data);

	}

	void check_log( std::vector< std::string >* check_log2data ){
		if(check_log2data->size()>0){
			if( reader_hdr->flogoff == 0 ){
				want_log_but_no_log = 1;
			}//fi
			else{
				want_log_but_no_log = 0;
				logs_to_collect = check_log2data->size();
				for( std::vector< std::string >::const_iterator it = check_log2data->begin(); it != check_log2data->end(); ++it ){
					log2data.insert( std::make_pair(*it, 1) );
				}//rof
			}//esle
		}//fi
		else{
			logs_to_collect = 0;
		}//esle
	}

	void read_proc_subhdr( SUBHDR& shr ){
		ifstr->read( (char*) &shr, SUBHSZ );
	}
	virtual void parse_file() = 0;	//abstraction -> although e.g. TXVALS_parser could stand in here
	void parse_file( Rcpp::NumericVector::iterator _X_ptr, Rcpp::NumericMatrix::iterator _Y_ptr, unsigned int _npts ){
		X_ptr = _X_ptr;
		Y_ptr = _Y_ptr;
		subhdr.subnpts = _npts;
		parse_file();
	}
	virtual ~Super_parser(){}
	virtual void readX( int amt = 0);
	void readY( short row, int amt, char fexp );
	/*
	void read_proc_logstc(){
		ifstr->read( (char*) &logstc, LOGSTCSZ );
	}
	*/
	short read_log();//DO
	void print_stored_log();
	void read_process_subhdr();
	//virtual Rcpp::NumericMatrix* getX() = 0;
	//virtual Rcpp::NumericMatrix* getY() = 0;

	//virtual void print_Y() = 0;
};

#endif


