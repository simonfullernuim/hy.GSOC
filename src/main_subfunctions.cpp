/*
 * main_subfunctions.cpp
 *
 *  Created on: 11 Jun 2012
 *      Author: simon fuller
 */

#include "main_subfunctions.h"

using namespace std;

void switch_flag( BYTE& hdrflag, BYTE& switch_flg, bool& TORDRD_case, bool& TALABS_case){
	if( (hdrflag & TSPREC) == TSPREC ){
		switch_flg = switch_flg - 1;
	}
	if( (hdrflag & TORDRD) == TORDRD ){
		TORDRD_case = 1;
		switch_flg = switch_flg - TORDRD;
	}
	if( (hdrflag & TALABS) == TALABS ){
		TALABS_case = 1;
		switch_flg = switch_flg - TALABS;
	}
}//cnuf


string convert_to_str(int number, stringstream& ss ){
   ss.clear();
   ss.str("");
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}//cnuf

string convert_to_str(double number, stringstream& ss ){
   ss.clear();
   ss.str("");
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

string convert_to_str( std::string str ){ return str; }
