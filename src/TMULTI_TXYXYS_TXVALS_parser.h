/*
 * TMULTI_TXYXYS_TXVALS_parser.h
 *
 *  Created on: 31 May 2012
 *      Author: simon
 */

#ifndef TMULTI_TXYXYS_TXVALS_PARSER_H_
#define TMULTI_TXYXYS_TXVALS_PARSER_H_

#include "Super_parser.h"

class TMULTI_TXYXYS_TXVALS_parser : public Super_parser{
private:
protected:
public:


	TMULTI_TXYXYS_TXVALS_parser( std::ifstream* ipt_ifstr, SPC* ipt_reader_hdr, short ipt_tsprec_subval, std::vector< std::string >* ipt_log2data, std::vector< std::string >* ipt_hdr2data )
		: Super_parser ( ipt_ifstr, ipt_reader_hdr, ipt_tsprec_subval, ipt_log2data, ipt_hdr2data ){
		hasDirectory = (reader_hdr->fnpts == 0 ) ? false : true;
	}
	virtual ~TMULTI_TXYXYS_TXVALS_parser(){}

	void parse_file(){
		readX( subhdr.subnpts );
		readY( 0, subhdr.subnpts, subhdr.subexp, 1 );
	}

};


#endif /* TMULTI_TXYXYS_TXVALS_PARSER_H_ */
