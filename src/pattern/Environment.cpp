/*
 * Environment.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: naxo
 */

#include "Environment.h"
#include "../grammar/ast/Basics.h"

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


short Environment::declareToken(const ast::Id &name_loc){
	const string& name = name_loc.getString();
	if(tokenMap.count(name))
		throw SemanticError("Token "+name+" already defined.",name_loc.loc);
	short id = tokenNames.size();
	tokenMap[name] = id;
	tokenNames.push_back(name);
	return id;
}
short Environment::declareVariable(const ast::Id &name_loc,bool is_kappa){
	//if(this->exists(name,algMap) || this->exists(name,kappaMap))
	const string& name = name_loc.getString();
	if(this->exists(name,varMap))
		throw SemanticError("Label "+name+" already defined.",name_loc.loc);
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
Compartment& Environment::declareCompartment(const ast::Id &name_loc){
	const string& name = name_loc.getString();
	if(compartmentMap.count(name))
		throw SemanticError("Compartment "+name+" already defined.");
	short id = compartments.size();
	compartments.emplace_back(name);
	compartmentMap[name] = id;
	return compartments[id];
}
Channel& Environment::declareChannel(const ast::Id &name_loc) {
	const string& name = name_loc.getString();
	short id;
	if(channelMap.count(name))
		id = channelMap[name];
	else{
		id = channels.size();
		channels.push_back(list<Channel>());
		channelMap[name] = id;
	}
	channels[id].emplace_back(name);
	return channels[id].back();
}
Signature& Environment::declareSignature(const ast::Id &name_loc) {
	const string& name = name_loc.getString();
	if(signatureMap.count(name))
		throw SemanticError("Signature "+name+" already defined.");
	short id = signatures.size();
	signatures.emplace_back(name);
	signatureMap[name] = id;
	return signatures[id];
}

//TODO
short Environment::getVarId(const string &s) const {
	return 0;
}//TODO
short Environment::getChannelId(const string &s) const {
	return 0;
}//TODO
short Environment::getCompartmentId(const string &s) const {
	return compartmentMap.at(s);
}

const Compartment& Environment::getCompartment(short id) const{
	return compartments[id];
}







//DEBUG methods

void Environment::show() const {
	try{
	cout << "This is the environment content" << endl;
	cout << "\tCompartments[" << compartments.size() << "]" << endl;
	for(unsigned int i = 0; i < compartments.size() ; i++)
		cout << (i+1) << ") " << compartments[i].toString() << endl;

	cout << "\tChannels[" << channels.size() << "]" << endl;
	for(unsigned int i = 0; i < channels.size() ; i++){
		cout << (i+1) << ") ";
		for(list<Channel>::const_iterator it = channels[i].cbegin();it != channels[i].cend();it++){
			cout << it->toString() << endl;
			list< list<int> > l = it->getConnections();
			it->printConnections(l);
		}
	}
	}
	catch(exception &e){
		cout << "error: " << e.what() << endl;
	}
	cout << "end of env.show()" << endl;

}








} /* namespace pattern */
