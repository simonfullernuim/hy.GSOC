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


