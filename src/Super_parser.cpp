/*
 * Super_parser.cpp
 *
 *  Created on: 16 May 2012
 *      Author: simon
 */


#include "Super_parser.h"
using namespace std;


void Super_parser::check_log( std::vector< std::string >* check_log2data ){
	if(check_log2data->size()>0){
		if( reader_hdr->flogoff == 0 ){
			want_log_but_no_log = 1;
		}//fi
		else{
			want_log_but_no_log = 0;
			logs_to_collect = check_log2data->size();
			for( std::vector< std::string >::const_iterator it = check_log2data->begin(); it != check_log2data->end(); ++it ){
				log2data.insert( std::make_pair(*it, 1) );
			}//rof
		}//esle
	}//fi
	else{
		logs_to_collect = 0;
	}//esle
}//cnuf


void Super_parser::readX( int amt ){

	int ctr = 0;
	//short step = tsprec_subval / 8;
	short step = 4;
	int ipt_bytes = amt * step;
	int mod_bytes = ipt_bytes % LINE_MAX;

	int idx = 0;
	double tmp;
	//Rcpp::NumericVector::iterator xptr = X->begin();
	/*
		//ifstr->read( buffer, LINEMAX);
		for(short i = 0; i < amt; i++){
		//	cout << "ctr: " << ctr << endl;
			ifstr->read( (char*)&tmp, step );
			*xptr = (double) tmp * pow((double)2, reader_hdr->fexp - tsprec_subval);
			++xptr
			//ctr=ctr+1;
		}//rof

	 */
	while((ctr=ctr+LINE_MAX)<=ipt_bytes){
		ifstr->read( buffer, LINE_MAX );
		for(short i = 0; i < LINE_MAX / step; ++i){
			memcpy(&tmp, buffer+(i*step), step);
			//X[row][idx++] = (double) tmp * pow((double)2, reader_hdr->fexp - tsprec_subval);
			*X_ptr = tmp;
			++X_ptr;
		}//rof
	}//elihw
	ifstr->read( buffer, mod_bytes );
	for( short i = 0; i < mod_bytes/step; ++i){
		memcpy(&tmp, buffer+(i*step), step);
		//X[row][idx++] = (double) tmp * pow((double)2, reader_hdr->fexp - tsprec_subval);
		*X_ptr = tmp;
		++X_ptr;
	}//rof
}
/*
 * By default offset is reader_hdr->fnsubs --> for TMULTI_TXYXYS_TXVALS this is set to 1
 */


void  Super_parser::readY( short row, int amt, char fexp, unsigned int _offset ){
	unsigned int offset = (_offset == 0) ? reader_hdr->fnsub : _offset;

	Rcpp::NumericMatrix::iterator rdit = ( Y_ptr +  row );
	//unsigned char tfexp = 128;
	int ctr = 0;
	short step = tsprec_subval / 8;
	//short step = 4;
	int ipt_bytes = amt * step;
	int mod_bytes = ipt_bytes % LINE_MAX;
	//int mod_amt = amt % LINE_MAX;
	/*
	 * Other approach:: keep commented out for now
	 */

	/*
	if(fexp!=0x80){
		int tmp;
		//ifstr->read( buffer, LINEMAX);
		for(short i = 0; i < amt; i++){
		//	cout << "ctr: " << ctr << endl;
			ifstr->read( (char*)&tmp, step );
			Y[row][i] = (double) tmp * pow((double)2, reader_hdr->fexp - tsprec_subval);
			//ctr=ctr+1;
		}//rof
	}//fi
	else{
		double tmp;
		for(short i = 0; i < amt; i++){
			ifstr->read( (char*) &Y[row][i], 4 );
				//Y[row,i] = pow((double)tmp, reader_hdr->fexp) / tsprec_subval;
		}//rof
	}//esle
	*/
	if(fexp==(-128)){
		float tmp;
		while((ctr=ctr+LINE_MAX)<=ipt_bytes){
			ifstr->read( buffer, LINE_MAX );
			for(short i = 0; i < LINE_MAX/step; ++i){
				memcpy(&tmp, buffer+(i*4), step);
				*rdit = (double) tmp;
				rdit = rdit + reader_hdr->fnsub;
			}//rof
		}//elihw
		ifstr->read( buffer, mod_bytes );
		for( short i = 0; i < mod_bytes/step; ++i){
			memcpy(&tmp, buffer+(i*step), step);
			*rdit = (double) tmp;
			rdit = rdit + reader_hdr->fnsub;
		}//rof
	}//fi
	else{
		int tmp;
		while((ctr=ctr+LINE_MAX)<=ipt_bytes){
			ifstr->read( buffer, LINE_MAX );
			for	(short i = 0; i < LINE_MAX / step; ++i){
				memcpy(&tmp, buffer+(i*step), step);
				*rdit = (double) tmp * pow((double)2, fexp - tsprec_subval);
				rdit = rdit + reader_hdr->fnsub;
			}//rof
		}//elihw
		ifstr->read( buffer, mod_bytes );
		for( short i = 0; i < mod_bytes/step; ++i){
			memcpy(&tmp, buffer+(i*step), step);
			*rdit = (double) tmp * pow((double)2, fexp - tsprec_subval);
			rdit = rdit + reader_hdr->fnsub;
		}//rof
	}//esle
}





