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

Mixture::Mixture(const Mixture& m) : declaredComps(m.declaredComps),
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
			for(auto ag : *comp)
				delete ag;
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
		throw std::invalid_argument("Cannot call declareAgents() on a initialized Mixture");
	for(size_t i = 0; i < agentCount ; i++){
		agents[i] = &env.declareAgentPattern(agents[i],is_lhs);
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
Mixture::Agent::~Agent(){
	1;
};

Mixture::Site& Mixture::Agent::addSite(short env_site){
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

int Mixture::Agent::compare(const Agent &a,set<small_id>& already_done) const {
	int ret = 0;
	if(this == &a)
		return 0;
	if(signId == a.signId){
		for(auto &id_site : interface){
			try{
				int ret_b = id_site.second.compare(a.getSite(id_site.first));
				if(ret_b * ret == -1)
					ret = -100;
				else
					ret = ret ? ret : ret_b;
				already_done.emplace(id_site.first);
			}
			catch(std::out_of_range &e){
				if(!id_site.second.isEmptySite() ||
						id_site.second.link_type != WILD){
					if(ret == 1)
						ret = -100;
					else
						ret = ret ? ret : -1;
				}
			}
		}
		for(auto &id_site : a.interface){
			if(already_done.count(id_site.first))
				continue;
			//already_done.emplace(id_site.first);
			try{
				int ret_b = -1*id_site.second.compare(getSite(id_site.first));
				if(ret_b * ret == -1)
					ret = -100;
				else
					ret = ret ? ret : ret_b;
			}
			catch(std::out_of_range &e){
				if(!id_site.second.isEmptySite() ||
						id_site.second.link_type != WILD){
					if(ret == -1)
						return -100;
					else
						ret = ret ? ret : 1;
				}
			}
		}
	}
	else
		throw False();
	return ret;
}

void Mixture::Agent::setSiteValue(small_id site_id,small_id lbl_id){
	//interface[site_id].val_type = ValueType::LABEL;
	interface[site_id].state.set(lbl_id);
}
void Mixture::Agent::setSiteValue(small_id site_id,FL_TYPE val){
	//interface[site_id].val_type = ValueType::LABEL;
	interface[site_id].state.set(val);
}
void Mixture::Agent::setLinkPtrn(small_id trgt_site,small_id ag_ptrn,small_id site_ptrn){
	interface.at(trgt_site).lnk_ptrn = make_pair(ag_ptrn,site_ptrn);
}


void Mixture::Agent::addParent(small_id id,Agent *a){
	parents[id].emplace_back(a);
}
void Mixture::Agent::addParents(small_id id,const list<Agent*>& la){
	parents[id].insert(parents[id].end(),la.begin(),la.end());
}
void Mixture::Agent::addChild(small_id id,Agent *a){
	childs[id].emplace_back(a);
}
void Mixture::Agent::addChilds(small_id id,const list<Agent*>& la){
	childs[id].insert(childs[id].end(),la.begin(),la.end());
}

const list<Mixture::Agent*>& Mixture::Agent::getParentPatterns(small_id id) const {
	static list<Mixture::Agent*> empty;
	try{
		return parents.at(id);
	} catch(std::out_of_range &ex){}
	return empty;
}
const list<Mixture::Agent*>& Mixture::Agent::getChildPatterns(small_id id) const {
	static list<Mixture::Agent*> empty;
	try{
		return childs.at(id);
	} catch(std::out_of_range &ex){}
	return empty;
}
const map<small_id,list<Mixture::Agent*> >& Mixture::Agent::getParentPatterns() const {
	return parents;
}
const map<small_id,list<Mixture::Agent*> >& Mixture::Agent::getChildPatterns() const {
	return childs;
}


void Mixture::Agent::addCc(const Mixture::Component* cc,small_id id) const{
	includedIn.emplace_back(cc,id);
}
const list<pair<const Mixture::Component*,small_id> >& Mixture::Agent::getIncludes() const{
	return includedIn;
}


string Mixture::Agent::toString(const Environment& env, short mixAgId,
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
				if(labelSite && !it->second.isEmptySite())//is not an empty site
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
					if(it->second.isBindToAny())
						out += "!_";
					else{
						signature2bind = & env.getSignature( it->second.lnk_ptrn.first );
						site2bind = & signature2bind->getSite( it->second.lnk_ptrn.second );
						out += "!{" + site2bind->getName() + "." + signature2bind->getName()+"}";
					}
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
Mixture::Site::Site() : state((small_id)-1),link_type(FREE),lnk_ptrn(-1,-1) {}

bool Mixture::Site::isEmptySite() const{
	static auto empty = (small_id)-1;
	return state.smallVal == empty;
}
bool Mixture::Site::operator ==(const Site &s) const{
	if(memcmp(&state,&s.state,sizeof(state::SomeValue))) //only valid if union in state is cleaned at constructor
		return false;
	if(s.link_type == link_type){
		if(link_type >= LinkType::BIND && s.lnk_ptrn != lnk_ptrn)//bind or bind_to //TODO PATH
			return false;
	}
	else return false;
	return true;
}
int Mixture::Site::compare(const Site &s) const{
	int ret = 0;
	/*cout << sizeof(state::SomeValue) << endl;
	cout << sizeof(double) << endl;
	for(int i = 0; i < sizeof(state::SomeValue); i++)
		cout << (unsigned)((char*)(&state))[i] << "-" << (unsigned)((char*)(&s.state))[i] << " ";
	cout << endl;*/
	//if(memcmp(&state,&s.state,sizeof(state::SomeValue))){ //only valid if union in state is cleaned at constructor
	if(state.fVal != s.state.fVal || state.t != s.state.t){
		if(isEmptySite())
			ret = 1;
		else{
			if(s.isEmptySite())
				ret = -1;
			else
				throw False();
		}
	}
	if(s.link_type == link_type){
		if(link_type == LinkType::BIND_TO && s.lnk_ptrn != lnk_ptrn)
			throw False();
		if(link_type == LinkType::BIND)
			switch(compareLinkPtrn(s.lnk_ptrn)){
			case 0: return ret;
			case 1: return ret > -1 ? 1 : -100;
			case -1:return ret < 1 ? -1 : -100;
			default: break;//never
			}
	}
	else {
		if(link_type == WILD)
			return ret != -1 ? 1 : -100;
		else
			if(s.link_type == WILD)
				return ret != 1 ? -1 : -100;
			else
				if(link_type * s.link_type == 6)
					switch(compareLinkPtrn(s.lnk_ptrn)){
					case 0:return s.link_type == LinkType::BIND_TO ? (ret == 1 ? -100 : -1) : ret;
					case 1:return ret > -1 ? 1 : -100;
					case -1:return ret < 1 ? -1 : -100;
					default: break;//never
					}
				else
					throw False();
	}
	return ret;
}

int Mixture::Site::compareLinkPtrn(ag_st_id ptrn) const{
	if(isBindToAny()){
		if(ptrn.first == small_id(-1))
			return 0;
		else
			return 1;
	}
	else{
		if(ptrn.first == small_id(-1))
			return -1;
		else
			if(lnk_ptrn == ptrn)
				return link_type == LinkType::BIND_TO ? 1 : 0;//need to compare with other type
			else
				throw False();
	}
}

bool Mixture::Site::isBindToAny() const{
	return link_type == LinkType::BIND && lnk_ptrn.first == small_id(-1);
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
