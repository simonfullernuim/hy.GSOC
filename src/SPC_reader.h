/*
 * spc_reader.h
 *
 *  Created on: 11 May 2012
 *      Author: simon
 *      "Only 'new' spc files (1996 file format) with LSB word order are supported." - Cf read.spc+
 */
#ifndef SPC_READER_H_
#define SPC_READER_H_

#ifndef LINE_MAX
#define LINE_MAX _POSIX2_LINE_MAX
#endif

#define DSIZE 8

#include <stdio.h>
//#include <R.h>
#include <Rcpp.h>
#include <string>
#include <iostream>
#include <fstream>
#include <limits.h>	//http://stackoverflow.com/questions/3552095/sensible-line-buffer-size-in-c       --> for buffer size

#include "SPC.h"
#include "Super_parser.h"
//#include "Basic_parser.h"

//class Super_parser;
//class Basic_parser;
//using namespace Rcpp;

/*
 * read.spc <- function (filename,
		keys.hdr2data = c('fexper', 'fres', 'fsource'), keys.hdr2log = c('fdate', 'fpeakpt'),
		keys.log2data = FALSE, keys.log2log = TRUE,
		log.txt = TRUE, log.bin = FALSE, log.disk = FALSE,
		hdr = list (),
		no.object = FALSE)
 *
 */


class SPC_reader{
	//set RInside for using Rcpp types in debugging
private:
protected:
public:

	SPCHDR hdr;
	//NOTE: SUBHDR is held in the instantiated parse type since some have several SUBHDRS
	std::FILE *fp;
	std::ifstream ifstr;
	std::string file_loc;
	//perhaps some sort of index ptr
	//Rcpp::Rstreambuf rbf;
	Super_parser * parser;
	short file_type;
	char buffer[LINE_MAX];	//should be stack or heap?
	//ftflgs bools for TSPREC and TORDRD
	bool isTSPREC;	//i.e. has 16-bit int blocks		--> "Only if fex[ is no 0x08h"
	short tsprec_subval;
	bool isTORDRD;
	bool isTALABS;

//size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream
//int fgetpos(FILE *stream, fpos_t *pos);
//int fsetpos(FILE *stream, const fpos_t *pos);
//int feof(FILE *stream);

//	http://courses.cs.vt.edu/~cs2604/fall02/binio.html
	//friend class Super_parser;
	//friend class Basic_parser;

	SPC_reader( std::string ipt_file_loc, short ipt_file_type = 1 ){// : file_loc( ipt_file_loc ), file_type( ipt_file_type ){
		file_loc = ipt_file_loc;
		file_type = ipt_file_type;
		tsprec_subval = 32; //unless told otherwise..
		//fp = fopen(ipt_file_loc.c_str(), "r");
		ifstr.open( ipt_file_loc.c_str(), std::ios_base::in | std::ios_base::binary );
	}
	~SPC_reader(){
		//fclose(fp);
		ifstr.close();
		delete parser;
	}

	void read_proc_hdr();
	void read_rest_of_file(){
		parser->parse_file();
	}

	//void print_Y(){ parser->print_Y(); }

	bool checkNativeEndianness(){
		ushort a=0x1234;
		if (*((unsigned char *)&a)==0x12)
			return BIG_ENDIAN;
		else
			return LITTLE_ENDIAN;
	}

	void print_SPCHDR();
	//std::iftream ifs( "filename" );
	//ifs.read( (char*)data, size ); // still "high-level"
	//ifs.rdbuf()->sgetn( (char*)data, size ); // low-level
};

#endif /* SPC_READER_H_ */
