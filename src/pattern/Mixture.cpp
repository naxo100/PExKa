/*
 * Mixture.cpp
 *
 *  Created on: Apr 26, 2016
 *      Author: naxo
 */

#include "Mixture.h"
#include "Environment.h"


namespace pattern {


/***************** class Mixture ************************/

Mixture::Mixture(short agent_count) : declaredComps(false),agentCount(0),siteCount(0),compCount(0){
	agents = new const Agent*[agent_count];
}

Mixture::Mixture(const Mixture& m) : declaredComps(m.declaredComps),
		agentCount(m.agentCount),siteCount(m.siteCount),compCount(m.compCount){
	if(m.declaredComps){
		comps = new vector<const Component*>(m.compCount);
		for(size_t i = 0; i < compCount; i++)
			(*comps)[i] = (*m.comps)[i];
	}
	else {
		agents = new const Agent*[m.compCount];
		for(size_t i = 0; i < agentCount; i++)
			agents[i] = m.agents[i];
	}
}

//TODO
Mixture::~Mixture() {
	links.clear();
	if(declaredComps)
		delete comps;
	else
		delete[] agents;
}

void Mixture::addAgent(const Mixture::Agent *a){
	if(declaredComps)
		throw std::bad_function_call("Cannot call addAgent() on a initialized Mixture");;
	agents[agentCount] = a;
	agentCount++;
	//return agentCount-1;
}

void Mixture::addLink(const ag_st_id &p1,const ag_st_id &p2){
	if(declaredComps)
		throw std::bad_function_call("Cannot call addLink() on a initialized Mixture");;
	links.emplace_back(p1.first,p1.second);
	links.emplace_back(p2.first,p2.second);
}

void Mixture::setComponents(Environment &env){
	//Component c;
	//map<id_pair,id_pair,CompareIdPair> m(graph);
	//sort(agents->begin(),agents->end());
	if(declaredComps)
		throw std::bad_function_call("Cannot call setComponents() on a initialized Mixture");;
	list<pair<Component*,map<short,short> > > comps;
	for(auto l_it = links.cbegin();l_it != links.cend(); advance(l_it,2)){
		auto p1 = *l_it,p2 = *next(l_it,1);
		auto c_it = comps.begin();
		for(;c_it != comps.end(); c_it++){
			Component &comp = *get<0>(*c_it);
			//set<short> &content = get<1>(*c_it);
			map<short,short> &mask = get<1>(*c_it);//ag_mix -> ag_comp
			if(mask.find(p1.first) != mask.end()){
				comp.addAgent(agents[p2.first]);
				mask.insert(make_pair(p2.first,comp.size()-1));
				//content.insert(lnk.second.first);
				comp.addLink(make_pair(p1,p2),mask);
				break;
			}
			else if(mask.find(p2.first) != mask.end()){
				comp.addAgent(agents[p1.first]);
				mask.insert(make_pair(p1.first,comp.size()-1));
				//content.insert(lnk.first.first);
				comp.addLink(make_pair(p1,p2),mask);
				break;
			}
		}
		if(c_it == comps.end()){
			comps.emplace_back(new Component(),map<short,short>());
			comps.back().second.emplace(p1.first,0);
			comps.back().first->addAgent(agents[p1.first]);
			comps.back().second.emplace(p2.first,1);
			comps.back().first->addAgent(agents[p2.first]);
			comps.back().first->addLink(make_pair(p1,p2),comps.back().second);
		}

	}
	for(size_t i = 0; i < agentCount ; i++){
		auto c_it = comps.begin();
		for(;c_it != comps.end(); c_it++){
			if(c_it->second.find(i) != c_it->second.end())
				break;
		}
		if(c_it == comps.end()){
			comps.emplace_back(new Component(),map<short,short>());
			comps.back().first->addAgent(agents[i]);
		}
	}
	compCount = comps.size();
	delete[] agents;
	this->comps = new vector<const Component*>(compCount);
	int i=0;
	for(const auto& c : comps){
		c.first->setGraph();
		auto comp = env.declareComponent(*c.first);
		delete c.first;
		(*this->comps)[i] = &comp;
		i++;
	}
	sort(this->comps->begin(),this->comps->end());
	//delete?
	links.clear();
	declaredComps = true;
	//return this->comps;
}

bool Mixture::operator==(const Mixture& m) const{
	if(this == &m)
		return true;
	if(agentCount != m.agentCount || compCount != m.compCount ||
			siteCount!= m.siteCount || declaredComps != m.declaredComps)
		return false;
	if(declaredComps){
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

string Mixture::toString(const Environment& env) const {
	return "agents: " + to_string(agentCount) + "\nComponents: " + to_string(compCount);
}


/*************** Class Agent ********************/

Mixture::Agent::Agent(short sign_id) : signId(sign_id){}

Mixture::Site& Mixture::Agent::addSite(short env_site){
	//auto &intf =
	return interface[env_site] = Site();
}

const Mixture::Site& Mixture::Agent::getSite(short id) const {
	return interface.at(id);
}

short Mixture::Agent::getId() const {
	return signId;
}

bool Mixture::Agent::operator ==(const Agent &a) const {
	if(this == &a)
		return true;
	if(signId == a.signId && interface.size() == a.interface.size()){
		for(const pair<int,Site> &id_site : interface){
			try{
				if(! (id_site.second == a.getSite(id_site.first)) )
					return false;
			}
			catch(std::out_of_range &e){
				return false;
			}
		}
	}
	else
		return false;
	return true;
}

void Mixture::Agent::setSiteValue(short site_id,short lbl_id){
	interface[site_id].val_type = ValueType::LABEL;
	interface[site_id].state.id_value = lbl_id;
}

/************** class Site *********************/
bool Mixture::Site::operator ==(const Site &s) const{
	if(val_type == s.val_type){
		switch(val_type){
		case VOID:
			break;
		case LABEL:
			if(s.state.id_value != state.id_value)
				return false;
			break;
		case INT_VAL:
			if(s.state.int_value != state.int_value)
				return false;
			break;
		case FLOAT_VAL:
			if(s.state.float_value != state.float_value)
				return false;
			break;
		}
	}
	else
		return false;
	if(s.link_type == link_type){
		if(link_type == LinkType::BIND_TO && s.lnk != lnk)
			return false;
	}
	return true;
}

/************** class Component ****************/

Mixture::Component::Component() : links(new list<ag_st_id>()){};
Mixture::Component::Component(const Component& comp) : agents(comp.size(),nullptr),graph(comp.graph) {
	for(size_t i = 0; i < agents.size() ; i++)
		agents[i] = comp.agents[i];
}

size_t Mixture::Component::size() const{
	return agents.size();
}


short Mixture::Component::addAgent(const Mixture::Agent* a){
	agents.push_back(a);
	return agents.size()-1;
}

void Mixture::Component::addLink(const pair<ag_st_id,ag_st_id> &lnk,const map<short,short> &mask){
	ag_st_id first(mask.at(lnk.first.first),lnk.first.second);
	ag_st_id second(mask.at(lnk.second.first),lnk.second.second);
	links->emplace_back(first);
	links->emplace_back(second);
}

bool Mixture::Component::operator ==(const Component &c) const{
	if(this == &c)
		return true;
	if(*c.graph != *graph)
		return false;
	if(size() == c.size()){
		for(size_t i = 0; i< size(); i++){
			if(!(*agents[i] == *c.agents[i]))
				return false;
		}
	}
	else
		return false;
	return true;
}

void Mixture::Component::setGraph() {
	list<pair<const Agent*,short> > reorder;
	for(size_t i = 0; i < agents.size(); i++ )
		reorder.emplace_back(agents[i],(short)i);
	reorder.sort();
	size_t i = 0;
	map<short,short> mask;
	for(auto aptr_id : reorder){
		agents[i] = aptr_id.first;
		mask.emplace(aptr_id.second,(short)i);
		i++;
	}

	auto graph_ptr = new map<ag_st_id,ag_st_id>();
	for(auto l_it = links->begin(); l_it != links->end(); advance(l_it,2)){
		auto p1 = *l_it, p2 = *next(l_it,1);
		graph_ptr->emplace(make_pair(mask[p1.first],p1.second),
				make_pair(mask[p2.first],p2.second));
		graph_ptr->emplace(make_pair(mask[p2.first],p2.second),
				make_pair(mask[p1.first],p1.second));
	}
	delete links;
	graph = graph_ptr;
}

/*****************************************/

template <typename T1,typename T2>
bool ComparePair<T1,T2>::operator()(pair<T1,T2> p1,pair<T1,T2> p2){
	return p1.first < p2.first ? true : (p1.second < p2.second ? true : false );
}

} /* namespace pattern */
