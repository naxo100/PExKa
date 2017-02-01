/*
 * Exceptions.cpp
 *
 *  Created on: Aug 16, 2016
 *      Author: naxo
 */


#include <sstream>
#include <cstring>
#include "Exceptions.h"

using namespace std;

SemanticError::SemanticError(const string &str,const yy::location &l)
		: msg(), loc(l) {
	strcpy(msg,str.c_str());
}

const char* SemanticError::what() const _GLIBCXX_USE_NOEXCEPT {
	static char c[150];//TODO??
	sprintf(c,"Semantic error in file \"%s\", line %d, characters %d-%d:\n%s",
			loc.begin.filename->c_str(),loc.begin.line,loc.begin.column,loc.end.column,msg);
	//stringstream r;
	//string s (string("Semantic error in file \"") + *loc.begin.filename +
	//		"\", line " + to_string(loc.begin.line) + ", characters " +
	//		to_string(loc.begin.column) + "-" + to_string(loc.end.column) + ":\n" +
	//		msg );
	return c;
}

void SemanticError::setLocation(const yy::location &l){
	loc = l;
}

