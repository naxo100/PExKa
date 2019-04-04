/*
 * Warning.cpp
 *
 *  Created on: May 30, 2017
 *      Author: naxo
 */

#include "Warning.h"


Warning::Warning(const std::string& m,const yy::location& l) : msg(m),loc(l){}

Warning::~Warning() {}

const std::string& Warning::what() const{
	return msg;
}



WarningStack WarningStack::stack = WarningStack();


WarningStack::WarningStack(){}
WarningStack& WarningStack::getStack() {
	return stack;
}

void WarningStack::show() const {
	for(auto& warn : *this){
		std::cout << warn.what() << std::endl;
	}
	if(size() == 0)
		std::cout << "No warnings." << std::endl;
}
