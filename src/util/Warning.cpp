/*
 * Warning.cpp
 *
 *  Created on: May 30, 2017
 *      Author: naxo
 */

#include "Warning.h"

WarningStack::WarningStack(){}

Warning::Warning(const std::string& m,const yy::location& l) : msg(m),loc(l){}

Warning::~Warning() {}

WarningStack WarningStack::stack = WarningStack();


WarningStack& WarningStack::getStack() {
	return stack;
}

