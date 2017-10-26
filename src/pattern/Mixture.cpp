/*
 * Mixture.cpp
 *
 *  Created on: Apr 26, 2016
 *      Author: naxo
 */

#include <cstring>
#include "Mixture.h"
#include "Environment.h"


namespace pattern {


/***************** class Mixture ************************/

Mixture::Mixture(short agent_count) : declaredComps(false),agentCount(0),
		siteCount(0),compCount(0),id(-1){
	agents = new const Agent*[agent_count];
}

Mixture::Mixture(const Mixture& m) : declaredComps(m.declaredComps),
		agentCount(m.agentCount),siteCount(m.siteCount),compCount(m.compCount),id(-1){
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

void Mixture::setId(short_id i){
	id = i;
}
short_id Mixture::getId() const {
	return id;
}
void Mixture::addAgent(const Mixture::Agent *a){
	if(declaredComps)
		throw std::invalid_argument("Cannot call addAgent() on an initialized Mixture");
	agents[agentCount] = a;
	agentCount++;
	//return agentCount-1;
}

void Mixture::addLink(const ag_st_id &p1,const ag_st_id &p2){
	if(declaredComps)
		throw std::invalid_argument("Cannot call addLink() on an initialized Mixture");
	if(p1.first < p2.first)
		links.emplace(p1,p2);
	else
		links.emplace(p2,p1);
}

void Mixture::declareAgents(Environment &env){
	if(declaredComps)
		throw std::invalid_argument("Cannot call declareAgents() on a initialized Mixture");
	for(size_t i = 0; i < agentCount ; i++){
		agents[i] = &env.declareAgentPattern(agents[i]);
	}
}
vector<ag_st_id> Mixture::setComponents(){
	vector<ag_st_id> order_mask(agentCount);//ag_id(mix)-> (comp_id,ag_id(comp))
	if(declaredComps)
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
	//delete[] agents; //we need to preserve mixture order.
	this->comps = new vector<const Component*>(compCount);
	int i=0;
	//set comps.graph and declare comps in env
	for(const auto& c : comps){
		auto mask = c.first->setGraph();
		for(auto mix_mask : c.second)
			order_mask[mix_mask.first].second = mask[mix_mask.second];
		//const Component &comp = env.declareComponent(*c.first);
		//delete c.first;
		(*this->comps)[i] = c.first;
		i++;
	}
	//sort(this->comps->begin(),this->comps->end());
	links.clear();
	//declaredComps = true;
	return order_mask;
}
vector<ag_st_id> Mixture::setAndDeclareComponents(Environment &env){
	vector<ag_st_id> order_mask(agentCount);//ag_id(mix)-> (comp_id,ag_id(comp))
	if(declaredComps)
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
	this->comps = new vector<const Component*>(compCount);
	vector<pair<const Component*,int>> comps_order(compCount);
	int i=0;
	//set comps.graph and declare comps in env
	for(const auto& c : comps){
		auto mask = c.first->setGraph();
		for(auto mix_mask : c.second)
			order_mask[mix_mask.first].second = mask[mix_mask.second];
		const Component &comp = env.declareComponent(*c.first);
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
	declaredComps = true;
	return order_mask;
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

ag_st_id Mixture::follow(small_id cc_id,small_id ag_id,small_id site) const{
	return (*comps)[cc_id]->follow(ag_id,site);
}

const vector<const Mixture::Component*>::iterator Mixture::begin() const {
	return comps->begin();
}
const vector<const Mixture::Component*>::iterator Mixture::end() const {
	return comps->end();
}

string Mixture::toString(const Environment& env) const {
	string out = "";
	short i = 0;

	for( auto c : *comps ) {
		out += "Component[" + to_string(i) + "] = " + c->toString(env) + "\n";
		i++;
	}

	return out + "\nAgents: " + to_string(agentCount) + "\nComponents: " + to_string(compCount) + "\n";
}

string Mixture::toString(const Environment& env,const vector<ag_st_id>& mask) const {
	string out("");
	for(size_t i = 0;i < mask.size(); i++)
		out += getAgent(mask[i]).toString(env)+",";
	out.pop_back();
	return out;
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


const unordered_map<small_id,Mixture::Site>::const_iterator Mixture::Agent::begin() const{
	return interface.begin();
}
const unordered_map<small_id,Mixture::Site>::const_iterator Mixture::Agent::end() const{
	return interface.end();
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

void Mixture::Agent::setSiteValue(small_id site_id,small_id lbl_id){
	//interface[site_id].val_type = ValueType::LABEL;
	interface[site_id].state.set(lbl_id);
}
void Mixture::Agent::setSiteValue(small_id site_id,float val){
	//interface[site_id].val_type = ValueType::LABEL;
	interface[site_id].state.set(val);
}

const string Mixture::Agent::toString(const Environment& env, short mixAgId,
		map<ag_st_id,short> bindLabels  ) const {
	string out = "", glue = ",";

	const Signature& sign = env.getSignature(signId);
	out += sign.getName() + "(";

	// inspect interface
	for( auto it = interface.begin(); it != interface.end(); ++it ) {
		/* it.first = site ID , it.second = Mixture::Site type
		 */
		const Signature::Site& site = sign.getSite( it->first );
		const Signature::LabelSite* labelSite;

		// variables needed to LinkType::BIND_TO
		const Signature* signature2bind;
		const Signature::Site* site2bind;

		//out += "[" + to_string(mixAgId) + "," + to_string(it->first) + "]" + site.getName(); //site name
		out += site.getName(); //site name

		switch(it->second.state.t) {
			case state::BaseExpression::SMALL_ID:
				labelSite = dynamic_cast<const Signature::LabelSite*>(& site);
				if(labelSite && it->second.state.smallVal != small_id(-1))//is not an empty site
					out += "~" + labelSite->getLabel(it->second.state.smallVal); //value of site
				break;
			case state::BaseExpression::INT :
				break;
			case state::BaseExpression::FLOAT :
				//TODO if NAN EmptySite;deprecated
				break;
			default:
				throw std::invalid_argument("Mixture::Agent::toString(): not a valid state type.");
		}

		switch(it->second.link_type) {
			case LinkType::BIND :
				if ( bindLabels.size() > 0 && bindLabels[ag_st_id(mixAgId, it->first)] )
					out += "!" + to_string( bindLabels[ag_st_id(mixAgId, it->first)] );
				else
					out += "!_";
				break;
			case LinkType::BIND_TO :
				signature2bind = & env.getSignature( it->second.lnk_ptrn.first );
				site2bind = & signature2bind->getSite( it->second.lnk_ptrn.second );
				out += "!"+ site2bind->getName() + "." + signature2bind->getName();
				break;
			case LinkType::FREE :
				out += ""; // no noted
				break;
			case LinkType::PATH :
				out += "PATH";
				break;
			case LinkType::WILD :
				out += "?";
				break;
		}

		out += glue;
	}

	// remove last glue
	if( out.substr(out.size()-glue.size(), out.size()) == glue )
		out = out.substr(0, out.size()-glue.size());

	out += ")";

	return out;
}

/************** class Site *********************/
Mixture::Site::Site() : state((small_id)0),link_type() {}

bool Mixture::Site::isEmptySite() const{
	static auto empty = (small_id)-1;
	return state.smallVal == empty;
}
bool Mixture::Site::operator ==(const Site &s) const{
	/*if(val_type == s.val_type){
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
	}*/
	//if(state != s.state)
	if(memcmp(&state,&s.state,sizeof(state::SomeValue))) //only valid if union in state is cleaned at constructor
		return false;
	if(s.link_type == link_type){
		if(link_type == LinkType::BIND_TO && s.lnk_ptrn != lnk_ptrn)
			return false;
	}
	else return false;
	return true;
}

/************** class Component ****************/

Mixture::Component::Component() : links(new list<ag_st_id>()){};
Mixture::Component::Component(const Component& comp) : agents(comp.size(),nullptr) {
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

const vector<const Mixture::Agent*>::const_iterator Mixture::Component::begin() const{
	agents.begin();
	return agents.begin();
}
const vector<const Mixture::Agent*>::const_iterator Mixture::Component::end() const{
	return agents.end();
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
	list<pair<const Agent*,short> > reorder;
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


/*****************************************/

template <typename T1,typename T2>
bool ComparePair<T1,T2>::operator()(pair<T1,T2> p1,pair<T1,T2> p2){
	return p1.first < p2.first ? true : (p1.second < p2.second ? true : false );
}

} /* namespace pattern */
