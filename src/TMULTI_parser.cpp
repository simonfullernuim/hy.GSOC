/*
 * TMULTI_parser.cpp
 *
 *  Created on: 24 May 2012
 *      Author: simon
 */

#include "TMULTI_parser.h"

void TMULTI_parser::parse_file(){
	read_proc_subhdr( subhdr );
	subexps.push_back( subhdr.subexp );
	readX( reader_hdr->fnpts );
	unsigned int i = 0;
	for( ; i < reader_hdr->fnsub-1; ++i ){
		readY( i, reader_hdr->fnpts, subhdr.subexp );	//use subexp for scaling
		read_proc_subhdr( subhdr );
		subexps.push_back( subhdr.subexp );
	}
	readY( i, reader_hdr->fnpts, subhdr.subexp );	//do not read subhdr after last read
	uncollected = read_log();

}

