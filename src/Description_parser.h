/*
 * Description_parser.h
 *
 *  Created on: 31 May 2012
 *      Author: simon
 */

#ifndef DESCRIPTION_PARSER_H_
#define DESCRIPTION_PARSER_H_

#include <fstream>

class Description_parser{
private:
protected:
	bool has_Directory;
	std::ifstream* ifstr;
public:
	Description_parser( std::ifstream* _ifstr ){
		ifstr = _ifstr;
	}
	void parse_description( Rcpp::NumericVector description_vector ){
		if(!has_Directory) return;

	}
};



#endif /* DESCRIPTION_PARSER_H_ */
