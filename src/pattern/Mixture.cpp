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

Mixture::Mixture(short agent_count) : agentCount(0),siteCount(0),compCount(0){
	agents = new const Agent*[agent_count];
}

//TODO
Mixture::~Mixture() {
	//delete[] agents;
}

short Mixture::addAgent(const Mixture::Agent *a){
	agents[agentCount] = a;
	agentCount++;
	return agentCount-1;
}

void Mixture::addLink(const id_pair &first,const id_pair &second){
	graph.emplace(first,second);
	graph.emplace(second,first);
}

void Mixture::setComponents(Environment &env){
	//Component c;
	//map<id_pair,id_pair,CompareIdPair> m(graph);
	list<pair<Component*,map<short,short> > > comps;
	for(const auto& lnk : graph){
		auto c_it = comps.begin();
		for(;c_it != comps.end(); c_it++){
			Component &comp = *get<0>(*c_it);
			//set<short> &content = get<1>(*c_it);
			map<short,short> &mask = get<1>(*c_it);//ag_mix -> ag_comp
			if(mask.find(lnk.first.first) != mask.end()){
				comp.addAgent(agents[lnk.second.first]);
				mask.insert(make_pair(lnk.second.first,comp.size()-1));
				//content.insert(lnk.second.first);
				comp.addLink(lnk,mask);
				break;
			}
			else if(mask.find(lnk.second.first) != mask.end()){
				comp.addAgent(agents[lnk.first.first]);
				mask.insert(make_pair(lnk.first.first,comp.size()-1));
				//content.insert(lnk.first.first);
				comp.addLink(lnk,mask);
				break;
			}
		}
		if(c_it != comps.end()){
			comps.emplace_back(new Component(),map<short,short>());
			comps.back().second.emplace(lnk.first.first,0);
			comps.back().first->addAgent(agents[lnk.first.first]);
			comps.back().second.emplace(lnk.second.first,1);
			comps.back().first->addAgent(agents[lnk.second.first]);
			comps.back().first->addLink(lnk,comps.back().second);
		}

	}
	for(int i = 0; i < agentCount ; i++){
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
	this->comps = new const Component*[compCount];
	int i=0;
	for(const auto& c : comps){
		auto comp = env.declareComponent(*c.first);
		delete c.first;
		this->comps[i] = &comp;
		i++;
	}
	//return this->comps;
}

string Mixture::toString() const {
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
	if(signId == a.signId){
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

Mixture::Component::Component() {};
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

void Mixture::Component::addLink(const pair<id_pair,id_pair> &lnk,const map<short,short> &mask){
	id_pair first(mask.at(lnk.first.first),lnk.first.second);
	id_pair second(mask.at(lnk.second.first),lnk.second.second);
	graph.emplace(first,second);
	graph.emplace(second,first);
}

bool Mixture::Component::operator ==(const Component &c) const{
	if(this == &c)
		return true;
	if(c.graph != graph)
		return false;
	if(size() == c.size()){
		for(size_t i = 0; i< size(); i++){
			if(!(*agents[i] == *c.agents[i]))
				return false;
		}
	}
	return true;
}

/*****************************************/


bool CompareIdPair::operator()(id_pair p1,id_pair p2){
	return p1.first < p2.first ? true : (p1.second < p2.second ? true : false );
}

} /* namespace pattern */
