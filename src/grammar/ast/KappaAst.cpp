/*
 * KappaAst.cpp
 *
 *  Created on: Jan 8, 2016
 *      Author: naxo
 */

#include "KappaAst.h"
#include <fstream>
namespace ast {

using namespace std;
KappaAst::KappaAst(): parser(*this), lexer (*this){}

KappaAst::KappaAst(char* fls[],int flsc): parser(*this), lexer(*this) {
	for(int i = 0; i < flsc; i++)
		files.push_back(fls[i]);
}

KappaAst::~KappaAst() {
	// TODO Auto-generated destructor stub
}

int KappaAst::parse(){
	int r;
	if(files.empty()){
		// = stdin;
		r = parser.parse();
	}
	else{
		for(list<string>::iterator it = files.begin();it != files.end() ; it++){
			ifstream *f = new ifstream(it->c_str());
			if (!f->failbit ) {
				lexer.switch_streams( f, NULL);
		      //error ("cannot open " + file + ": " + strerror(errno));
		      exit (EXIT_FAILURE);
		    }
		    r = parser.parse();
		    delete f;
		   // fclose(yyin);
		}
	}
	return r;
}

yy::KappaParser::symbol_type KappaAst::getNextToken(){
	return lexer.get_next_token();
}


} /* namespace ast */


