#ifdef INSIDE

#define BUFFER 1024

/*
 * 	Inheritance: Super_Class --> Basic_parser --> TMULTI_parser
 * 							 --> TXVALS_parser
 *							 --> TMULTI_TXVALS_parser
 *
 *							 Well, a 3D CUDA implemented circular buffer
also asynchronous multithreaded processing
those two are the keys to it
 */


/*
 * Ancilliary data: Label: TALABS==1 --> then we use the data in hdr...
 * So we can return: e.g.
 *
 */

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <vector>

#include "WTypes.h"
#include "SPC.h"
#include "SPC_reader.h"
#include "Super_parser.h"
#include "Basic_parser.h"
#include "TXVALS_parser.h"
#include "TMULTI_parser.h"
#include "TMULTI_TXVALS_parser.h"
#include "TXYXYS_TXVALS_parser.h"
#include "TMULTI_TXYXYS_TXVALS_parser.h"
#include <Rcpp.h>
#include <RInside.h>
#include <R.h>
using namespace std;

/*
 *  ALT-SHIFT-W for Project Explorer
 */
/*
 *
 * setClass("example",
    representation  (
           size = "numeric",
           id    = "character"
    )
)

fx <- cxxfunction( signature(x = "example"),
'
     S4 obj(x) ;
     obj.slot( "size" ) = 10 ;
     obj.slot( "id"   ) = "foo" ;
     return obj ;
', plugin = "Rcpp" )

str( fx( new("example", size=4, id="id_value") ) )
 *
 */
