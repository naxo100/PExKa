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
	static char c[250] ;//TODO??

	if(loc.begin.filename == nullptr)
		sprintf(c,"Semantic error in file (no-location):\n%s",msg);
	else
		sprintf(c,"Semantic error in file \"%s\", line %d, characters %d-%d:\n%s",
	        loc.begin.filename->c_str(),loc.begin.line,loc.begin.column,loc.end.column,msg);
	return c;
}

void SemanticError::setLocation(const yy::location &l){
	loc = l;
	//loc.begin.filename = new string(*l.begin.filename);
	//loc.end.filename = new string(*l.end.filename);
}

/** /brief Syntax error Exception
 *
 */
SyntaxError::SyntaxError(const string &str, const yy::location &l)
		: msg(), loc(l) {
	strcpy(msg,str.c_str());
}

const char* SyntaxError::what() const _GLIBCXX_USE_NOEXCEPT {
	static char c[250] ;//TODO??

	sprintf(c,"Syntax error in file \"%s\", line %d, characters %d-%d:\n%s",
	        loc.begin.filename->c_str(),loc.begin.line,loc.begin.column,loc.end.column,msg);

	return c;
}

void SyntaxError::setLocation(const yy::location &l){
	loc = l;
}



NullEvent::NullEvent(int e) : error(e) {}

const char* NullEvent::what() const _GLIBCXX_USE_NOEXCEPT {
	switch(error){
	case 0:return "Not a null event";
	case 1:return "unary rule with binary instance";
	case 2:return "binary rule with unary instance";
	case 3:return "clashing instance";
	case 4:return "overapproximation clash";
	case 5:return "invalid injection clash";
	case 6:return "perturbation interrupting time";
	default:return "invalid arg";
	}
	return "invalid!!!!";
}

