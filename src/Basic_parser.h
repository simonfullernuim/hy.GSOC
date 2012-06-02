/*
 * basic_parser.h
 *
 *  Created on: 19 May 2012
 *      Author: simon
 */

#ifndef BASIC_PARSER_H_
#define BASIC_PARSER_H_

#include <Rcpp.h>
//#include <RInside.h>
//#include <R.h>

#include <fstream>
#include <iostream>

#include "Super_parser.h"
#include "SPC_reader.h"

class Basic_parser : public Super_parser{
private:
public:
	//Rcpp::NumericMatrix xmat;
	int Y_len;
	//Rcpp::NumericMatrix ymat;
	//NEW PLAN: --> pass more info by reference..
	Basic_parser( std::ifstream* ipt_ifstr, SPC* ipt_reader_hdr,  short ipt_tsprec_subval,
			Rcpp::NumericVector::iterator ipt_X_ptr, Rcpp::NumericMatrix::iterator ipt_Y_ptr, std::vector< std::string >* ipt_log2data, std::vector< std::string >* ipt_hdr2data )
			: Super_parser( ipt_ifstr, ipt_reader_hdr, ipt_tsprec_subval, ipt_X_ptr, ipt_Y_ptr,	 ipt_log2data, ipt_hdr2data ) {
		int Y_len = reader_hdr->fnpts;
	}
	void parse_file();
	virtual ~Basic_parser(){}
	virtual void readX(int amt, short row = 0 );

	//virtual Rcpp::NumericMatrix* getX(){ return &X; }
	//virtual Rcpp::NumericMatrix* getY(){ return &Y;}
/*
	void print_Y(){
		for(int i = 0; i < Y_len; ++i ){
		//	std::cout << Y(0,i) << std::endl;
		}
	}
*/
};


#endif /* BASIC_PARSER_H_ */
