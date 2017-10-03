/*
 * Injection.cpp
 *
 *  Created on: Apr 11, 2017
 *      Author: naxo
 */

#include "Injection.h"

namespace matching {


Injection::~Injection() {
	// TODO Auto-generated destructor stub
}

bool Injection::operator<(const Injection& inj) const {
	return this->getEmbedding() < inj.getEmbedding();
}



} /* namespace simulation */
