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
		lexer3 (*this),parser3(*this), lexer4(*this), parser4(*this) {}

KappaDriver::KappaDriver(const vector<string> &fls):
		files(fls),lexer3 (*this), parser3(*this),lexer4(*this),parser4(*this) {}

KappaDriver::~KappaDriver() {}

int KappaDriver::parse(){
	int r;
	if(files.empty()){
		// = stdin;
		r = parser3.parse();
	} else {
		for(auto& file : files) {
			curr_file = file;
			ifstream f(file.c_str());

			if (f.fail()) {
				cerr << "Cannot open input file " << file << endl;
				exit (EXIT_FAILURE);
		    }


			string file_type = file.substr(file.find_last_of("."));
			if(file_type == "ka4"){
				lexer4.switch_streams( &f, NULL);
				loc.initialize(&file,1,1);
				r = parser4.parse();
			}
			else{
				lexer3.switch_streams( &f, NULL);
				loc.initialize(&file,1,1);
				r = parser3.parse();
			}

		    f.close();
		}
	}
	return r;
}

template<>
kappa3::Parser::symbol_type KappaDriver::getNextToken(){
	return lexer3.get_next_token();
}
template<>
kappa4::Parser::symbol_type KappaDriver::getNextToken(){
	return lexer4.get_next_token();
}

const std::string* KappaDriver::getCurrentFileName() const{
	return &curr_file;
}

yy::location* KappaDriver::getLocation(){
	return &loc;
}

ast::KappaAst& KappaDriver::getAst(){
	return syntax_tree;
}


} /* namespace grammar */