int main(){
//	RInside R;
	//multithreaded IO
	//http://www.drdobbs.com/go-parallel/article/220300055?pgno=3


	//http://www.dreamincode.net/forums/topic/170054-understanding-and-reading-binary-files-in-c/
	//http://www.dreamincode.net/forums/topic/60185-manipulate-raw-binary-data/
	//http://stackoverflow.com/questions/9812155/convert-from-unsigned-char-array-to-double
	//rcpp_hello_world();


	/*
	 * I found the answer! And it's embarrassingly simple.

The problem was that I was using the Release version of SDL instead of the Debug version! (I had 'libsdl' from MacPorts whereas I should have had 'libsdl-devel'.)

So my generic answer is: make sure the libs you're linking against were compiled with debug flags set too, it's not always enough to just make sure your own code has them set.

Cheers!

thoughton.
	 */
	cout << (int) ( (char) 128 ) << endl;

	cout << ( TMULTI | TRANDM ) << endl;
			//Multifile, Unevenly Spaced X Values, Common X Array
	cout << ( TMULTI | TXVALS ) << endl; //|TORDRD sub case

	//Multifile, Unevenly Spaced X Values, Unique X arrays..
	cout << ( TMULTI | TXYXYS | TXVALS ) << endl;

	cout << ( TSPREC | TCGRAM | TMULTI | TRANDM | TORDRD | TALABS | TXYXYS | TXVALS ) << endl;


	//SEXP XOb;
	//Rcpp::S4 Obj(XOb);


	ifstream test;
	string file;
	//file = "/home/simon/College/GSOC/spc_sdk/Data/m_xyxy.spc";	// todo: 245 TSPREC | 4 | 16 | TALABS | TXYXYS | TXVALS
	//file = "/home/simon/College/GSOC/spc_sdk/Data/RAMAN.SPC";	//PASSED: Account for TALABS
	//file = "/home/simon/College/GSOC/spc_sdk/Data/nir.spc"; //PASSED: Attach Warnings()
	file = "/home/simon/College/GSOC/spc_sdk/Data/ms.spc";		// PASSED I THINK - integer Y values - should these be X - anyway it parses intelligibly: 225: TSPREC | TALABS | TXYXYS | TXVALS
	/*
	 * INPUT INFO FROM R
	 */
	//file = "/home/simon/College/GSOC/spc_sdk/Data/s_xy.spc";	//TXVALS	-> PROVIDE ERROR HANDLING for SUBNPTS != NULL
	//file = "/home/simon/College/GSOC/spc_sdk/Data/s_evenx.spc";	//DEFAULT ->PASSED
	//file = "/home/simon/College/GSOC/spc_sdk/Data/NMR_FID.SPC";	//PASSED WL and SPC
	//file = "/home/simon/College/GSOC/spc_sdk/Data/NMR_SPC.SPC";	//PASSED WL and SPC
	//file = "/home/simon/College/GSOC/spc_sdk/Data/Ft-ir.spc";
	vector<string> hdr2data; vector<string> log2data;

	SPC_reader* spcrdr = new SPC_reader( file );
	spcrdr->read_proc_hdr();
	cout << "TYPE: " << (int) (spcrdr->hdr.ftflgs &  255 )<< endl;
	Rcpp::NumericVector X( spcrdr->hdr.fnpts );
	bool TORDRD_case =0;
	bool TALABS_case = 0;
	/*
	 * Okay, TORDRD does not seem to do anything thus far: so I am coralling it here and taking it out of the current switch statement..
	 * This is where we stop that happpening.
	 */
	DWORD switch_flg = spcrdr->hdr.ftflgs;
	if( (spcrdr->hdr.ftflgs & TSPREC) == TSPREC ){
		switch_flg = switch_flg - 1;
	}
	if( (spcrdr->hdr.ftflgs & TORDRD) == TORDRD ){
		TORDRD_case = 1;
		switch_flg = switch_flg - TORDRD;
	}
	if( (spcrdr->hdr.ftflgs & TALABS) == TALABS ){
		TALABS_case = 1;
		switch_flg = switch_flg - TALABS;
	}


	/* NBNBNBB:
	 * Probably nicer to roll this out into a discrimination net when finished..
	 */
	//switch( spcrdr->hdr.ftflgs ){	//we use polymorphism, but the matrices must be passed in from outside
	switch( switch_flg ){
		case 0 :{
			cout << "Case: " << (int) spcrdr->hdr.ftflgs << endl;
			Rcpp::NumericMatrix Y(1, spcrdr->hdr.fnpts );
			spcrdr->parser = new Basic_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, X.begin(), Y.begin(), &log2data );

			spcrdr->parser->parse_file();
			spcrdr->print_SPCHDR();
			cout << endl;
			cout << "X values:" << endl;

			for(unsigned int i = 0; i < spcrdr->hdr.fnpts; ++i ){
				cout <<  setprecision(12) << X(i) << endl;
			}//rof
			cout << endl;
			int n;
			cout << "enter key:" << endl;
			cin >> n;
			cout << "Y values" << endl;
			for(unsigned int i = 0; i < spcrdr->hdr.fnpts; ++i ){
				cout <<  setprecision(12) << Y(0,i) << endl;
			}//rof
			cout << endl;

			break;
		}//esac
		case TMULTI :{
			cout << "Case: " << spcrdr->hdr.ftflgs << endl;
			Rcpp::NumericMatrix Y(spcrdr->hdr.fnsub, spcrdr->hdr.fnpts );
			spcrdr->parser = new TMULTI_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, X.begin(), Y.begin(), &log2data );
			spcrdr->parser->parse_file();
			spcrdr->print_SPCHDR();
			cout << endl;
			cout << "Y values" << endl;
			for(unsigned int i = 0; i < spcrdr->hdr.fnsub; ++i){
				cout << "row: " << i + 1 << endl;
				for(unsigned int j = 0; j < spcrdr->hdr.fnpts; ++j ){
					cout << j+1 <<": " << setprecision(12) << Y(i,j) << endl;
				}//rof
				int n;
				cout << "enter key:" << endl;
				cin >> n;
				cin.ignore();
				cout << n << endl;

				cin.clear();
			}//rof
			cout << endl;
			cout << "X values:" << endl;
			for(unsigned int i = 0; i < spcrdr->hdr.fnpts; ++i ){
				cout <<  setprecision(12) << X(i) << endl;
			}//rof
			cout << "Subexps: " << endl;
			for( vector<int>::const_iterator it = spcrdr->parser->subexps.begin(); it != spcrdr->parser->subexps.end(); ++it){
				cout << *it << endl;
			}//rof
			break;
		}

		case TXVALS :{
			cout << "Case: " << (int) spcrdr->hdr.ftflgs << endl;
			Rcpp::NumericMatrix Y(1, spcrdr->hdr.fnpts );
			spcrdr->parser = new TXVALS_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, X.begin(), Y.begin(), &log2data );
			spcrdr->parser->parse_file();
			spcrdr->print_SPCHDR();
			cout << endl;
			cout << "X values:" << endl;
			for(unsigned int i = 0; i < spcrdr->hdr.fnpts; ++i ){
				cout <<  setprecision(12) << X(i) << endl;
			}//rof
			cout << "Y values:" << endl;
			for(unsigned int i = 0; i < spcrdr->hdr.fnpts; ++i ){
				cout <<  setprecision(12) << Y(0,i) << endl;
			}//rof

			break;

		}
		case ( TMULTI | TXVALS ) :{
			cout << "Case: " << spcrdr->hdr.ftflgs << endl;
			if( spcrdr->hdr.fnsub < 1 ){
				cout << "FLAG FNSUB ERROR HERE" << endl;
				break;
			}
			Rcpp::NumericMatrix Y(spcrdr->hdr.fnsub, spcrdr->hdr.fnpts );
			spcrdr->parser = new TMULTI_TXVALS_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, X.begin(), Y.begin(), &log2data );
			spcrdr->parser->parse_file();
			spcrdr->print_SPCHDR();
			for( unsigned int i = 0; i < spcrdr->hdr.fnsub; ++i ){
				for( unsigned int j = 0; j < spcrdr->hdr.fnpts; ++j ){
					cout <<  setprecision(12) << Y(i,j) << endl;
				}//rof
				cout << endl;
			}
			cout << endl;
			cout << "X values:" << endl;
			for(unsigned int i = 0; i < spcrdr->hdr.fnpts; ++i ){
				cout <<  setprecision(12) << X(i) << endl;
			}//rof
			break;
		}
		//CASE: MULTI X's SEPARATE SPECTRA

		case ( TXYXYS | TXVALS ) :{
			//CHICKEN AND EGG PROBLEM HERE...
			SUBHDR subhdr;
			spcrdr->ifstr.read( (char*) &subhdr, SUBHSZ );
			Rcpp::NumericVector X( subhdr.subnpts );
			Rcpp::NumericMatrix Y(1, subhdr.subnpts );
			spcrdr->parser = new TXYXYS_TXVALS_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, X.begin(), Y.begin(), &log2data, subhdr );

			spcrdr->parser->parse_file();
			spcrdr->print_SPCHDR();
			cout << endl;
			cout << "X values:" << endl;
			for(unsigned int i = 0; i < subhdr.subnpts; ++i ){
				cout << (i+1) << ": " << setprecision(12) << X(i) << endl;
			}//rof
			cout << "Y values:" << endl;
			for(unsigned int i = 0; i < subhdr.subnpts; ++i ){
				cout << (i+1) << ": " <<  setprecision(12) << Y(0,i) << endl;
			}//rof
			break;
		}
		case (TMULTI | TXYXYS | TXVALS) :{
			//PLAN: if we use DIRECTORY - fetch linearly, but store in linked list according to directory structure
			Rcpp::List hyperSpecList;
			spcrdr->parser = new TMULTI_TXYXYS_TXVALS_parser( &spcrdr->ifstr, &spcrdr->hdr, spcrdr->tsprec_subval, &log2data );

			for(unsigned int i = 0; i < spcrdr->hdr.fnsub; ++i){
				SUBHDR subhdr;
				spcrdr->ifstr.read( (char*) &subhdr, SUBHSZ );
				Rcpp::NumericVector X(subhdr.subnpts);
				Rcpp::NumericMatrix Y(1, subhdr.subnpts );
				spcrdr->parser->parse_file( X.begin(), Y.begin(), subhdr.subnpts );

			}
			if()
			break;

		}

		default :{
			cout << "No action. Case: " << (int) spcrdr->hdr.ftflgs << endl;
			break;
		}

	}

	delete spcrdr;
	/*
	char* input, *output;

	FILE* fin, *fout;

	char* buffer;
	size_t read;

	input = "/home/simon/College/GSOC/spc_sdk/Data/RAMAN.spc";
	output = "/home/simon/College/GSOC/test.txt";

	fin = fopen( input, "r");
	if (!fin)
		return 1;
	fout = fopen( output, "w");
	if (!fout)
		return 1;
	buffer = new char[BUFFER];

	while (!feof(fin)) {
		read = fread(buffer, 1, BUFFER, fin);
	    if (!read)
	    	break;
	    for (size_t i = 0; i < read; ++i) {
	       // buffer[i] = buffer[i];
	    }
	    fwrite(buffer, 1, read, fout);
	    }
	    fclose(fin);
	    fclose(fout);

	*/
	cout <<"closing main"<<endl;
	return 1;
}


void ReadFile(char *name)
{
	FILE *file;
	char *buffer;
	unsigned long fileLen;

	//Open file
	file = fopen(name, "rb");
	if (!file)
	{
		fprintf(stderr, "Unable to open file %s", name);
		return;
	}

	//Get file length
	fseek(file, 0, SEEK_END);
	fileLen=ftell(file);
	fseek(file, 0, SEEK_SET);

	//Allocate memory
	buffer=(char *)malloc(fileLen+1);
	if (!buffer)
	{
		fprintf(stderr, "Memory error!");
                                fclose(file);
		return;
	}

	//Read file contents into buffer
	fread(buffer, fileLen, 1, file);
	fclose(file);

	//Do what ever with buffer

	free(buffer);
}

#endif
