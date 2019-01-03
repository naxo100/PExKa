/*
 * Dependencies.cpp
 *
 *  Created on: Apr 21, 2016
 *      Author: naxo
 */

#include "Dependencies.h"

namespace pattern {

Dependencies::Dependencies() {
	// TODO Auto-generated constructor stub

}

Dependencies::~Dependencies() {
	// TODO Auto-generated destructor stub
}

Dependencies::Dependency::Dependency(Dep d,unsigned i) : type(d),id(i) {}
Dependencies::Dependency::Dependency(string _aux) : type(AUX),aux(_aux),id(0) {}

bool Dependencies::Dependency::operator <(const Dependency& d) const {
	return type < d.type ? true : type == d.type ? id < d.id : false;
}

void Dependencies::addDependency(Dependency key,Dep d, unsigned int id){
	deps[key].deps.emplace(d,id);
}
const Dependencies::DepSet2& Dependencies::getDependencies(Dependency d) {
	return deps[d];
}



} /* namespace pattern */
