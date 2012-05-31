/*
 * SPC_reader.cpp
 *
 *  Created on: 16 May 2012
 *      Author: simon
 */

#include "SPC_reader.h"

using namespace std;
//http://stackoverflow.com/questions/2078290/efficient-way-to-process-simple-but-large-files-in-c  --> cf. double buffers

//RE: LINKER NOT FINDING CLASS
//http://web.cs.swarthmore.edu/~newhall/unixhelp/debuggingtips_C.html
void SPC_reader::read_proc_hdr(){
		//read hdr in one go
		ifstr.read((char*) &hdr, SPCHSZ);
		//ifstr.rdbuf()->sgetn( buffer, SPCHSZ );	//read 1st SPCHSZ bytes...

		//process ftflgs
		isTSPREC = (bool) TSPREC & hdr.ftflgs;	//check if TSPREC == 1
		isTORDRD = (bool) TORDRD & hdr.ftflgs;	//check if TORDRD
		if(isTSPREC){
			tsprec_subval = 16;
		}
		/*
		switch( hdr.ftflgs ){
			//Single File, Evenly Spaced X Values
			case 0:{ cout << 0 << endl;
					 //parser = new Basic_parser( &ifstr, &hdr, tsprec_subval );
					 break; }
			//Single File, Unevenly Spaced X Values
			case TXVALS: cout << 1 << endl; break;
			//MULTIFILE - Z values Evenly Spaced??
			case TMULTI: cout << 2 << endl; break;// | TORDRD subcase
			case ( TMULTI | TRANDM ): cerr << "TMULTI | TRANDM: TRANDM is not used.\n ftflgs: " << hdr.ftflgs << endl; break;
			//Multifile, Unevenly Spaced X Values, Common X Array
			case ( TMULTI | TXVALS ): cout << 4 << endl; break; //|TORDRD sub case
			//Multifile, Unevenly Spaced X Values, Unique X arrays..
			case ( TMULTI | TXYXYS | TXVALS ): cout << 5 << endl; break;	//|TORDRD sub case
			default: cerr << "Invalid File Format - ftflgs: " << hdr.ftflgs << endl; break;
		}//hctiws
		*/
		if( (hdr.ftflgs & TALABS) == TALABS ){
			isTALABS = 1;
			/*
			int i = 0;
			while(i < 30 && hdr.fcatxt[i]!='\0'){
				hdr.fxtype[i] = hdr.fcatxt[i];
			}
			++i;//skip null
			while(i < 30 && hdr.fcatxt[i]!='\0'){
				hdr.fytype[i] = hdr.fcatxt[i];
			}
			++i;
			while(i < 30 && hdr.fcatxt[i]!='\0'){
				hdr.fztype[i] = hdr.fcatxt[i];
			}
			++i;
			*/
			// FWTYPE?????

		}

}//cnuf

//#define TSPREC	1	/* Single precision (16 bit) Y data if set. */
//#define TCGRAM	2
//#define TMULTI	4
//#define TRANDM	8
//#define TORDRD	16
//#define TALABS	32
//#define TXYXYS	64
//#define TXVALS	128


void SPC_reader::print_SPCHDR(){
		int c = 1;
		cout << c++ << ": " << (short)hdr.ftflgs << endl;;
		//read fversn
		cout << c++ << ": " << (short)hdr.fversn << endl;
		cout << c++ << ": " << (short)hdr.fexper << endl;
		cout << c++ << ": " << hdr.fexp << endl;
		cout << c++ << ": " << (DWORD) hdr.fnpts << endl;
		cout << c++ << ": " << (double) hdr.ffirst << endl;
		cout << c++ << ": " << (double) hdr.flast << endl;
		cout << c++ << ": " << (DWORD) hdr.fnsub << endl;
		cout << c++ << ": " << (short) hdr.fxtype << endl;
		cout << c++ << ": " << (short) hdr.fytype << endl;
		cout << c++ << ": " << (short) hdr.fztype << endl;
		cout << c++ << ": " << (short) hdr.fpost << endl;
		cout << c++ << ": " << (DWORD) hdr.fdate << endl;

		cout << c++ << ": ";
		for(int i = 0; i < 8; ++i){
			cout << hdr.fres[i] << " ";
		}
		cout << endl;
		cout << c++ << ": ";
		for(int i =0; i < 8; ++i){
			cout << hdr.fsource[i] << " ";
		}
		cout << endl;

		cout << c++ << ": " << (short) hdr.fpeakpt << endl;

		cout << c++ << ": ";
		for(int i =0; i < 8; ++i){
			cout << hdr.fspare[i] << " ";
		}
		cout << endl;
		cout << c++ << ": ";
		for(int i =0; i < 129; ++i){
			cout << hdr.fcmnt[i] << " ";
		}
		cout << endl;
		cout << c++ << ": ";
		for(int i =0; i < 29; ++i){
			cout << hdr.fcatxt[i] << " ";
		}
		cout << endl;

		cout << c++ << ": " << (DWORD) hdr.flogoff << endl;
		cout << c++ << ": " << (DWORD) hdr.fmods << endl;
		cout << c++ << ": " << (short) hdr.fprocs << endl;
		cout << c++ << ": " << (short) hdr.flevel << endl;
		cout << c++ << ": " << (short) hdr.fsampin << endl;
		cout << c++ << ": " << (float) hdr.ffactor << endl;

		cout << c++ << ": ";
		for(int i =0; i < 48; ++i){
			cout << hdr.fmethod[i] << " ";
		}
		cout << endl;

		cout << c++ << ": " << (float) hdr.fzinc << endl;
		cout << c++ << ": " << (DWORD) hdr.fwplanes << endl;
		cout << c++ << ": " << (float) hdr.fwinc << endl;
		cout << c++ << ": " << (short) hdr.fwtype << endl;
}
