/*
 * TMULTI_parser.h
 *
 *  Created on: 24 May 2012
 *      Author: simon
 */

#ifndef TMULTI_PARSER_H_
#define TMULTI_PARSER_H_

#include "Basic_parser.h"

class TMULTI_parser : public Basic_parser{
private:
protected:
public:

	TMULTI_parser( std::ifstream* ipt_ifstr, SPC* ipt_reader_hdr, short ipt_tsprec_subval,
		Rcpp::NumericVector::iterator ipt_X_ptr, Rcpp::NumericMatrix::iterator ipt_Y_ptr, std::vector< std::string >* ipt_log2data, std::vector< std::string >* ipt_hdr2data )
		: Basic_parser( ipt_ifstr, ipt_reader_hdr, ipt_tsprec_subval, ipt_X_ptr, ipt_Y_ptr,	 ipt_log2data, ipt_hdr2data ) {
			int Y_len = reader_hdr->fnpts;
	}
	virtual ~TMULTI_parser(){}
	void parse_file();
};



#endif /* TMULTI_PARSER_H_ */
