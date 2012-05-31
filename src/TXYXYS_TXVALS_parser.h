/*
 * TXYXYS_TXVALS_parser.h
 *
 *  Created on: 30 May 2012
 *      Author: simon
 */

#ifndef TXYXYS_TXVALS_PARSER_H_
#define TXYXYS_TXVALS_PARSER_H_

#include "Super_parser.h"

class TXYXYS_TXVALS_parser : public Super_parser{
private:
protected:
public:
	TXYXYS_TXVALS_parser( std::ifstream* ipt_ifstr, SPC* ipt_reader_hdr,  short ipt_tsprec_subval,
				Rcpp::NumericVector::iterator ipt_X_ptr, Rcpp::NumericMatrix::iterator ipt_Y_ptr, std::vector< std::string >* ipt_log2data, SUBHDR _subhdr )
				: Super_parser( ipt_ifstr, ipt_reader_hdr, ipt_tsprec_subval, ipt_X_ptr, ipt_Y_ptr,	 ipt_log2data ) {
			memcpy( &subhdr, &_subhdr, SUBHSZ );
			int Y_len = reader_hdr->fnpts;
	}

	void parse_file(){
		readX( subhdr.subnpts );
		readY( 0, subhdr.subnpts, subhdr.subexp);//reverse order to pass ms.soc

		if(subhdr.subnpts!=0){
				//ERROR HANDLING
				/*
				 * RETURN AN ERROR IN DATA? IF NOT PROCESSED??
				 */
			}

	}
};



#endif /* TXYXYS_TXVALS_PARSER_H_ */
