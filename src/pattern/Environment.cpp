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
//TODO
	useExpressions.emplace_back(0);
	useExpressions[0].evaluateCells();
}

Environment::~Environment() {
	// TODO Auto-generated destructor stub
}


bool Environment::exists(const string &name,const Environment::IdMap &map){
	return map.count(name);
}


short Environment::declareToken(const ast::Id &name_loc){
	const string& name = name_loc.getString();
	if(tokenMap.count(name) || signatureMap.count(name) )
		throw SemanticError("Name "+name+" already defined for agent or token.",name_loc.loc);
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
	id = varNames.size();
	varMap[name] = id;
	varNames.push_back(name);
	return id;
}
Compartment& Environment::declareCompartment(const ast::Id &name_loc){
	const string& name = name_loc.getString();
	if(compartmentMap.count(name))
		throw SemanticError("Compartment "+name+" already defined.",name_loc.loc);
	short id = compartments.size();
	compartments.emplace_back(name);
	compartmentMap[name] = id;
	return compartments[id];
}
UseExpression& Environment::declareUseExpression(unsigned short id,size_t n){
	if(useExpressions.size() != id)
		throw std::invalid_argument("UseExpression id doesn't match.");
	useExpressions.emplace_back(n);
	return useExpressions[id];
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
	if(tokenMap.count(name) || signatureMap.count(name) )
		throw SemanticError("Name "+name+" already defined for agent or token.",name_loc.loc);
	short id = signatures.size();
	signatures.emplace_back(name);
	signatureMap[name] = id;
	signatures[id].setId(id);
	return signatures[id];
}

const Mixture& Environment::declareMixture(const Mixture& new_mix) {
	for(const auto &mix : mixtures)
		if( mix == new_mix ){
			return mix;
		}
	mixtures.emplace_back(new_mix);
	return mixtures.back();
}

const Mixture::Component& Environment::declareComponent(const Mixture::Component& new_comp){
	for(const auto &comp : components ){
		if( comp == new_comp){
			return comp;
		}
	}
	components.emplace_back(new_comp);
	//Mixture::Component& co =
	return components.back();
}

const Mixture::Agent& Environment::declareAgentPattern(const Mixture::Agent* new_ag){
	for(auto &ag : agentPatterns ){
		if( ag == *new_ag){
			delete new_ag;
			return ag;
		}
	}
	agentPatterns.emplace_back(*new_ag);
	delete new_ag;
	return agentPatterns.back();
}

simulation::Rule& Environment::declareRule(const ast::Id &name_loc,const Mixture& mix){
	//if(this->exists(name,algMap) || this->exists(name,kappaMap))
	const string& name = name_loc.getString();
	if(this->exists(name,varMap))
		throw SemanticError("Label "+name+" already defined.",name_loc.loc);
	small_id id;
	id = varNames.size();
	varMap[name] = id;
	varNames.push_back(name);
	rules.emplace_back(name_loc,mix);
	return rules.back();
}

short Environment::getVarId(const string &s) const {
	return varMap.at(s);
}
short Environment::getChannelId(const string &s) const {
	return channelMap.at(s);
}
short Environment::getCompartmentId(const string &s) const {
	return compartmentMap.at(s);
}
short Environment::getSignatureId(const string &s) const {
	return signatureMap.at(s);
}
short Environment::getTokenId(const string &s) const {
	return tokenMap.at(s);
}


const Compartment& Environment::getCompartment(short id) const{
	return compartments[id];
}
const list<Channel>& Environment::getChannels(short id) const{
	return channels[id];
}
const UseExpression& Environment::getUseExpression(short id) const {
	return useExpressions[id];
}
const Signature& Environment::getSignature(short id) const {
	return signatures[id];
}
const vector<pattern::Signature>& Environment::getSignatures() const{
	return signatures;
}
const list<Mixture::Component>& Environment::getComponents() const{
	return components;
}


const Compartment& Environment::getCompartmentByCellId(unsigned cell_id) const{
	for(auto &comp : compartments){
		if(cell_id < (unsigned)comp.getLastCellId())
			return comp;
	}
	throw std::out_of_range("Compartment of cell-id "+to_string(cell_id)+" not found.");
}


template <>
size_t Environment::size<Mixture>() const {
	return mixtures.size();
}
template <>
size_t Environment::size<Mixture::Component>() const {
	return components.size();
}
template <>
size_t Environment::size<Channel>() const {
	return channels.size();
}
template <>
size_t Environment::size<Token>() const {
	return tokenNames.size();
}


template <>
void Environment::reserve<Compartment>(size_t count) {
	compartments.reserve(count);
}
template <>
void Environment::reserve<Signature>(size_t count) {
	signatures.reserve(count);
}
template <>
void Environment::reserve<Channel>(size_t count) {
	channels.reserve(count);
}
template <>
void Environment::reserve<Token>(size_t count) {
	tokenNames.reserve(count);
}
template <>
void Environment::reserve<UseExpression>(size_t count) {
	useExpressions.reserve(count);
}
template <>
void Environment::reserve<simulation::Rule>(size_t count) {
	rules.reserve(count);
}


//DEBUG methods

std::string Environment::cellIdToString(unsigned int cell_id) const {
	return this->getCompartmentByCellId(cell_id).cellIdToString(cell_id);

}

void Environment::show() const {
	try{
		cout << "This is the environment content" << endl;
		cout << "\tCompartments[" << compartments.size() << "]" << endl;
		for(unsigned int i = 0; i < compartments.size() ; i++)
			cout << (i+1) << ") " << compartments[i].toString() << endl;

		cout << "\tUseExpressions[" << useExpressions.size() << "]" << endl;
		for(auto& use_expr : useExpressions){
			cout << "list of cells: " ;
			for(auto cell_id : use_expr.getCells())
				cout << cellIdToString(cell_id) << ", ";
			cout << endl;
		}

		cout << "\tChannels[" << channels.size() << "]" << endl;
		for(unsigned int i = 0; i < channels.size() ; i++){
			cout << (i+1) << ") ";
			for(list<Channel>::const_iterator it = channels[i].cbegin();it != channels[i].cend();it++){
				cout << it->toString() << endl;
				list< list<int> > l = it->getConnections();
				it->printConnections(l);
			}
		}
		cout << "\tAgentPatterns[" << agentPatterns.size() << "]" << endl;
		/*for(auto& ap : agentPatterns)
			cout << ap.toString(*this) << endl;*/
		cout << "\tComponents[" << components.size() << "]" << endl;
		cout << "\tMixtures[" << mixtures.size() << "]" << endl;
		int i = 0;
		for(auto& mix : mixtures){
			cout << (i+1) << ") ";
			cout << mix.toString(*this) << endl;
			i++;
		}

		cout << "\Rules[" << rules.size() << "]" << endl;
		i = 0;
		for(auto& rul : rules){
			cout << (i+1) << ") ";
			cout << rul.toString(*this) << endl;
			i++;
		}

	}
	catch(exception &e){
		cout << "error: " << e.what() << endl;
	}
	cout << "end of env.show()" << endl;

}








} /* namespace pattern */
