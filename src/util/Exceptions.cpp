/*
 * Exceptions.cpp
 *
 *  Created on: Aug 16, 2016
 *      Author: naxo
 */


#include <sstream>
#include "Exceptions.h"

using namespace std;

SemanticError::SemanticError(const string &str,const yy::location &l)
		: msg(str), loc(l) {}

const char* SemanticError::what() const _GLIBCXX_USE_NOEXCEPT {
	stringstream r;
	r << "Semantic error: " << msg << "(" << loc << ")";
	return r.str().c_str();
}

void SemanticError::setLocation(const yy::location &l){
	loc = l;
}

