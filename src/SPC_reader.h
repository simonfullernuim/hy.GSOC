/*
 * spc_reader.h
 *
 *  Created on: 11 May 2012
 *      Author: simon
 *      "Only 'new' spc files (1996 file format) with LSB word order are supported." - Cf read.spc+
 */
#ifndef SPC_READER_H_
#define SPC_READER_H_

#define DSIZE 8

#include <stdio.h>
#include <Rcpp.h>
#include <string>
#include <iostream>
#include <fstream>
#include <limits.h>

#include "SPC.h"
#include "Super_parser.h"

class SPC_reader{

private:
protected:
public:
	static const std::string FXTYPE[];
	static const std::string FYTYPE[];

	SPCHDR hdr;
	std::ifstream ifstr;
	std::string file_loc;

	Super_parser * parser;
	short file_type;
	char buffer[LINE_MAX];
	bool isTSPREC;	//i.e. has 16-bit int blocks
	short tsprec_subval;
	bool isTORDRD;
	bool isTALABS;


	SPC_reader( std::string ipt_file_loc, short ipt_file_type = 1 ){// : file_loc( ipt_file_loc ), file_type( ipt_file_type ){
		file_loc = ipt_file_loc;
		file_type = ipt_file_type;
		tsprec_subval = 32; //unless told otherwise..
		ifstr.open( ipt_file_loc.c_str(), std::ios_base::in | std::ios_base::binary );
	}
	SPC_reader(){}
	~SPC_reader(){
		ifstr.close();//is this okay with default constructor??
		delete parser;
	}

	void read_proc_hdr();
	void read_rest_of_file(){
		parser->parse_file();
	}

	static std::string get_FX( int i ){
		return ( i >= 0 && i < 31 ) ? FXTYPE[i] : "NA";
	}

	static std::string get_FY( int i ){
		return ( i >= -1 && i < 31 ) ? FYTYPE[i+1] : ( ( i >= 128 && i <= 131 ) ? FYTYPE[i-100] : "NA" );
	}

	bool checkNativeEndianness(){
		ushort a=0x1234;
		if (*((unsigned char *)&a)==0x12)
			return BIG_ENDIAN;
		else
			return LITTLE_ENDIAN;
	}

	void print_SPCHDR();
};

#endif /* SPC_READER_H_ */
