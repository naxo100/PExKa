/*
 * KappaDriver.cpp
 *
 *  Created on: Jan 8, 2016
 *      Author: naxo
 */

#include "KappaDriver.h"
#include <fstream>
namespace grammar {

using namespace std;
KappaDriver::KappaDriver():
		parser(*this), lexer (*this){}

KappaDriver::KappaDriver(const vector<string> &fls):
		files(fls),parser(*this), lexer(*this) {}

KappaDriver::~KappaDriver() {}

int KappaDriver::parse(){
	int r;
	if(files.empty()){
		// = stdin;
		r = parser.parse();
	}
	else{
		for(fit = files.begin();fit != files.end() ; fit++){
			ifstream f(fit->c_str());
			if (f.fail()) {
		      cerr << "Cannot open file " << fit->c_str() << endl;
		      exit (EXIT_FAILURE);
		    }
			lexer.switch_streams( &f, NULL);
			loc.initialize(&*fit,1,1);
		    r = parser.parse();
		    cout << "file parsed!!" << endl;
		    f.close();
		}
	}
	return r;
}

yy::KappaParser::symbol_type KappaDriver::getNextToken(){
	return lexer.get_next_token();
}

std::string* KappaDriver::getCurrentFileName() const{
	return &(*fit);
}

yy::location* KappaDriver::getLocation(){
	return &loc;
}

ast::KappaAst& KappaDriver::getAst(){
	return syntax_tree;
}


} /* namespace grammar */


