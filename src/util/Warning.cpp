/*
 * Warning.cpp
 *
 *  Created on: May 30, 2017
 *      Author: naxo
 */

#include "Warning.h"


Warning::Warning(const std::string& m,const yy::location& l) : msg(m),loc(l){}

Warning::~Warning() {}

const std::string Warning::what() const{
	static char c[150];
	if(loc != yy::location())
		if(loc.begin.line == loc.end.line)
			sprintf(c,"In file \"%s\", line %d, characters %d-%d:\n  ",
					loc.begin.filename->c_str(),loc.begin.line,loc.begin.column,loc.end.column);
		else
			sprintf(c,"In file \"%s\", lines %d-%d:\n  ",
					loc.begin.filename->c_str(),loc.begin.line,loc.end.line);
	else
		c[0] = '\0';
	return c + msg;
}



WarningStack WarningStack::stack = WarningStack();


WarningStack::WarningStack(){}
WarningStack& WarningStack::getStack() {
	return stack;
}

int WarningStack::global_id = 1;
int WarningStack::add(int id,const std::string &msg,const yy::location &loc){

	if(id == 0)
		id = ++global_id;
	if(this->count(id) < MAX_WARNS-1)
		emplace(id,Warning(msg,loc));
	else if(this->count(id) == MAX_WARNS-1)
		emplace(id,Warning(msg+" (Shutting down this warning).",loc));
	return id;
}

void WarningStack::show() const {
	for(auto& id_warn : *this){
		std::cout << id_warn.second.what() << std::endl;
	}
	if(size() == 0)
		std::cout << "No warnings." << std::endl;
}
