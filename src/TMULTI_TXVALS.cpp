/*
 * TMULTI_TXVALS.cpp
 *
 *  Created on: 23 May 2012
 *      Author: simon
 */
#include "TMULTI_TXVALS_parser.h"


void TMULTI_TXVALS_parser::parse_file(){
	read_proc_subhdr( subhdr );
	readX( reader_hdr->fnpts );

	for(unsigned int i = 0; i < reader_hdr->fnsub; ++i){
		read_proc_subhdr( subhdr );
		readY( i, reader_hdr->fnpts, subhdr.subexp );	//use subexp for scaling
	}
	uncollected = read_log();
}


