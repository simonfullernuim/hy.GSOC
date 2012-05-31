

#include "Super_parser.h"

class TXVALS_parser : public Super_parser{
private:
protected:
public:


	TXVALS_parser( std::ifstream* ipt_ifstr, SPC* ipt_reader_hdr,  short ipt_tsprec_subval,
			Rcpp::NumericVector::iterator ipt_X_ptr, Rcpp::NumericMatrix::iterator ipt_Y_ptr, std::vector< std::string >* ipt_log2data )
			: Super_parser( ipt_ifstr, ipt_reader_hdr, ipt_tsprec_subval, ipt_X_ptr, ipt_Y_ptr,	 ipt_log2data ) {
		int Y_len = reader_hdr->fnpts;
	}
	virtual ~TXVALS_parser(){}

	void parse_file();
	virtual void readX(int amt){
		Super_parser::readX( amt );
	}

};
