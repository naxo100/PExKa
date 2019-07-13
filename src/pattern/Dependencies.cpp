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

Dependency::Dependency(Dep d,unsigned i) : type(d),id(i) {}
Dependency::Dependency(string _aux) : type(AUX),aux(_aux),id(0) {}

bool Dependency::operator <(const Dependency& d) const {
	return type == d.type ?
				(id == d.id ? aux < d.aux : id < d.id) : type < d.type;
}

void Dependencies::addDependency(Dependency key,Dependency::Dep d, unsigned int id){
	deps[key].deps.emplace(d,id);
}

void Dependencies::addTimePertDependency(unsigned id,float time){
	deps[Dependency(Dependency::TIME)].ordered_deps.emplace(time,Dependency(Dependency::PERT,id));
}

const Dependencies::DepSet2& Dependencies::getDependencies(Dependency d) {
	return deps[d];
}

void Dependencies::erase(Dependency key, Dependency trigger){
	deps.erase(key);
	deps[trigger].deps.erase(key);
	//deps[trigger].ordered_deps.erase(key); TODO
}

void Dependencies::eraseTimePerts(multimap<float,Dependency>::const_iterator first,
			multimap<float,Dependency>::const_iterator last){
	deps[Dependency::TIME].ordered_deps.erase(first,last);
}


} /* namespace pattern */