short Super_parser::read_log(){
//seekg(JUMP, ios::cur);
	if( logs_to_collect == 0 || want_log_but_no_log == 1){
		return logs_to_collect;
	}
	ifstr->read( (char*) &logstc, LOGSTCSZ );

	string str;
	string label;
	string value;
	short logs_collected = 0;
	string::iterator it;	//can't use const with assign - makes no sense - assigning from const, not to!!!
	ifstr->seekg(logstc.logtxto - LOGSTCSZ, ios::cur );
	while(logs_collected < logs_to_collect && ifstr->good()){	//check this..
		getline( *ifstr, str, '\n' );//<-DOES THIS WORK??? i.e. will '\n' catch the line break on binary mode??
		it = str.begin();
		while( *it != '=' && it != str.end() ){
			++it;
		}
		if( *(it - 1) == ' '){
			label.assign(str.begin(), it-1);
		}
		else{
			label.assign(str.begin(), it);
		}
		if( log2data.find(label) != log2data.end() ){//if it is in the list..
			value.assign(it+1, str.end());
			if( *(value.end()-1) == 0x0D ){	//\r
				log_labeltovalue.insert( make_pair(label, string(value.begin(), value.end()-1)) );
			}
			else{
				log_labeltovalue.insert( make_pair(label, value) );
			}
			++logs_collected;
		}//fi
	}//elihw
	return (logs_to_collect - logs_collected);	//return the numer of log values not collected
}

//test function - remove
void Super_parser::print_stored_log(){
	for( std::map< std::string, std::string >::const_iterator it = log_labeltovalue.begin(); it != log_labeltovalue.end(); ++it  ){
		std::cout << it->first << ":\t" << it->second << std::endl;
	}
}



void Super_parser::get_hdr_data(std::vector< std::string >* hdr2data){
	set_hdr_map();
	map<string, string >::const_iterator map_it;
	for( std::vector< std::string >::const_iterator it = hdr2data->begin(); it != hdr2data->end(); ++it ){
		map_it = hdr_map.find( *it );
		if( map_it != hdr_map.end() ){
			hdr_labeltovalues.insert( make_pair( map_it->first, map_it->second ));
		}//fi
	}//rof
}//cnuf

void Super_parser::arrange_data(){

}

void Super_parser::parse_directory( Rcpp::NumericVector::iterator description_vector_it ){
	if(!hasDirectory) return;
	unsigned int nsub = reader_hdr->fnsub;
	int * offsets = new int[nsub];
	int * offset_indices = new int[nsub];
	for( unsigned int i = 0; i < nsub; ++i ){	//
		ifstr->read( (char*) &current_ssftc, SSFTCSZ );
		offsets[i] = current_ssftc.ssfposn;
		offset_indices[i] = i + 1;
	}
		/*
		 use a simple insertion sort to sort the offsets and stores the index positions
		  - a quicksort is not much use with small sizes and insertion sort acts well with presorted / partially sorted data
		*/
		int j; int tmp; int tmp_idx;
		for(unsigned int i = 1; i < nsub; ++i ){
			if( offsets[i] < offsets[i-1] ){
				tmp = offsets[i];
				tmp_idx = offset_indices[i];
				j = i;
				do{
					offsets[j] = offsets[j-1];
					offset_indices[j] = offset_indices[j-1];
					--j;
				}while(j>0 && tmp < offsets[j-1]);
				offsets[j] = tmp;
				offset_indices[j] = tmp_idx;
			}
		}
		for( unsigned int i = 0; i < reader_hdr->fnsub; ++i ){
			*description_vector_it = offset_indices[i];
			++description_vector_it;
		}
		delete [] offsets;
		delete [] offset_indices;
	}//cnuf
