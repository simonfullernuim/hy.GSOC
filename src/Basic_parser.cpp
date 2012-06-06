
#include "Basic_parser.h"

void Basic_parser::parse_file(){

	read_proc_subhdr( subhdr );
	readX( reader_hdr->fnpts );
	readY( 0, reader_hdr->fnpts, reader_hdr->fexp );
	read_log();
}

void Basic_parser::readX(int amt ){
	double mult = ( reader_hdr->flast - reader_hdr->ffirst )/(reader_hdr->fnpts - 1);
	double val = reader_hdr->ffirst;
	for(int i = 0; i < amt; i++){
		*X_ptr =  val;
		++X_ptr;
		val += mult;
	}//rof
}//cnuf

