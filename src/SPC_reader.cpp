/*
 * SPC_reader.cpp
 *
 *  Created on: 16 May 2012
 *      Author: simon
 */

#include "SPC_reader.h"

using namespace std;

void SPC_reader::read_proc_hdr(){

	ifstr.read((char*) &hdr, SPCHSZ);

		isTSPREC = (bool) TSPREC & hdr.ftflgs;	//check if TSPREC == 1
		isTORDRD = (bool) TORDRD & hdr.ftflgs;	//check if TORDRD
		if(isTSPREC){
			tsprec_subval = 16;
		}
		if( (hdr.ftflgs & TALABS) == TALABS ){
			isTALABS = 1;
		}

}//cnuf

void SPC_reader::print_SPCHDR(){
		int c = 1;
		Rcpp::Rcout << c++ << ": " << (short)hdr.ftflgs << endl;;
		Rcpp::Rcout << c++ << ": " << (short)hdr.fversn << endl;
		Rcpp::Rcout << c++ << ": " << (short)hdr.fexper << endl;
		Rcpp::Rcout << c++ << ": " << hdr.fexp << endl;
		Rcpp::Rcout << c++ << ": " << (DWORD) hdr.fnpts << endl;
		Rcpp::Rcout << c++ << ": " << (double) hdr.ffirst << endl;
		Rcpp::Rcout << c++ << ": " << (double) hdr.flast << endl;
		Rcpp::Rcout << c++ << ": " << (DWORD) hdr.fnsub << endl;
		Rcpp::Rcout << c++ << ": " << (short) hdr.fxtype << endl;
		Rcpp::Rcout << c++ << ": " << (short) hdr.fytype << endl;
		Rcpp::Rcout << c++ << ": " << (short) hdr.fztype << endl;
		Rcpp::Rcout << c++ << ": " << (short) hdr.fpost << endl;
		Rcpp::Rcout << c++ << ": " << (DWORD) hdr.fdate << endl;

		Rcpp::Rcout << c++ << ": ";
		for(int i = 0; i < 8; ++i){
			Rcpp::Rcout << hdr.fres[i] << " ";
		}
		Rcpp::Rcout << endl;
		Rcpp::Rcout << c++ << ": ";
		for(int i =0; i < 8; ++i){
			Rcpp::Rcout << hdr.fsource[i] << " ";
		}
		Rcpp::Rcout << endl;

		Rcpp::Rcout << c++ << ": " << (short) hdr.fpeakpt << endl;

		Rcpp::Rcout << c++ << ": ";
		for(int i =0; i < 8; ++i){
			Rcpp::Rcout << hdr.fspare[i] << " ";
		}
		Rcpp::Rcout << endl;
		Rcpp::Rcout << c++ << ": ";
		for(int i =0; i < 129; ++i){
			Rcpp::Rcout << hdr.fcmnt[i] << " ";
		}
		Rcpp::Rcout << endl;
		Rcpp::Rcout << c++ << ": ";
		for(int i =0; i < 29; ++i){
			Rcpp::Rcout << hdr.fcatxt[i] << " ";
		}
		Rcpp::Rcout << endl;

		Rcpp::Rcout << c++ << ": " << (DWORD) hdr.flogoff << endl;
		Rcpp::Rcout << c++ << ": " << (DWORD) hdr.fmods << endl;
		Rcpp::Rcout << c++ << ": " << (short) hdr.fprocs << endl;
		Rcpp::Rcout << c++ << ": " << (short) hdr.flevel << endl;
		Rcpp::Rcout << c++ << ": " << (short) hdr.fsampin << endl;
		Rcpp::Rcout << c++ << ": " << (float) hdr.ffactor << endl;

		Rcpp::Rcout << c++ << ": ";
		for(int i =0; i < 48; ++i){
			Rcpp::Rcout << hdr.fmethod[i] << " ";
		}
		Rcpp::Rcout << endl;

		Rcpp::Rcout << c++ << ": " << (float) hdr.fzinc << endl;
		Rcpp::Rcout << c++ << ": " << (DWORD) hdr.fwplanes << endl;
		Rcpp::Rcout << c++ << ": " << (float) hdr.fwinc << endl;
		Rcpp::Rcout << c++ << ": " << (short) hdr.fwtype << endl;
}

const string SPC_reader::FXTYPE[31] = {
			"`/` (x, \"a. u.\")",
			"`/` (tilde (nu), cm^-1)",
			"`/` (lambda, (mu * m))",
			"`/` (lambda, nm)",
			"`/` (t, s)",
			"`/` (t, min)",
			"`/` (f, Hz))",
			"`/` (f, kHz))",
			"`/` (f, MHz))",
			"`/` (frac (m, z), frac (u, e))",
			"`/` (delta, ppm)",
			" /` (t, d)",
			"`/` (t, a)",
			"`/` (Delta*tilde (nu), cm^-1)",
			"`/` (E, eV)",
			"NA",
			"Diode No",
			"Channel",
			"`/` (x, degree)",
			"`/` (T, degree*F)",
			"`/` (T, degree*C)",
			"`/` (T, K)",
			"Data Point",
			"`/` (t, ms)",
			"`/` (t, micro*s)",
			"`/` (t, ns)",
			"`/` (f, GHz)",
			"`/` (lambda, cm)",
			"`/` (lambda, m)",
			"`/` (lambda, mm)",
			"`/` (t, h)"
};

const string SPC_reader::FYTYPE[32] = {
		"`/` (I[Ref], \"a. u.\")",
		"`/` (I, \"a. u.\")",
		"`/` (I[IGRM], \"a. u.\")",
		"A",
		"frac ((1 - R)^2, 2 * R)",
		"Counts",
		"`/` (U, V)",
		"`/` (y, degree)",
		"`/` (I, mA)",
		"`/` (l, mm)",
		"`/` (U, mV)",
		"-log (R)",
		"`/` (y, '%')",
		"`/` (I, 'a. u.')",
		"I / I[0]",
		"`/` (E, J)",
		"NA",
		"`/` (G, dB)",
		"NA",
		"NA",
		"`/` (T, degree*F)",
		"`/` (T, degree*C)",
		"`/` (T, K)",
		"n",
		"K",
		"Re (y)",
		"Im (y)",
		"y (complex)",
		"T",
		"R",
		"`/` (I, 'a. u.')",
		"`/` (I[Emission], 'a. u.'"
};
