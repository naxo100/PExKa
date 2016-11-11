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


short Environment::declareToken(const string &name){
	if(this->exists(name,tokenMap))
		throw SemanticError("Token "+name+" already defined.");
	short id = tokenNames.size();
	tokenMap[name] = id;
	tokenNames.push_back(name);
	return id;
}

short Environment::declareVariable(const string &name,bool is_kappa){
	//if(this->exists(name,algMap) || this->exists(name,kappaMap))
	if(this->exists(name,varMap))
		throw SemanticError("Label "+name+" already defined.");
	short id;
	/*if(is_kappa){
		id = kappaNames.size();
		kappaMap[name] = id;
		kappaNames.push_back(name);
	}
	else{
		id = algNames.size();
		algMap[name] = id;
		algNames.push_back(name);
	}*/
	id = varNames.size();
	varMap[name] = id;
	varNames.push_back(name);
	return id;
}

//TODO
short Environment::getVarId(const string &s) const {
	return 0;
}//TODO
short Environment::declareChannel(const string &s) {
	return 0;
}//TODO
short Environment::getChannelId(const string &s) const {
	return 0;
}//TODO
short Environment::declareSignature(const Signature &s) {
	return 0;
}

} /* namespace pattern */