//make the whole map a string map;
//this is a little more time consuming than we want but:
//a) it avoids a lot of trouble trying to handle the struct types..
//b) it is a constant independent of the size of the data
//c) it is quite small
void Super_parser::set_hdr_map(){//nb check char versions..
	   hdr_map.insert(std::make_pair("ftflgs", convert_to_str(reader_hdr->ftflgs)));
	   hdr_map.insert(std::make_pair("fversn", convert_to_str(reader_hdr->fversn)));
	   hdr_map.insert(std::make_pair("fexper", convert_to_str(reader_hdr->fexper)));
	   hdr_map.insert(std::make_pair("fexp", convert_to_str(reader_hdr->fexp)));
	   hdr_map.insert(std::make_pair("fnpts", convert_to_str((int)reader_hdr->fnpts)));
	   hdr_map.insert(std::make_pair("ffirst", convert_to_str(reader_hdr->ffirst)));
	   hdr_map.insert(std::make_pair("flast", convert_to_str(reader_hdr->flast)));
	   hdr_map.insert(std::make_pair("fnsub", convert_to_str((int)reader_hdr->fnsub)));
	   hdr_map.insert(std::make_pair("fxtype", convert_to_str(reader_hdr->fxtype)));
	   hdr_map.insert(std::make_pair("fytype", convert_to_str(reader_hdr->fytype)));
	   hdr_map.insert(std::make_pair("fztype", convert_to_str(reader_hdr->fztype)));
	   hdr_map.insert(std::make_pair("fpost", convert_to_str(reader_hdr->fpost)));
	   hdr_map.insert(std::make_pair("fdate", convert_to_str((int)reader_hdr->fdate)));
	   string tmp_str(reader_hdr->fres, 8);
	   hdr_map.insert(std::make_pair("fres",  tmp_str ));//     null terminated[]?but is it?
	   tmp_str(reader_hdr->fsource, 8);
	   hdr_map.insert(std::make_pair("fsource", tmp_str));// 	null terminated[]?but is it?
	   hdr_map.insert(std::make_pair("fpeakpt", convert_to_str(reader_hdr->fpeakpt)));
	   string fsp_str = "";
	   for(int i = 0; i < 8; ++i){
		   fsp_str = fsp_str + reader_hdr->fspare[i] + "" + ',';
	   }
	   hdr_map.insert(std::make_pair("fspare", fsp_str));//float[8]
	   tmp_str(reader_hdr->fcmnt,129);
	   hdr_map.insert(std::make_pair("fcmnt", reader_hdr->fcmnt));//	null terminated[]
	   tmp_str = "";
	   for(int i = 0; i < 30; ++i){
		   tmp_str = tmp_str + "" + ( reader_hdr->fcatxt[i] == '\0' ) ? ", " : reader_hdr->fcatxt[i];
	   }
	   hdr_map.insert(std::make_pair("fcatxt", tmp_str));//[] fcatxt has 3 values separated by nulls
	   hdr_map.insert(std::make_pair("flogoff", convert_to_str((int)reader_hdr->flogoff)));
	   hdr_map.insert(std::make_pair("fmods", convert_to_str((int)reader_hdr->fmods)));
	   hdr_map.insert(std::make_pair("fprocs", convert_to_str(reader_hdr->fprocs)));
	   hdr_map.insert(std::make_pair("flevel", convert_to_str(reader_hdr->flevel)));
	   hdr_map.insert(std::make_pair("fsampin", convert_to_str(reader_hdr->fsampin)));
	   hdr_map.insert(std::make_pair("ffactor", convert_to_str(reader_hdr->ffactor)));
	   tmp_str(reader_hdr->fmethod, 48);
	   hdr_map.insert(std::make_pair("fmethod", tmp_str));//[]
	   hdr_map.insert(std::make_pair("fzinc", convert_to_str(reader_hdr->fzinc)));
	   hdr_map.insert(std::make_pair("fwplanes", convert_to_str((int)reader_hdr->fwplanes)));
	   hdr_map.insert(std::make_pair("fwinc", convert_to_str(reader_hdr->fwinc)));
	   hdr_map.insert(std::make_pair("fwtype", convert_to_str(reader_hdr->fwtype)));
	  // hdr_map.insert(std::make_pair("freserv", convert_to_str(reader_hdr->freserv)));
}

string convert_to_str(int number)
{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}
string convert_to_str(double number)
{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}
