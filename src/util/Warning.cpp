/*
 * Warning.cpp
 *
 *  Created on: May 30, 2017
 *      Author: naxo
 */

#include "Warning.h"

namespace simulation {

Warning::Warning(const std::string& m,const yy::location& l) : msg(m),loc(l){}

Warning::~Warning() {}




WarningStack& WarningStack::getStack() {
	return stack;
}

} /* namespace simulation */
