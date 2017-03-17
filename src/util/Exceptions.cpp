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
	//loc.begin.filename = new string(*l.begin.filename);
	//loc.end.filename = new string(*l.end.filename);
	strcpy(msg,str.c_str());
}
/*SemanticError::~SemanticError(){
	cout << "borrando sem error" << endl;
	delete loc.begin.filename;
	delete loc.end.filename;
	loc.begin.filename = nullptr;
	loc.end.filename = nullptr;
}*/

const char* SemanticError::what() const _GLIBCXX_USE_NOEXCEPT {
	static char c[150];//TODO??
	//sprintf(c,"%s",(loc.begin.filename == YY_NULLPTR) ? "" : loc.begin.filename->c_str());
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
	//loc.begin.filename = new string(*l.begin.filename);
	//loc.end.filename = new string(*l.end.filename);
}

