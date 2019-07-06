/*
 * Environment.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: naxo
 */

#include "Environment.h"
#include "../grammar/ast/Basics.h"
#include "mixture/Agent.h"
#include "mixture/Component.h"
#include "../util/Warning.h"

using namespace std;

namespace pattern {

Environment::Environment() {
//TODO
	//useExpressions.emplace_back(0);
	//useExpressions[0].evaluateCells();
}

Environment::~Environment() {
	for(auto pert : perts)
		delete pert;
}


bool Environment::exists(const string &name,const Environment::IdMap &map){
	return map.count(name);
}


unsigned Environment::declareToken(const ast::Id &name_loc){
	const string& name = name_loc.getString();
	if(tokenMap.count(name) || signatureMap.count(name) )
		throw SemanticError("Name "+name+" already defined for agent or token.",name_loc.loc);
	auto id = tokenNames.size();
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

Mixture& Environment::declareMixture(const Mixture& new_mix) {
	for(auto &mix : mixtures)
		if( mix == new_mix ){
			return mix;
		}
	mixtures.emplace_back(new_mix);
	mixtures.back().setId(mixtures.size()-1);
	return mixtures.back();
}

Mixture::Component& Environment::declareComponent(const Mixture::Component& new_comp){
	for(auto &comp : components ){
		if( comp == new_comp){
			return comp;
		}
	}
	components.emplace_back(new_comp);
	auto& cc = components.back();
	cc.setId(components.size()-1);
	small_id i = 0;
	for(auto ag : cc)
		ag->addCc(&cc,i++);
	return cc;
}

/*Mixture::Agent& Environment::declareAgentPattern(const Mixture::Agent* new_ag,bool is_lhs){
	list<pair<set<small_id>,Mixture::Agent*> > greater;
	list<pair<set<small_id>,Mixture::Agent*> > less;
	//cout << "Declaring agent pattern " << new_ag->toString(*this) << endl;
	for(auto &ag : agentPatterns[new_ag->getId()] ){
		try {
			set<small_id> sites;
			switch(new_ag->compare(ag,sites)){//1 if new_ag < ag
			case 0:
				//cout << "Same agent pattern was declared before: " << ag.toString(*this) << endl;
				delete new_ag;
				return ag;
			case 1:
				//cout << "Found an agent pattern more specific: " << ag.toString(*this) << endl;
				less.emplace_back(sites,&ag);
				break;
			case -1:
				//cout << "Found an agent pattern more general: " << ag.toString(*this) << endl;
				greater.emplace_back(sites,&ag);
				break;
			default:
				//cout << "Found an agent pattern more general and specific: " << ag.toString(*this) << endl;
				less.emplace_back(sites,&ag);
				greater.emplace_back(sites,&ag);
				break;
			}
		}
		catch(False& ex){
			//cout << "not compatible patterns: " << ag.toString(*this) << endl;//different patterns
		}
	}
	agentPatterns[new_ag->getId()].emplace_back(*new_ag);
	auto& new_agent = agentPatterns[new_ag->getId()].back();
	for(auto& sites_ag : less)
		for(auto site : sites_ag.first){
			if (is_lhs)
				sites_ag.second->addParent(site,&new_agent);
			new_agent.addChild(site,sites_ag.second);
		}
	for(auto& sites_ag : greater)
		for(auto site : sites_ag.first){
			if (is_lhs)
				sites_ag.second->addChild(site,&new_agent);
			new_agent.addParent(site,sites_ag.second);
		}
	delete new_ag;
	return new_agent;
}*/

Mixture::Agent& Environment::declareAgentPattern(const Mixture::Agent* new_ag,bool is_lhs){
	for(auto &ag : agentPatterns[new_ag->getId()] ){
		if(new_ag->operator==(ag)){
			if(new_ag->getAuxNames() == ag.getAuxNames()){
				//cout << "Same agent pattern was declared before: " << ag.toString(*this) << endl;
				delete new_ag;
				return ag;
			}
			else{
				stringstream loc;
				loc << ag.getLoc();
				ADD_WARN("Same agent pattern declared before ("+loc.str()+
					") but with different aux names. Try to normalize name to improve performance.",new_ag->getLoc());
			}
		}
	}

	agentPatterns[new_ag->getId()].emplace_back(*new_ag);
	auto& new_agent = agentPatterns[new_ag->getId()].back();
	delete new_ag;
	return new_agent;
}

simulation::Rule& Environment::declareRule(const ast::Id &name_loc,const Mixture& mix,const yy::location& loc){
	//if(this->exists(name,algMap) || this->exists(name,kappaMap))
	const string& name = name_loc.getString();
	if(this->exists(name,varMap))
		throw SemanticError("Label "+name+" already defined.",name_loc.loc);
	small_id id;
	id = varNames.size();
	varMap[name] = id;
	varNames.push_back(name);
	rules.emplace_back(rules.size(),name_loc.getString(),mix,loc);
	return rules.back();
}

void Environment::declarePert(simulation::Perturbation* pert){
	pert->setId(perts.size());
	perts.push_back(pert);
}

void Environment::declareObservable(state::Variable* var){
	observables.emplace_back(var);
}

void Environment::buildInfluenceMap(const VarVector& vars){
	for(auto& r : rules)
		r.checkInfluence(*this,vars);
}
void Environment::buildFreeSiteCC() {
	for(auto& ag_class : agentPatterns){
		for(auto ag : ag_class){
			for(auto& site_id : ag){
				if(site_id.second.link_type == Pattern::FREE){
					auto& l = freeSiteCC[(ag.getId() << sizeof(small_id)) + site_id.first];
					l.insert(l.end(),ag.getIncludes().begin(),ag.getIncludes().end());
				}
			}
		}
	}
}

const list<pair<const Mixture::Component*,small_id> >& Environment::getFreeSiteCC(small_id ag,small_id site) const{
	static list<pair<const Mixture::Component*,small_id> > l;
	try{
		return freeSiteCC.at((ag << sizeof(small_id)) + site);
	}
	catch(std::out_of_range& ex){}
	return l;
}

short Environment::getVarId(const string &s) const {
	return varMap.at(s);
}
short Environment::getVarId(const ast::Id &s) const {
	try {
		return varMap.at(s.getString());
	}catch(out_of_range &ex){
		throw SemanticError("Variable "+s.getString()+" has not been declared (yet).",s.loc);
	}
	return -1;
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
unsigned Environment::getTokenId(const string &s) const {
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
const list<Mixture::Agent>& Environment::getAgentPatterns(small_id id) const{
	return agentPatterns[id];
}
const vector<simulation::Rule>& Environment::getRules() const {
	return rules;
}
const vector<simulation::Perturbation*>& Environment::getPerts() const {
	return perts;
}
const list<state::Variable*>& Environment::getObservables() const {
	return observables;
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
size_t Environment::size<state::TokenVar>() const {
	return tokenNames.size();
}
template <>
size_t Environment::size<simulation::Rule>() const {
	return rules.size();
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
void Environment::reserve<Mixture::Agent>(size_t count) {
	agentPatterns.resize(count);
}
template <>
void Environment::reserve<Channel>(size_t count) {
	channels.reserve(count);
}
template <>
void Environment::reserve<state::TokenVar>(size_t count) {
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
template <>
void Environment::reserve<simulation::Perturbation>(size_t count) {
	perts.reserve(count);
}

const DepSet& Environment::getDependencies(const Dependency& dep) const{
	return deps.getDependencies(dep).deps;
}
const Dependencies& Environment::getDependencies() const{
	return deps;
}
void Environment::addDependency(Dependency key,Dependency::Dep dep,unsigned id){
	if(key.type == Dependency::TIME && dep == Dependency::PERT &&
			perts[id]->nextStopTime() >= 0)
		deps.addTimePertDependency(id, perts[id]->nextStopTime());
	else
		deps.addDependency(key,dep,id);
}


//DEBUG methods

std::string Environment::cellIdToString(unsigned int cell_id) const {
	return this->getCompartmentByCellId(cell_id).cellIdToString(cell_id);

}

void Environment::show() const {
	//try{
		cout << "This is the environment content" << endl;
		cout << "\t\tCompartments[" << compartments.size() << "]" << endl;
		for(unsigned int i = 0; i < compartments.size() ; i++)
			cout << (i+1) << ") " << compartments[i].toString() << endl;

		cout << "\n\t\tUseExpressions[" << useExpressions.size() << "]" << endl;
		for(auto& use_expr : useExpressions){
			cout << "list of cells: " ;
			for(auto cell_id : use_expr.getCells())
				cout << cellIdToString(cell_id) << ", ";
			cout << endl;
		}

		cout << "\n\t\tChannels[" << channels.size() << "]" << endl;
		for(unsigned int i = 0; i < channels.size() ; i++){
			cout << (i+1) << ") ";
			for(list<Channel>::const_iterator it = channels[i].cbegin();it != channels[i].cend();it++){
				cout << it->toString() << endl;
				list< list<int> > l = it->getConnections();
				it->printConnections(l);
			}
		}
		/*cout << "\n\t\tAgentPatterns[" ;
		int count = 0;
		for(auto& ag_list : agentPatterns)
			count += ag_list.size();
		cout << count << "]" << endl;
		for(auto& ap_list : agentPatterns)
			for(auto& ap : ap_list){
				for(auto& site_ptrns : ap.getParentPatterns()){
					cout << this->getSignature(ap.getId()).getSite(site_ptrns.first).getName()
							<< "~{";
					for(auto& emb : site_ptrns.second)
						cout << emb->toString(*this) << ",";
					cout << "}, ";
				}
				cout << " >>> [ " << ap.toString(*this) << " ] >>> ";
				for(auto& site_ptrns : ap.getChildPatterns()){
					cout << this->getSignature(ap.getId()).getSite(site_ptrns.first).getName()
							<< "~{";
					for(auto& emb : site_ptrns.second)
						cout << emb->toString(*this) << ",";
					cout << "}, ";
				}
				cout << endl;
			}*/
		cout << "\n\t\tComponents[" << components.size() << "]" << endl;
		int i = 0;
		for(auto& comp : components){
			cout << "[" << comp.getId() << "] -> ";
			cout << comp.toString(*this) << endl;
			//i++;
		}
		cout << "\n\t\tMixtures[" << mixtures.size() << "]" << endl;

		cout << "\n\t\tRules[" << rules.size() << "]" << endl;
		i = 0;
		for(auto& rul : rules){
			cout << (i+1) << ") ";
			cout << rul.toString(*this);
			cout << "influence-cc: (" << rul.getInfluences().size() << ")" << endl;
			for(auto& inf : rul.getInfluences()){
				cout << "\t" << inf.first->toString(*this);
				cout << "  (" << inf.second.node_id.size() << ") \n";
			}
			cout << endl;
			i++;
		}

	/*}
	catch(exception &e){
		cout << "error: " << e.what() << endl;
	}*/
	cout << "end of env.show()" << endl;

}








} /* namespace pattern */
