/*
 * TXVALS_parser.cpp
 *
 *  Created on: 23 May 2012
 *      Author: simon
 */

#include "TXVALS_parser.h"


void TXVALS_parser::parse_file(){
	readX( reader_hdr->fnpts );
	read_proc_subhdr( subhdr );
	if(subhdr.subnpts!=0){
		//ERROR HANDLING
		/*
		 * RETURN AN ERROR IN DATA? IF NOT PROCESSED??
		 */
	}

	readY( 0, reader_hdr->fnpts, reader_hdr->fexp);
	uncollected = read_log();
}


