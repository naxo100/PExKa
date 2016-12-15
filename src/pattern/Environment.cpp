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
short Environment::declareCompartment(const Compartment& comp){
	if(compartmentMap.count(comp.getName()))
		throw SemanticError("Compartment "+comp.getName()+" already defined.");
	short id = compartments.size();
	compartments.push_back(comp);
	compartmentMap[comp.getName()] = id;
	return id;
}
short Environment::declareChannel(const Channel &c) {
	short id;
	if(channelMap.count(c.getName()))
		id = channelMap[c.getName()];
	else{
		id = channels.size();
		channels.push_back(list<Channel>());
		channelMap[c.getName()] = id;
	}
	channels[id].push_back(c);
	return id;
}
short Environment::declareSignature(const Signature &s) {
	if(signatureMap.count(s.getName()))
		throw SemanticError("Signature "+s.getName()+" already defined.");
	short id = signatures.size();
	signatures.push_back(s);
	signatureMap[s.getName()] = id;
	return id;
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
