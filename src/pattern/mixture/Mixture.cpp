/*
 * Mixture.cpp
 *
 *  Created on: Apr 26, 2016
 *      Author: naxo
 */

#include <cstring>
#include "Mixture.h"
#include "../Environment.h"
//#include "../util/Warning.h"
#include "Agent.h"
#include "Component.h"


namespace pattern {


const set<small_id>& Pattern::includedIn() const {
	return includes;
}

void Pattern::addInclude(small_id id) {
	includes.emplace(id);
}

/***************** class Mixture ************************/

Mixture::Mixture(short agent_count) : declaredComps(false),agentCount(0),
		siteCount(0),compCount(0),id(-1){
	comps = nullptr;
	agents = new Agent*[agent_count];
}

Mixture::Mixture(const Mixture& m) : declaredComps(m.declaredComps),auxiliars(m.auxiliars),
		agentCount(m.agentCount),siteCount(m.siteCount),compCount(m.compCount),id(-1){
	if(m.comps){
		agents = nullptr;
		comps = new vector<Component*>(m.compCount);
		for(size_t i = 0; i < compCount; i++)
			(*comps)[i] = (*m.comps)[i];
	}
	else {
		comps = nullptr;
		agents = new Agent*[m.compCount];
		for(size_t i = 0; i < agentCount; i++)
			agents[i] = m.agents[i];
	}
}

//TODO
Mixture::~Mixture() {
	links.clear();
	if(!declaredComps)
		for(auto comp : *comps){
			//for(auto ag : *comp)
			//	delete ag;
			delete comp;
		}
	if(comps)
		delete comps;
	if(agents)
		delete[] agents;
}

void Mixture::setId(short_id i){
	id = i;
}
short_id Mixture::getId() const {
	return id;
}
void Mixture::addAgent(Mixture::Agent *a){
	if(comps)
		throw std::invalid_argument("Cannot call addAgent() on an initialized Mixture");
	agents[agentCount] = a;
	agentCount++;
	//return agentCount-1;
}

void Mixture::addLink(const ag_st_id &p1,const ag_st_id &p2,const Environment& env){
	if(comps)
		throw std::invalid_argument("Cannot call addLink() on an initialized Mixture");
	if(p1.first < p2.first)
		links.emplace(p1,p2);
	else
		links.emplace(p2,p1);
	agents[p1.first]->setLinkPtrn(p1.second,
			agents[p2.first]->getId(),p2.second);
	agents[p2.first]->setLinkPtrn(p2.second,
			agents[p1.first]->getId(),p1.second);

}

void Mixture::declareAgents(Environment &env, bool is_lhs){
	if(comps)
		throw std::invalid_argument("Cannot call declareAgents() on an initialized Mixture");
	for(size_t i = 0; i < agentCount ; i++){
		map<string,small_id> new_aux;
		agents[i] = &env.declareAgentPattern(agents[i],new_aux,is_lhs);
		//for(auto& aux : new_aux)
		//	this->setAux(aux.first, i, aux.second);
	}
}

vector<ag_st_id> Mixture::setComponents(){
	vector<ag_st_id> order_mask(agentCount);//ag_id(mix)-> (comp_id,ag_id(comp))
	if(comps)
		throw std::invalid_argument("Cannot call setComponents() on a initialized Mixture");
	list<pair<Component*,map<short,short> > > comps;
	//iterate link map ordered by agent_id, one copy per non directed link
	for(auto l_it = links.cbegin();l_it != links.cend(); l_it++){
		auto p1 = l_it->first,p2 = l_it->second;
		auto c_it = comps.begin();
		//iterate on local components, test if ag(p1) or ag(p2) \in comp
		unsigned c_id = 0;
		for(;c_it != comps.end(); c_it++,c_id++){
			Component &comp = *(c_it->first);
			map<short,short> &mask = c_it->second;//ag_mix -> ag_comp
			if(mask.find(p1.first) != mask.end()){
				if(mask.find(p2.first) == mask.end()){
					comp.addAgent(agents[p2.first]);
					order_mask[p2.first] = make_pair(c_id,comp.size()-1);
					mask.insert(make_pair(p2.first,comp.size()-1));
				}
				comp.addLink(make_pair(p1,p2),mask);
				break;
			}
			else if(mask.find(p2.first) != mask.end()){
				comp.addAgent(agents[p1.first]);
				order_mask[p1.first] = make_pair(c_id,comp.size()-1);
				mask.insert(make_pair(p1.first,comp.size()-1));
				comp.addLink(make_pair(p1,p2),mask);
				break;
			}
		}
		//add a new component if ag(p1) and ag(p2) are not in any defined component
		if(c_it == comps.end()){
			comps.emplace_back(new Component(),map<short,short>());
			comps.back().second.emplace(p1.first,0);
			comps.back().first->addAgent(agents[p1.first]);
			order_mask[p1.first] = make_pair(c_id,0);
			comps.back().second.emplace(p2.first,1);
			comps.back().first->addAgent(agents[p2.first]);
			order_mask[p2.first] = make_pair(c_id,0);
			comps.back().first->addLink(make_pair(p1,p2),comps.back().second);
		}

	}
	//add not connected agents as not connected components
	for(size_t i = 0; i < agentCount ; i++){
		auto c_it = comps.begin();
		for(;c_it != comps.end(); c_it++){
			if(c_it->second.find(i) != c_it->second.end())
				break;
		}
		if(c_it == comps.end()){
			comps.emplace_back(new Component(),map<short,short>());
			comps.back().first->addAgent(agents[i]);
			order_mask[i] = make_pair(comps.size()-1,0);
		}
	}
	compCount = comps.size();
	delete[] agents;
	agents = nullptr;
	this->comps = new vector<Component*>(compCount);
	//declaredComps = true;not declared
	int i=0;
	//set comps.graph and declare comps in env
	for(auto& c : comps){
		auto mask = c.first->setGraph();
		for(auto mix_mask : c.second)
			order_mask[mix_mask.first].second = mask[mix_mask.second];
		(*this->comps)[i] = c.first;
		i++;
	}
	//sort(this->comps->begin(),this->comps->end());
	links.clear();
	return order_mask;
}
vector<ag_st_id> Mixture::setAndDeclareComponents(Environment &env){
	vector<ag_st_id> order_mask(agentCount);//ag_id(mix)-> (comp_id,ag_id(comp))
	if(comps)
		throw std::invalid_argument("Cannot call setComponents() on a initialized Mixture");
	list<pair<Component*,map<short,short> > > comps;
	//iterate link map ordered by agent_id, one copy per non directed link
	for(auto l_it = links.cbegin();l_it != links.cend(); l_it++){
		auto p1 = l_it->first,p2 = l_it->second;
		auto c_it = comps.begin();
		//iterate on local components, test if ag(p1) or ag(p2) \in comp
		unsigned c_id = 0;
		for(;c_it != comps.end(); c_it++,c_id++){
			Component &comp = *(c_it->first);
			map<short,short> &mask = c_it->second;//ag_mix -> ag_comp
			if(mask.find(p1.first) != mask.end()){
				if(mask.find(p2.first) == mask.end()){
					comp.addAgent(agents[p2.first]);
					order_mask[p2.first] = make_pair(c_id,comp.size()-1);
					mask.insert(make_pair(p2.first,comp.size()-1));
				}
				comp.addLink(make_pair(p1,p2),mask);
				break;
			}
			else if(mask.find(p2.first) != mask.end()){
				comp.addAgent(agents[p1.first]);
				order_mask[p1.first] = make_pair(c_id,comp.size()-1);
				mask.insert(make_pair(p1.first,comp.size()-1));
				comp.addLink(make_pair(p1,p2),mask);
				break;
			}
		}
		//add a new component if ag(p1) and ag(p2) are not in any defined component
		if(c_it == comps.end()){
			comps.emplace_back(new Component(),map<short,short>());
			comps.back().second.emplace(p1.first,0);
			comps.back().first->addAgent(agents[p1.first]);
			order_mask[p1.first] = make_pair(c_id,0);
			comps.back().second.emplace(p2.first,1);
			comps.back().first->addAgent(agents[p2.first]);
			order_mask[p2.first] = make_pair(c_id,1);
			comps.back().first->addLink(make_pair(p1,p2),comps.back().second);
		}

	}
	//add not connected agents as not connected components
	for(size_t i = 0; i < agentCount ; i++){
		auto c_it = comps.begin();
		for(;c_it != comps.end(); c_it++){
			if(c_it->second.find(i) != c_it->second.end())
				break;
		}
		if(c_it == comps.end()){
			comps.emplace_back(new Component(),map<short,short>());
			comps.back().first->addAgent(agents[i]);
			order_mask[i] = make_pair(comps.size()-1,0);
		}
	}
	compCount = comps.size();
	delete[] agents;
	agents = nullptr;
	this->comps = new vector<Component*>(compCount);
	declaredComps = true;
	vector<pair<Component*,int>> comps_order(compCount);
	int i=0;
	//set comps.graph and declare comps in env
	for(auto& c : comps){
		auto mask = c.first->setGraph();
		for(auto mix_mask : c.second)
			order_mask[mix_mask.first].second = mask[mix_mask.second];
		auto &comp = env.declareComponent(*c.first);
		delete c.first;
		comps_order[i] = make_pair(&comp,i);
		i++;
	}
	sort(comps_order.begin(),comps_order.end());
	for(unsigned i = 0; i < compCount; i++)
		(*this->comps)[i] = comps_order[i].first;
	for(unsigned i = 0; i < order_mask.size(); i++)
		order_mask[i].first = comps_order[order_mask[i].first].second;
	links.clear();
	for(auto& elem : auxiliars){
		auto cc_ag = order_mask[get<1>(elem.second)];
		get<0>(elem.second) = cc_ag.first;
		get<1>(elem.second) = cc_ag.second;
	}
	return order_mask;
}

bool Mixture::operator==(const Mixture& m) const{
	if(this == &m)
		return true;
	if(agentCount != m.agentCount || compCount != m.compCount ||
			siteCount!= m.siteCount )
		return false;
	if(comps){
		for(size_t i = 0; i < compCount; i++)
			if(!((*comps)[i] == (*m.comps)[i]))
				return false;
	} else
		for(size_t i = 0; i < compCount; i++)
			if(!(agents[i] == m.agents[i]))
				return false;
	return true;
}

size_t Mixture::size() const {
	return agentCount;
}

size_t Mixture::compsCount() const {
	return compCount;
}

const Mixture::Agent& Mixture::getAgent(small_id cc,small_id ag) const{
	return (*comps)[cc]->getAgent(ag);
}
const Mixture::Agent& Mixture::getAgent(ag_st_id cc_ag) const{
	return (*comps)[cc_ag.first]->getAgent(cc_ag.second);
}
const Mixture::Agent& Mixture::getAgent(small_id ag_id) const {
	for(size_t i = 0; i < comps->size(); i++){
		if(ag_id < (*comps)[i]->size())
			return getAgent(i,ag_id);
		ag_id -= (*comps)[i]->size();
	}
	throw std::out_of_range("Agent id out of bounds for this Mixture.");
}
const Mixture::Component& Mixture::getComponent(small_id id) const {
	return *(*comps)[id];
}
Mixture::Component& Mixture::getComponent(small_id id) {
	return *(*comps)[id];
}

const map<string,tuple<small_id,small_id,small_id>>& Mixture::getAux() const{
	return auxiliars;
}

void Mixture::addInclude(small_id r_id) {
	if(!comps)
		throw std::invalid_argument("Cannot call addInclude() on a uninitialized Mixture");
	for(auto cc : *comps)
		cc->addInclude(r_id);
	Pattern::addInclude(r_id);
}

ag_st_id Mixture::follow(small_id cc_id,small_id ag_id,small_id site) const{
	return (*comps)[cc_id]->follow(ag_id,site);
}

const vector<Mixture::Component*>::iterator Mixture::begin() const {
	return comps->begin();
}
const vector<Mixture::Component*>::iterator Mixture::end() const {
	return comps->end();
}

void Mixture::setAux(string id,small_id ag,small_id site){
	if(declaredComps)
		throw std::invalid_argument("Cannot call setAux() on an initialized mixture.");
	if(auxiliars.count(id))
		throw SemanticError("The auxiliar "+id+" is already used in this mixture.",yy::location());
	auxiliars[id] = make_tuple(small_id(-1),ag,site);
}

void Mixture::setAuxCoords(const std::map<std::string,std::tuple<int,small_id,small_id>>& aux_coords){
	if(!declaredComps)
		throw std::invalid_argument("Cannot call setAuxCoords() on an no initialized mixture.");
	for(int cc = 0; cc < comps->size(); cc++)
		for(int ag = 0; ag < comps->at(cc)->size(); ag++)
			comps->at(cc)->agents[ag]->setAuxCoords(aux_coords);
}

string Mixture::toString(const Environment& env) const {
	string out = "Agents: " + to_string(agentCount) + "\tComponents: " + to_string(compCount) + "\n";
	short i = 0;

	for( auto c : *comps ) {
		out += "[" + to_string(i) + "] -> " + c->toString(env) + ", ";
		i++;
	}

	return out;
}

string Mixture::toString(const Environment& env,const vector<ag_st_id>& mask) const {
	string out("");
	for(size_t i = 0;i < mask.size(); i++)
		out += getAgent(mask[i]).toString(env)+",";
	out.pop_back();
	return out;
}






/*****************************************/

template <typename T1,typename T2>
bool ComparePair<T1,T2>::operator()(pair<T1,T2> p1,pair<T1,T2> p2){
	return p1.first < p2.first ? true : (p1.second < p2.second ? true : false );
}


ostream& operator<<(ostream& os,const ag_st_id &ag_st){
	return os << "(" << ag_st.first << "," << ag_st.second << ")";
}

} /* namespace pattern */
