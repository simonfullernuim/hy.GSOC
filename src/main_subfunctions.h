/*
 * main_subfunctions.h
 *
 *  Created on: 11 Jun 2012
 *      Author: simon fuller
 */

#ifndef MAIN_SUBFUNCTIONS_H_
#define MAIN_SUBFUNCTIONS_H_

#include <sstream>

#include "WTypes.h"
#include "SPC.h"

void switch_flag( BYTE& hdrflag, BYTE& switch_flg, bool& TORDRD_case, bool& TALABS_case);
std::string convert_to_str( int number, std::stringstream& ss );
std::string convert_to_str( double number, std::stringstream& ss );
std::string convert_to_str( std::string str );
#endif /* MAIN_SUBFUNCTIONS_H_ */
