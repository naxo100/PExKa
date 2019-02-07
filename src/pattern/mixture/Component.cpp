/*
 * Component.cpp
 *
 *  Created on: Jan 21, 2019
 *      Author: naxo100
 */

#include "Component.h"
#include "Agent.h"

namespace pattern {

/************** class Component ****************/

Mixture::Component::Component() : id(-1),links(new list<ag_st_id>()){};
Mixture::Component::Component(const Component& comp) : id(-1),agents(comp.size(),nullptr) {
	for(size_t i = 0; i < agents.size() ; i++)
		agents[i] = comp.agents[i];
	graph = new map<ag_st_id,ag_st_id>(*comp.graph);
}
Mixture::Component::~Component(){
	delete graph;
}

size_t Mixture::Component::size() const{
	return agents.size();
}

void Mixture::Component::setId(short_id i) {
	id = i;
}
short_id Mixture::Component::getId() const {
	return id;
}

const Mixture::Agent& Mixture::Component::getAgent(small_id ag) const {
	return *(agents[ag]);
}

const vector<Mixture::Agent*>::const_iterator Mixture::Component::begin() const{
	agents.begin();
	return agents.begin();
}
const vector<Mixture::Agent*>::const_iterator Mixture::Component::end() const{
	return agents.end();
}


short Mixture::Component::addAgent(Mixture::Agent* a){
	agents.push_back(a);
	return agents.size()-1;
}

void Mixture::Component::addLink(const pair<ag_st_id,ag_st_id> &lnk,const map<short,short> &mask){
	ag_st_id first(mask.at(lnk.first.first),lnk.first.second);
	ag_st_id second(mask.at(lnk.second.first),lnk.second.second);
	links->emplace_back(first);
	links->emplace_back(second);
}

ag_st_id Mixture::Component::follow(small_id ag_id,small_id site) const{
	try{
		return graph->at(ag_st_id(ag_id,site));
	}
	catch(std::out_of_range& e){}
	return make_pair(ag_id,site);
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


const map<ag_st_id,ag_st_id>& Mixture::Component::getGraph() const{
	return *graph;
}

vector<small_id> Mixture::Component::setGraph() {
	list<pair<Agent*,short> > reorder;
	for(size_t i = 0; i < agents.size(); i++ )
		reorder.emplace_back(agents[i],(short)i);
	reorder.sort();
	size_t i = 0;
	vector<small_id> mask(agents.size());
	for(auto aptr_id : reorder){
		agents[i] = aptr_id.first;
		mask[aptr_id.second] = i;
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
	return mask;
}

void Mixture::Component::addRateDep(const simulation::Rule& dep,small_id cc){
	deps.emplace_back(dep,cc);
}

const list<pair<const simulation::Rule&,small_id>>& Mixture::Component::getRateDeps() const{
	return deps;
}

string Mixture::Component::toString(const Environment& env) const {
	string out, glue = ",";

	// put labels to bindings
	map<ag_st_id,short> bindLabels;
	short bindLabel = 1;
	for(auto lnk = graph->begin() ; lnk != graph->end() ; ++lnk ) {
		/* Logic structure: [ag_id,site_id],[ag_id',site_id'] that means [Agent,Site] is conected with [Agent',Site']
		 * C++ structure:   [lnk->first.first,lnk->first.second],[lnk->second.first,lnk->second.second]
		 * the graph is bidirectional because the links are symmetrical, that means exist [ag_id,site_id],[ag_id',site_id'0]
		 * and [ag_id',site_id'],[ag_id,site_id]
		 */

		if( ! bindLabels[ag_st_id(lnk->first.first, lnk->first.second)] &&
				! bindLabels[ag_st_id(lnk->second.first, lnk->second.second)] ) {
			bindLabels[ag_st_id(lnk->first.first, lnk->first.second)] = bindLabel;
			bindLabels[ag_st_id(lnk->second.first, lnk->second.second)] = bindLabel;
			bindLabel++;

		}

	}

	for( unsigned mixAgId = 0; mixAgId < agents.size(); mixAgId++ ) {
		out += agents[mixAgId]->toString(env, mixAgId, bindLabels ) + glue;
		bindLabel++;
	}

	// remove the last glue
	if( out.substr(out.size()-glue.size(), out.size()) == glue )
		out = out.substr(0, out.size()-glue.size());

	return out;
}


} /* namespace pattern */
