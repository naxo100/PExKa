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

namespace state {



/********* Node Class ************/
Node::Node(const pattern::Signature& sign) : signId(sign.getId()),address(-1),
		intfSize(sign.getSiteCount()),deps(new matching::InjSet()){
	interface = new Internal[intfSize];
	for(int i = 0; i < intfSize; i++){
		interface[i].val = sign.getSite(i).getDefaultValue();
	}
}

Node::Node(const Node& node){
	throw invalid_argument("do not call basic copy constructor of node.");
}

Node& Node::operator=(const Node& node){
	throw invalid_argument("do not call operator=() of node.");
}

Node::Node(const Node& node,const map<Node*,Node*>& mask) :
		signId(node.signId),address(-1),intfSize(node.intfSize),deps(new matching::InjSet()){
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

void Node::copyDeps(const Node& node,EventInfo& ev,matching::InjRandSet* injs) {
	for(small_id i = 0; i < intfSize; i++){
		for(auto inj : *node.interface[i].deps.first){
			matching::Injection* new_inj;
			try {
				new_inj = ev.inj_mask.at(inj);
			}
			catch(out_of_range &ex){
				new_inj = ev.inj_mask[inj] = injs[inj->pattern().getId()].emplace(inj,ev.new_cc);
			}
			if(new_inj){
				interface[i].deps.first->emplace(new_inj);
			}
		}
		for(auto inj : *node.interface[i].deps.second){
			matching::Injection* new_inj;
			try {
				new_inj = ev.inj_mask.at(inj);
			}
			catch(out_of_range &ex){
				new_inj = ev.inj_mask[inj] = injs[new_inj->pattern().getId()].emplace(inj,ev.new_cc);
			}
			if(new_inj){
				interface[i].deps.second->emplace(new_inj);
			}
		}
	}
	for(auto inj : *deps){
		matching::Injection* new_inj;
		try {
			new_inj = ev.inj_mask.at(inj);
		}
		catch(out_of_range &ex){
			new_inj = ev.inj_mask[inj] = injs[new_inj->pattern().getId()].emplace(inj,ev.new_cc);
		}
		if(new_inj){
			deps->emplace(new_inj);
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

bool Node::test(const pair<small_id,pattern::Mixture::Site>& id_site,
		std::queue<pair<small_id,Node&> >& match_queue,
		two<list<Internal*> > &port_list) const{
	auto& port = interface[id_site.first];
	if(!id_site.second.isEmptySite()){
		if(memcmp(&port.val,&id_site.second.state,sizeof(state::SomeValue)))
			throw False();
		else
			port_list.first.emplace_back(&port);
	}
	switch(id_site.second.link_type){
	case pattern::Mixture::WILD:
		break;
	case pattern::Mixture::FREE:
		if(port.link.first)
			throw False();
		//else
		port_list.second.emplace_back(&port);
		break;
	case pattern::Mixture::BIND:
		if(!port.link.first)
			throw False();//or return port.link.first;
		//else
		port_list.second.emplace_back(&port);
		match_queue.emplace(id_site.first,*(port.link.first));//its site_id instead ag_mix_id
		return true;
		break;
	case pattern::Mixture::BIND_TO:
		if(port.link.first){
			if(port.link.first->getId() != id_site.second.lnk_ptrn.first
					|| port.link.second != id_site.second.lnk_ptrn.second)
				throw False();
		} else
			throw False();
		port_list.second.emplace_back(&port);
		break;
	case pattern::Mixture::PATH:
		//TODO
		break;
	}
	return false;
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



void Node::removeFrom(EventInfo& ev,matching::InjRandSet* injs,SiteGraph& graph) {
	graph.remove(this);
	for(small_id i = 0; i < intfSize; i++){
		interface[i].negativeUpdate(ev,injs);
		auto lnk = interface[i].link;
		if(lnk.first){
			lnk.first->interface[lnk.second].negativeUpdate(ev,injs,lnk.first->interface[lnk.second].deps.second);
			lnk.first->setLink(lnk.second,nullptr,0);
			ev.side_effects.emplace(lnk);
		}
	}
	Internal::negativeUpdate(ev,injs,deps);
	delete this;//TODO do not delete, reuse nodes
}

void Node::changeIntState(EventInfo& ev,matching::InjRandSet* injs,small_id id,small_id value){
	if(interface[id].val.smallVal == value){
		ev.warns++;
	}
	else{
		interface[id].val.set(value);
		interface[id].negativeUpdate(ev,injs,interface[id].deps.first);
	}
}
void Node::unbind(EventInfo& ev,matching::InjRandSet* injs,small_id id){
	auto& inter = interface[id];
	if(inter.link.first){
		inter.link.first->interface[inter.link.second].negativeUpdate(ev,//?? private?
				injs,inter.link.first->interface[inter.link.second].deps.second);
		inter.link.first->setLink(inter.link.second,nullptr,0);
		inter.negativeUpdate(ev,injs,inter.deps.second);
		inter.link.first = nullptr;
	}
	else
		ev.warns++;
}
void Node::bind(EventInfo& ev,matching::InjRandSet* injs,small_id id,Node* trgt_node,small_id trgt_site){
	//auto lnk2 = trgt_node->getLinkState(trgt_site);
	auto& lnk1 = interface[id].link;
	if(lnk1.first){
		if(lnk1.first == trgt_node && lnk1.second == trgt_site)
			return;
		lnk1.first->interface[lnk1.second].negativeUpdate(ev,injs,lnk1.first->interface[lnk1.second].deps.second);
		lnk1.first->setLink(lnk1.second,nullptr,0);
		ev.side_effects.emplace(lnk1);
	}
	interface[id].negativeUpdate(ev,injs,interface[id].deps.second);
	lnk1.first = trgt_node;lnk1.second = trgt_site;
	lnk1.first->bind(ev,injs,trgt_site,this,id);
}

Node::Internal* Node::begin(){
	return interface;
}
Node::Internal* Node::end(){
	return interface+intfSize;
}

const SomeValue& Node::getInternalState(small_id id){
	return interface[id].val;
}
const pair<Node*,small_id>& Node::getLinkState(small_id id){
	return interface[id].link;
}

pair<matching::InjSet*,matching::InjSet*>& Node::getLifts(small_id site){
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
	if(population == 1){
		throw OutOfInstances();
	}
	population--;
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

void SubNode::removeFrom(EventInfo& ev,matching::InjRandSet* injs,SiteGraph& graph){
	if(!ev.new_cc.size())
		try { cc.dec(ev); } catch(MultiNode::OutOfInstances &ex){
			Node::removeFrom(ev,injs,graph);
			graph.remove(this);
			//ev.new_cc[this] = this;
			//graph.decPopulation();//will be decreased when calling Node::removeFrom()
			return;
		}
	ev.new_cc[this] = this;
	graph.decPopulation();
	for(auto i = 0; i < intfSize; i++){
		for(auto dep : *interface[i].deps.first)
			ev.inj_mask[dep] = nullptr;
		for(auto dep : *interface[i].deps.second)
			ev.inj_mask[dep] = nullptr;
		auto lnk = interface[i].link;
		if(lnk.first){
			for(auto dep : *lnk.first->getLifts(lnk.second).second)//TODO review
				ev.inj_mask[dep] = nullptr;
			ev.side_effects.emplace(lnk);
			auto new_lnk_node = new Node(*lnk.first,ev.new_cc);
			ev.new_cc[lnk.first] = new_lnk_node;
			//new_lnk_node->setLink(lnk.second,nullptr,0);//set link at constructor??
		}
	}
	for(auto dep : *deps)
		ev.inj_mask[dep] = nullptr;
}

void SubNode::changeIntState(EventInfo& ev,matching::InjRandSet* injs,small_id id,small_id value){
	if(!ev.new_cc.size())
		try { cc.dec(ev); } catch(MultiNode::OutOfInstances &ex){
			Node::changeIntState(ev,injs,id,value);
			return;
		}
	auto new_node = new Node(*this,ev.new_cc);
	if(interface[id].val.smallVal == value){
		ev.warns++;
	}
	else
		new_node->setState(id,value);
	ev.new_cc.at(this) = new_node;
	for(auto dep : *interface[id].deps.first)
		ev.inj_mask[dep] = nullptr;
}
void SubNode::unbind(EventInfo& ev,matching::InjRandSet* injs,small_id id){
	//unbind must be inside cc
	if(!ev.new_cc.size())
		try { cc.dec(ev); } catch(MultiNode::OutOfInstances &ex){
			Node::unbind(ev,injs,id);
			return;
		}
	auto& lnk = this->interface[id].link;
	auto node1 = new Node(*this,ev.new_cc);
	auto node2 = new Node(*lnk.first,ev.new_cc);
	ev.new_cc.at(this) = node1;
	ev.new_cc.at(lnk.first) = node2;
	node1->setLink(id,nullptr,0);
	node2->setLink(lnk.second,nullptr,0);
	for(auto dep : *interface[id].deps.second)
		ev.inj_mask[dep] = nullptr;
	for(auto dep : *lnk.first->getLifts(lnk.second).second)
		ev.inj_mask[dep] = nullptr;
}
void SubNode::bind(EventInfo& ev,matching::InjRandSet* injs,small_id id,Node* trgt_node,small_id trgt_site){
	if(!ev.new_cc.count(this))
		try { cc.dec(ev); } catch(MultiNode::OutOfInstances &ex){
			Node::bind(ev,injs,id,trgt_node,trgt_site);
			return;
		}
	auto new_node = new Node(*this,ev.new_cc);
	ev.new_cc.at(this) = new_node;
	auto& lnk = trgt_node->getLinkState(trgt_site);
	if(lnk.first && lnk.first == this)//TODO compare with site?
		trgt_node->setLink(trgt_site,new_node,lnk.second);
	try{
		auto new_trgt_node = ev.new_cc.at(trgt_node);
		if(new_trgt_node){//TODO check if true
			throw invalid_argument("link between 2 sites of same agent, this cannot happen. do not make sense");
		}
		else{
			//link with a node in the same cc
			new_trgt_node = new Node(*trgt_node,ev.new_cc);
			ev.new_cc[trgt_node] = new_trgt_node;
			new_node->Node::bind(ev,injs,id,new_trgt_node,trgt_site);
		}
	}
	catch(std::out_of_range &e){
		new_node->Node::bind(ev,injs,id,trgt_node,trgt_site);
	}
	for(auto dep : *interface[id].deps.second)
		ev.inj_mask[dep] = nullptr;
	if(interface[id].link.first){
		for(auto dep : *interface[id].link.first->getLifts(trgt_site).second)
			ev.inj_mask[dep] = nullptr;
	}

}

/********** Internal struct ************/

Node::Internal::Internal() : val(small_id(-1)),link(nullptr,0),
		deps(new matching::InjSet(),new matching::InjSet()){}

Node::Internal::~Internal(){
	delete deps.first;
	delete deps.second;
	deps.first = nullptr;
	deps.second = nullptr;
}

void Node::Internal::negativeUpdate(EventInfo& ev,matching::InjRandSet* injs){
	negativeUpdate(ev,injs,deps.first);
	negativeUpdate(ev,injs,deps.second);
}

void Node::Internal::negativeUpdate(EventInfo& ev,matching::InjRandSet* injs,matching::InjSet* deps){
	auto dep_it = deps->begin();
	auto nxt = dep_it;
	while(dep_it != deps->end()){
		auto inj = *dep_it;
		nxt = next(dep_it);
		if((*dep_it)->isTrashed())
			deps->erase(dep_it);
		else{
			auto& emb = (inj)->getEmbedding();
			auto& cc = (inj)->pattern();
			for(unsigned i = 0; i < emb.size(); i++){
				if(emb[i]->removeDep(inj))
					continue;
				auto& mix_ag = cc.getAgent(i);
				for(auto& id_site : mix_ag){
					if(id_site.second.link_type != pattern::Pattern::WILD)
						emb[i]->getLifts(id_site.first).second->erase(inj);
					switch(id_site.second.state.t){
					case BaseExpression::SMALL_ID:
						if(!id_site.second.isEmptySite())
							emb[i]->getLifts(id_site.first).first->erase(inj);
						break;
					default:
						break;//TODO other types of values
					}
				}
			}
			injs[cc.getId()].erase(inj);
			//delete *dep_it;//TODO reuse injections
			ev.to_update.emplace(&cc);
		}
		dep_it = nxt;
	}
}

string Node::toString(const pattern::Environment &env,bool show_binds) const {
	auto& sign = env.getSignature(signId);
	string s(sign.getName()+"(");
	for(small_id i = 0; i < intfSize; i++){
		s += interface[i].toString(sign.getSite(i),show_binds) + ",";
	}
	if(intfSize)
		s.back() = ')';
	else
		s += ")";
	return s;
}

string SubNode::toString(const pattern::Environment &env,bool show_binds) const {
	return to_string(cc.getPopulation())+" " + Node::toString(env,show_binds);
}

string Node::Internal::toString(const pattern::Signature::Site& sit,bool show_binds) const {
	string s(sit.getName());
	try{
		dynamic_cast<const pattern::Signature::EmptySite&>(sit);
	}
	catch(std::bad_cast &e){
		s += "~";
		switch(val.t){
		case BaseExpression::FLOAT:
			s += to_string(val.fVal);
			break;
		case BaseExpression::INT:
			s += to_string(val.iVal);
			break;
		case BaseExpression::SHORT_ID:
			s += dynamic_cast<const pattern::Signature::LabelSite&>(sit)
					.getLabel(val.shortVal);
			break;
		case BaseExpression::SMALL_ID:
			s += dynamic_cast<const pattern::Signature::LabelSite&>(sit)
					.getLabel(val.smallVal);
			break;
		default:
			s += "?";//TODO exception?
		}
	}
	if(show_binds)
		if(link.first)
			s += "!("+to_string(link.first->address)+")";
	return s;
}



EventInfo::EventInfo() : emb(nullptr),cc_count(0),fresh_emb(nullptr),warns(0) {}

EventInfo::~EventInfo(){
	if(emb){
		for(small_id i = 0; i < cc_count; i++)
			delete[] emb[i];
		delete[] emb;
	}
	if(fresh_emb)
		delete[] fresh_emb;
}



} /* namespace state */
