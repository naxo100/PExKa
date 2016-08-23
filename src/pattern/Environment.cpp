/*
 * Environment.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: naxo
 */

#include "Environment.h"

using namespace std;

namespace pattern {

Environment::Environment() {
	// TODO Auto-generated constructor stub

}

Environment::~Environment() {
	// TODO Auto-generated destructor stub
}


bool Environment::exists(const string &name,const Environment::IdMap &map){
	return map.count(name);
}

short Environment::declareVariable(const string &name,bool is_kappa){
	if(exists(name,algMap) || exists(name,kappaMap))
		throw SemanticError("Label "+name+" already defined.");
	short id;
	if(is_kappa){
		id = kappaNames.size();
		kappaMap[name] = id;
		kappaNames.push_back(name);
	}
	else{
		id = algNames.size();
		algMap[name] = id;
		algNames.push_back(name);
	}
	return id;
}

} /* namespace pattern */
