/*
 * Node.cpp
 *
 *  Created on: Oct 18, 2017
 *      Author: naxo
 */

#include <cstring>
#include "Node.h"
#include "SiteGraph.h"
#include "../matching/Injection.h"
#include "../pattern/Environment.h"
#include "../pattern/mixture/Agent.h"

namespace state {



/********* Node Class ************/
Node::Node(const pattern::Signature& sign) : signId(sign.getId()),address(-1),
		intfSize(sign.getSiteCount()),deps(new InjSet()){
	//deps->reserve(10);
	interface = new Internal[intfSize];
	for(int i = 0; i < intfSize; i++){
		interface[i].val = sign.getSite(i).getDefaultValue();
	}
}

Node::Node(const Node& node) : signId(),address(-1),
		intfSize(0),deps(nullptr),interface(nullptr){//useless
	throw invalid_argument("do not call basic copy constructor of node.");
}

Node& Node::operator=(const Node& node){
	throw invalid_argument("do not call operator=() of node.");
}

Node::Node(const Node& node,const map<Node*,Node*>& mask) :
		signId(node.signId),address(-1),intfSize(node.intfSize),deps(new InjSet()){
	interface = new Internal[intfSize];
	for(small_id i = 0; i < intfSize; i++){
		interface[i].val = node.interface[i].val;
		if(node.interface[i].link.first)
			try{
				auto lnk = mask.at(node.interface[i].link.first);//maybe operator[]
				if(lnk && lnk != node.interface[i].link.first){
					interface[i].link.first = lnk;
					interface[i].link.second = node.interface[i].link.second;
					lnk->interface[interface[i].link.second].link.first = this;//maybe twice TODO
					lnk->interface[interface[i].link.second].link.second = i;
				}
			}
			catch(std::out_of_range &ex){
				throw invalid_argument("Calling copy constructor with an incomplete mask");
			}
		else
			interface[i].link = node.interface[i].link;
	}
}

Node::~Node() {
	//cout << "destroying node " << this << endl;
	delete[] interface;
	delete deps;
}

void Node::copyDeps(const Node& node,EventInfo& ev,matching::InjRandContainer** injs,
		const state::State& state) {
	for(small_id i = 0; i < intfSize; i++){
		for(auto inj : *node.interface[i].deps.first){
			const list<matching::Injection*>* new_injs;
			try {
				new_injs = &(ev.inj_mask.at(inj));
			}
			catch(out_of_range &ex){
				new_injs = &(injs[inj->pattern().getId()]->emplace(inj,ev.new_cc,state));
				ev.inj_mask[inj] = *new_injs;
			}
			for(auto new_inj : *new_injs){
				interface[i].deps.first->emplace(new_inj);
				ev.to_update.emplace(&new_inj->pattern());
			}
		}
		for(auto inj : *node.interface[i].deps.second){
			const list<matching::Injection*>* new_injs;
			try {
				new_injs = &ev.inj_mask.at(inj);
			}
			catch(out_of_range &ex){
				new_injs = &injs[inj->pattern().getId()]->emplace(inj,ev.new_cc,state);
				ev.inj_mask[inj] = *new_injs;
			}
			for(auto new_inj : *new_injs){
				interface[i].deps.second->emplace(new_inj);
				ev.to_update.emplace(&new_inj->pattern());
			}
		}
	}
	for(auto inj : *node.deps){
		const list<matching::Injection*>* new_injs;
		try {
			new_injs = &ev.inj_mask.at(inj);
		}
		catch(out_of_range &ex){
			new_injs = &injs[inj->pattern().getId()]->emplace(inj,ev.new_cc,state);
			ev.inj_mask[inj] = *new_injs;
		}
		for(auto new_inj : *new_injs){
			deps->emplace(new_inj);
			ev.to_update.emplace(&new_inj->pattern());
		}
	}
}

void Node::alloc(big_id addr){
	address = addr;
}

big_id Node::getAddress() const {
	return address;
}

void Node::setCount(pop_size q){
	throw invalid_argument("Cannot set population of basic Node");
}

pop_size Node::getCount() const {
	return 1;
}

short_id Node::getId() const{
	return signId;
}

bool Node::test(Node* &node,const pair<small_id,pattern::Mixture::Site>& id_site,
		two<list<Internal*> > &port_list, const State& state,
		const expressions::AuxMap& aux_map) const{
	auto& port = interface[id_site.first];
	if(!id_site.second.isEmptySite()){
		if( id_site.second.testValueOpt(port.val, state, aux_map) )
			port_list.first.emplace_back(&port);
		else
			return false;
	}

	switch(id_site.second.link_type){
	case pattern::Mixture::WILD:
		break;
	case pattern::Mixture::FREE:
		if(port.link.first)
			return false;
		//else
		port_list.second.emplace_back(&port);
		break;
	case pattern::Mixture::BIND:
		if(!port.link.first)
			return false;
		//else
		port_list.second.emplace_back(&port);
		/* Do not test for site_id ptrn, it's obvious that they are ok*/
		//else if(port.link.first->getLinkState(id_site.second.lnk_ptrn.second).second == id_site.first)//bind-node lnk-ptrn*/
		if(id_site.second.lnk_ptrn.first != small_id(-1))//ptrn is not bind to any
			node = port.link.first;
		break;
	case pattern::Mixture::BIND_TO:
		if(port.link.first){
			if(port.link.first->getId() != id_site.second.lnk_ptrn.first
					|| port.link.second != id_site.second.lnk_ptrn.second)
				return false;
		} else
			return false;
		port_list.second.emplace_back(&port);
		break;
	case pattern::Mixture::PATH:
		//TODO
		break;
	}
	return true;
}


template <typename T>
void Node::setState(small_id site_id,T value){
	throw std::invalid_argument("Node::setState(): not a valid state value.");
}

template <> void Node::setState(small_id site_id,SomeValue value){
	interface[site_id].val = value;
}
template <> void Node::setState(small_id site_id,small_id value){
	interface[site_id].val.smallVal = value;
}
template <> void Node::setState(small_id site_id,float value){
	interface[site_id].val.fVal = value;
}
template <> void Node::setState(small_id site_id,int value){
	interface[site_id].val.iVal = value;
}

void Node::setLink(small_id site_src,Node* lnk,small_id site_trgt){
	interface[site_src].link.first = lnk;
	interface[site_src].link.second = site_trgt;
}



void Node::removeFrom(EventInfo& ev,matching::InjRandContainer** injs,SiteGraph& graph) {
	graph.remove(this);
	for(small_id i = 0; i < intfSize; i++){
		interface[i].negativeUpdate(ev,injs);
		auto lnk = interface[i].link;
		if(lnk.first){
			lnk.first->interface[lnk.second].negativeUpdate(ev,injs,lnk.first->interface[lnk.second].deps.second);
			lnk.first->setLink(lnk.second,nullptr,0);//maybe this first TODO
			ev.side_effects.emplace(lnk);
		}
	}
	Internal::negativeUpdate(ev,injs,deps);
	ev.side_effects.erase(this);
	delete this;//TODO do not delete, reuse nodes
}

void Node::changeIntState(EventInfo& ev,matching::InjRandContainer** injs,small_id id,small_id value){
	if(interface[id].val.smallVal == value){
		ev.warns++;
	}
	else{
		interface[id].val.set(value);
		interface[id].negativeUpdate(ev,injs,interface[id].deps.first);
	}
}

void Node::assign(EventInfo& ev,matching::InjRandContainer** injs,small_id id,const SomeValue& val){
	if(interface[id].val == val){
		ev.warns++;
	}
	else{
		interface[id].val = val;//TODO opertor= ??
		interface[id].negativeUpdate(ev,injs,interface[id].deps.first);
	}
}

void Node::unbind(EventInfo& ev,matching::InjRandContainer** injs,small_id id,bool side_eff){
	auto& inter = interface[id];
	if(inter.link.first){
		if(side_eff)
			ev.side_effects.emplace(inter.link);
		inter.link.first->interface[inter.link.second].negativeUpdate(ev,//?? private?
				injs,inter.link.first->interface[inter.link.second].deps.second);
		inter.link.first->setLink(inter.link.second,nullptr,0);
		inter.negativeUpdate(ev,injs,inter.deps.second);
		inter.link.first = nullptr;
	}
	else
		ev.warns++;
}
void Node::bind(EventInfo& ev,matching::InjRandContainer** injs,small_id id,Node* trgt_node,
		small_id trgt_site,bool side_eff){
	//auto lnk2 = trgt_node->getLinkState(trgt_site);
	auto& lnk1 = interface[id].link;
	if(lnk1.first){
		if(lnk1.first == trgt_node && lnk1.second == trgt_site)
			return;
		if(side_eff)//side-eff for node1, node2? TODO
			ev.side_effects.emplace(lnk1);
		lnk1.first->interface[lnk1.second].negativeUpdate(ev,injs,lnk1.first->interface[lnk1.second].deps.second);
		lnk1.first->setLink(lnk1.second,nullptr,0);
		ev.side_effects.emplace(lnk1);
	}
	interface[id].negativeUpdate(ev,injs,interface[id].deps.second);
	lnk1.first = trgt_node;lnk1.second = trgt_site;
	lnk1.first->bind(ev,injs,trgt_site,this,id,side_eff);
}

Internal* Node::begin(){
	return interface;
}
Internal* Node::end(){
	return interface+intfSize;
}

const SomeValue& Node::getInternalState(small_id id) const{
	return interface[id].val;
}
const pair<Node*,small_id>& Node::getLinkState(small_id id) const{
	return interface[id].link;
}

two<InjSet*>& Node::getLifts(small_id site){
	return interface[site].deps;
}

void Node::addDep(matching::Injection* inj) {
	deps->emplace(inj);
}
bool Node::removeDep(matching::Injection* inj) {
	return deps->erase(inj);
}



/*********** MultiNode *****************/

MultiNode::MultiNode(unsigned n,pop_size pop) :
		population(pop),count(0) {
	cc = new SubNode*[n];
}
MultiNode::~MultiNode(){
	delete[] cc;
}

pop_size MultiNode::getPopulation() const {
	return population;
}

small_id MultiNode::add(SubNode* node){
	cc[count] = node;
	//count++;
	return count++;//count-1;//test
}

void MultiNode::remove() {
	count--;
	if(!count)
		delete this;
}

void MultiNode::dec(EventInfo& ev) {
	population--;//TODO its ok???
	if(population == 0){
		throw OutOfInstances();
	}
	//or here
	for(small_id i = 0; i < count; i++){
		ev.new_cc.emplace(cc[i],nullptr);
	}
}

/************ SubNode ******************/

SubNode::SubNode(const pattern::Signature& sign,MultiNode& multi) :
		Node(sign),cc(multi) {
	multiId = multi.add(this);
};

SubNode::~SubNode(){
	cc.remove();
}

pop_size SubNode::getCount() const {
	return cc.getPopulation();
}

void SubNode::removeFrom(EventInfo& ev,matching::InjRandContainer** injs,SiteGraph& graph){
	if(!ev.new_cc.count(this))
		try { cc.dec(ev); } catch(MultiNode::OutOfInstances &ex){
			Node::removeFrom(ev,injs,graph);
			graph.remove(this);
			//ev.new_cc[this] = this;
			//graph.decPopulation();//will be decreased when calling Node::removeFrom()
			return;
		}
	if(ev.new_cc[this]){
		ev.side_effects.erase(ev.new_cc[this]);//erasing possible side-effects on this node.
		delete ev.new_cc[this];
	}
	ev.new_cc[this] = this;
	graph.decPopulation();
	for(auto i = 0; i < intfSize; i++){
		for(auto dep : *interface[i].deps.first)//necessary? TODO
			ev.inj_mask[dep].clear();
		for(auto dep : *interface[i].deps.second)
			ev.inj_mask[dep].clear();
		auto lnk = interface[i].link;
		if(lnk.first){
			auto& opt_lnkd_node = ev.new_cc[lnk.first];
			if(opt_lnkd_node == lnk.first){
				ev.side_effects.erase(lnk.first);//maybe unnecessary
			}
			else{
				for(auto dep : *lnk.first->getLifts(lnk.second).second)//TODO review
					ev.inj_mask[dep].clear();
				if(opt_lnkd_node == nullptr)
					opt_lnkd_node = new Node(*lnk.first,ev.new_cc);
				ev.side_effects.emplace(opt_lnkd_node,i);
			}
			//new_lnk_node->setLink(lnk.second,nullptr,0);//set link at constructor??
		}
	}
	for(auto dep : *deps)
		ev.inj_mask[dep].clear();
}

void SubNode::changeIntState(EventInfo& ev,matching::InjRandContainer** injs,small_id id,small_id value){
	if(!ev.new_cc.count(this))
		try { cc.dec(ev); } catch(MultiNode::OutOfInstances &ex){
			Node::changeIntState(ev,injs,id,value);
			return;
		}
	auto& new_node = ev.new_cc.at(this);
	if(new_node == nullptr)
		new_node = new Node(*this,ev.new_cc);
	if(interface[id].val.smallVal == value){
		ev.warns++;
	}
	else
		new_node->setState(id,value);
	for(auto dep : *interface[id].deps.first)
		ev.inj_mask[dep].clear();
}

void SubNode::assign(EventInfo& ev,matching::InjRandContainer** injs,small_id id,const SomeValue& value){
	if(!ev.new_cc.count(this))
		try { cc.dec(ev); } catch(MultiNode::OutOfInstances &ex){
			Node::assign(ev,injs,id,value);
			return;
		}
	auto& new_node = ev.new_cc.at(this);
	if(new_node == nullptr)
		new_node = new Node(*this,ev.new_cc);
	if(interface[id].val == value){
		ev.warns++;
	}
	else
		new_node->setState(id,value);
	for(auto dep : *interface[id].deps.first)
		ev.inj_mask[dep].clear();
}

void SubNode::unbind(EventInfo& ev,matching::InjRandContainer** injs,small_id id,bool side_eff){
	//unbind must be inside cc
	auto& lnk = this->interface[id].link;
	if(!lnk.first){
		ev.warns++;
		return;//null_event
	}
	if(!ev.new_cc.count(this))
		try { cc.dec(ev); } catch(MultiNode::OutOfInstances &ex){
			Node::unbind(ev,injs,id);
			return;
		}
	auto& node1 = ev.new_cc.at(this);
	auto& node2 = ev.new_cc.at(lnk.first);
	if(node1 == nullptr)
		node1 = new Node(*this,ev.new_cc);
	if(node2 == nullptr)
		node2 = new Node(*lnk.first,ev.new_cc);;
	if(side_eff)
		ev.side_effects.emplace(node2,lnk.second);
	node1->setLink(id,nullptr,0);
	node2->setLink(lnk.second,nullptr,0);
	for(auto dep : *interface[id].deps.second)
		ev.inj_mask[dep].clear();
	for(auto dep : *lnk.first->getLifts(lnk.second).second)
		ev.inj_mask[dep].clear();
}

void SubNode::bind(EventInfo& ev,matching::InjRandContainer** injs,small_id id,Node* trgt_node,
		small_id trgt_site,bool side_eff){
	if(!ev.new_cc.count(this))
		try { cc.dec(ev); } catch(MultiNode::OutOfInstances &ex){
			Node::bind(ev,injs,id,trgt_node,trgt_site,side_eff);
			return;
		}
	auto& new_node = ev.new_cc.at(this);
	if(new_node == nullptr)
		new_node = new Node(*this,ev.new_cc);
	auto& lnk = trgt_node->getLinkState(trgt_site);
	/*for(auto it = ev.side_effects.find(this);it != ev.side_effects.end(); it++){
		ev.side_effects.emplace(new_node,it->second);
		ev.side_effects.erase(it);//TODO this may invalidate it++
	}*/
	if(lnk.first && lnk.first == this)//TODO compare with site?
		trgt_node->setLink(trgt_site,new_node,lnk.second);
	try{
		auto& new_trgt_node = ev.new_cc.at(trgt_node);
		if(new_trgt_node){//TODO check if true
			//throw invalid_argument("link between 2 sites of same agent, this cannot happen. do not make sense");
		}
		else{
			//link with a node in the same cc // or in other multi cc
			new_trgt_node = new Node(*trgt_node,ev.new_cc);
		}
		new_node->Node::bind(ev,injs,id,new_trgt_node,trgt_site,side_eff);
	}
	catch(std::out_of_range &e){
		new_node->Node::bind(ev,injs,id,trgt_node,trgt_site,side_eff);
	}
	for(auto dep : *interface[id].deps.second)
		ev.inj_mask[dep].clear();
	if(interface[id].link.first){
		for(auto dep : *interface[id].link.first->getLifts(trgt_site).second)
			ev.inj_mask[dep].clear();
	}

}

/********** Internal struct ************/

Internal::Internal() : val(small_id(-1)),link(nullptr,0),
		deps(new InjSet(),new InjSet()){}

Internal::~Internal(){
	//cout << "deps count: " << deps.first->size() << " - " << deps.second->size() << endl;
	delete deps.first;
	delete deps.second;
	deps.first = nullptr;
	deps.second = nullptr;
}

void Internal::negativeUpdate(EventInfo& ev,matching::InjRandContainer** injs){
	negativeUpdate(ev,injs,deps.first);
	negativeUpdate(ev,injs,deps.second);
}

void Internal::negativeUpdate(EventInfo& ev,matching::InjRandContainer** injs,InjSet* deps){
	auto dep_it = deps->begin();
	auto nxt = dep_it;
	auto end = deps->end();
	while(dep_it != end){
		auto inj = *dep_it;
		nxt = next(dep_it);
		if(inj->isTrashed())
			deps->erase(dep_it);
		else{
			auto& emb = (inj)->getEmbedding();
			auto& cc = (inj)->pattern();
			for(unsigned i = 0; i < emb.size(); i++){
				if(emb[i]->removeDep(inj))
					continue;
				auto& mix_ag = cc.getAgent(i);
				//int n = 0;
				//int m = 1;
				for(auto& id_site : mix_ag){
					if(id_site.second.link_type != pattern::Pattern::WILD)
						/*m++,n += */emb[i]->getLifts(id_site.first).second->erase(inj);
					if(!id_site.second.isEmptySite())
						/*m++,n += */emb[i]->getLifts(id_site.first).first->erase(inj);
				}
				//cout << "erase calls = " << m << " || erased elems = " << n << endl;
			}
			injs[cc.getId()]->erase(inj);
			//delete *dep_it;//TODO reuse injections
			ev.to_update.emplace(&cc);
		}
		dep_it = nxt;
	}
}

string Node::toString(const pattern::Environment &env,bool show_binds,map<const Node*,bool> *visited) const {
	auto& sign = env.getSignature(signId);
	string s(sign.getName()+"(");
	for(small_id i = 0; i < intfSize; i++){
		s += interface[i].toString(sign.getSite(i),show_binds,visited) + ",";
	}
	if(intfSize)
		s.back() = ')';
	else
		s += ")";
	if(visited){
		(*visited)[this] = true;
		for(auto nhb : *visited)
			if(!nhb.second)
				s += ", " + nhb.first->toString(env, show_binds, visited);
	}
	return s;
}

string SubNode::toString(const pattern::Environment &env,bool show_binds,map<const Node*,bool> *visit) const {
	return to_string(cc.getPopulation())+" " + Node::toString(env,show_binds,visit);
}

string Internal::toString(const pattern::Signature::Site& sit,bool show_binds,map<const Node*,bool> *visit) const {
	string s(sit.getName());
	try{
		dynamic_cast<const pattern::Signature::EmptySite&>(sit);
	}
	catch(std::bad_cast &e){
		s += "~";
		switch(val.t){
		case FLOAT:
			s += to_string(val.fVal);
			break;
		case INT:
			s += to_string(val.iVal);
			break;
		case SHORT_ID:
			s += dynamic_cast<const pattern::Signature::LabelSite&>(sit)
					.getLabel(val.shortVal);
			break;
		case SMALL_ID:
			s += dynamic_cast<const pattern::Signature::LabelSite&>(sit)
					.getLabel(val.smallVal);
			break;
		case NONE:
			s += "AUX";
			break;
		default:
			s += "?";//TODO exception?
		}
	}
	if(show_binds)
		if(link.first){
			s += "!("+to_string(link.first->address)+")";
			if(visit)
				(*visit)[link.first];
		}

	return s;
}



EventInfo::EventInfo() : emb(nullptr),cc_count(0),fresh_emb(nullptr),warns(0) {
	//TODO these numbers are arbitrary!!
	emb = new Node**[4];
	for(int i = 0; i < 4 ; i++)
		emb[i] = new Node*[12];
	fresh_emb = new Node*[5];
}

EventInfo::~EventInfo(){
	if(emb){
		for(small_id i = 0; i < 4; i++)
			delete[] emb[i];
		delete[] emb;
	}
	if(fresh_emb)
		delete[] fresh_emb;
}

void EventInfo::clear(small_id _cc_count){
	side_effects.clear();
	pert_ids.clear();
	new_cc.clear();
	inj_mask.clear();
	to_update.clear();
	rule_ids.clear();
	aux_map.clear();
	warns = 0;
	cc_count = _cc_count;
}



} /* namespace state */
