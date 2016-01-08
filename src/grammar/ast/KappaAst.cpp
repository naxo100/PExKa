/*
 * KappaAst.cpp
 *
 *  Created on: Jan 8, 2016
 *      Author: naxo
 */

#include "KappaAst.h"

namespace ast {

KappaAst::KappaAst(): parser(*this){}

KappaAst::KappaAst(char* fls[],int flsc): parser(*this){
	for(int i = 0; i < flsc; i++)
		files.push_back(fls[i]);
}

KappaAst::~KappaAst() {
	// TODO Auto-generated destructor stub
}




} /* namespace ast */
