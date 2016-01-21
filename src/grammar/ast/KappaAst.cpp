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
KappaAst::KappaAst():
		parser(*this), lexer (*this){}

KappaAst::KappaAst(const vector<string> &fls):
		parser(*this), lexer(*this), files(fls) {}

KappaAst::~KappaAst() {}

int KappaAst::parse(){
	int r;
	if(files.empty()){
		// = stdin;
		r = parser.parse();
	}
	else{
		for(vector<string>::iterator it = files.begin();it != files.end() ; it++){
			ifstream f(it->c_str());
			if (f.fail()) {
		      cerr << "Cannot open file " << it->c_str() << endl;
		      exit (EXIT_FAILURE);
		    }
			lexer.switch_streams( &f, NULL);
		    r = parser.parse();
		   // fclose(yyin);
		}
	}
	return r;
}

void KappaAst::addDeclaration(Declaration d){
	decls.push_back(d);
	std::cout << "declaration pushed." << endl;
}

yy::KappaParser::symbol_type KappaAst::getNextToken(){
	return lexer.get_next_token();
}


} /* namespace ast */


