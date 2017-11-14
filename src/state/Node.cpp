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
Node::Node(const pattern::Signature& sign) : signId(sign.getId()),address(-1), intfSize(sign.getSiteCount()){
	interface = new Internal[intfSize];
	for(int i = 0; i < intfSize; i++){
		interface[i].val = sign.getSite(i).getDefaultValue();
	}
}

Node::Node(const Node& node,const map<Node*,Node*>& mask) : signId(node.signId),address(-1),intfSize(node.intfSize) {
	interface = new Internal[intfSize];
	for(small_id i = 0; i < intfSize; i++){
		interface[i].val = node.interface[i].val;
		try{
			auto lnk = mask.at(node.interface[i].link.first);//maybe operator[]
			if(lnk){
				interface[i].link.first = lnk;
				interface[i].link.second = node.interface[i].link.second;
				lnk->interface[interface[i].link.second].link.first = this;
				lnk->interface[interface[i].link.second].link.second = i;
			}
		}
		catch(std::out_of_range &ex){
			throw invalid_argument("Calling copy constructor with an incomplete mask");
		}
	}
}

Node::~Node() {
	delete[] interface;
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



void Node::removeFrom(EventInfo& ev,matching::InjSet* injs,SiteGraph& graph) {
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
	delete this;//TODO do not delete, reuse nodes
}

void Node::changeIntState(EventInfo& ev,matching::InjSet* injs,small_id id,small_id value){
	if(interface[id].val.smallVal == value){
		ev.warns++;
	}
	else{
		interface[id].val.set(value);
		interface[id].negativeUpdate(ev,injs,interface[id].deps.first);
	}
}
void Node::unbind(EventInfo& ev,matching::InjSet* injs,small_id id){
	auto& inter = interface[id];
	if(inter.link.first){
		inter.link.first->interface[inter.link.second].negativeUpdate(ev,//?? private?
				inter.link.first->interface[inter.link.second].deps.second);
		inter.link.first->setLink(inter.link.second,nullptr,0);
		inter.negativeUpdate(ev,injs,inter.deps.second);
		inter.link.first = nullptr;
	}
	else
		ev.warns++;
}
void Node::bind(EventInfo& ev,matching::InjSet* injs,small_id id,Node* trgt_node,small_id trgt_site){
	auto& lnk2 = trgt_node->getLinkState(trgt_site);
	auto& lnk1 = interface[id].link;
	if(lnk1.first){
		lnk1.first->interface[lnk1.second].negativeUpdate(ev,injs,lnk1.first->interface[lnk1.second].deps.second);
		lnk1.first->setLink(lnk1.second,nullptr,0);
		ev.side_effects.emplace(lnk1);
	}
	if(lnk2.first){
		lnk2.first->interface[lnk2.second].negativeUpdate(ev,injs,lnk2.first->interface[lnk2.second].deps.second);
		lnk2.first->setLink(lnk2.second,nullptr,0);
		ev.side_effects.emplace(lnk2);
	}
	interface[id].negativeUpdate(ev,injs,interface[id].deps.second);
	lnk1.first = trgt_node;lnk1.second = trgt_site;
	trgt_node->interface[trgt_site].negativeUpdate(ev,injs,trgt_node->interface[trgt_site].deps.second);
	trgt_node->setLink(trgt_site,this,id);
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



/*********** MultiNode *****************/

MultiNode::MultiNode(unsigned n,pop_size pop) :
		population(pop),count(0) {
	cc = new SubNode*[n];
}

pop_size MultiNode::getPopulation() const {
	return population;
}

small_id MultiNode::add(SubNode* node){
	cc[count] = node;
	//count++;
	return count++;//count-1;//test
}

void MultiNode::dec(EventInfo& ev) {
	for(small_id i = 0; i < count; i++){
		ev.new_cc.emplace(cc[i],nullptr);
	}
	population--;
	if(!population){
		delete this;
	}
}

/************ SubNode ******************/

SubNode::SubNode(const pattern::Signature& sign,MultiNode& multi) :
		Node(sign),cc(multi) {
	multiId = multi.add(this);
};
SubNode::~SubNode(){}

pop_size SubNode::getCount() const {
	return cc.getPopulation();
}

void SubNode::removeFrom(EventInfo& ev,matching::InjSet* injs,SiteGraph& graph){
	cc.dec(ev);
	ev.new_cc[this] = this;
}

void SubNode::changeIntState(EventInfo& ev,matching::InjSet* injs,small_id id,small_id value){
	cc.dec(ev);
	auto new_node = new Node(*this,ev.new_cc);
	new_node->setState(id,value);
	ev.new_cc.at(this) = new_node;
}
void SubNode::unbind(EventInfo& ev,matching::InjSet* injs,small_id id){
	//unbind must be inside cc
	cc.dec(ev);
	auto node1 = new Node(*this,ev.new_cc);
	auto node2 = new Node(*this->interface[id].link.first,ev.new_cc);
	ev.new_cc.at(this) = node1;
	ev.new_cc.at(this->interface[id].link.first) = node2;
}
void SubNode::bind(EventInfo& ev,matching::InjSet* injs,small_id id,Node* trgt_node,small_id trgt_site){
	cc.dec(ev);
	auto new_node = new Node(*this,ev.new_cc);
	ev.new_cc.at(this) = new_node;
	try{
		auto new_trgt_node = ev.new_cc.at(trgt_node);
		if(new_trgt_node){
			throw invalid_argument("link between 2 sites of same agent, this cannot happen. do not make sense");
		}
		else{
			//link with a node in the same cc
			new_trgt_node = new Node(*trgt_node,ev.new_cc);
			Node::bind(ev,injs,id,new_trgt_node,trgt_site);
		}
	}
	catch(std::out_of_range &e){
		Node::bind(ev,injs,id,trgt_node,trgt_site);
	}

}

/********** Internal struct ************/

Node::Internal::Internal() : val(small_id(-1)),link(nullptr,0),
		deps(new matching::InjSet(),new matching::InjSet()){}

Node::Internal::~Internal(){
	delete deps.first;
	delete deps.second;
}

void Node::Internal::negativeUpdate(EventInfo& ev,matching::InjSet* injs){
	negativeUpdate(ev,injs,deps.first);
	negativeUpdate(ev,injs,deps.second);
}

void Node::Internal::negativeUpdate(EventInfo& ev,matching::InjSet* injs,matching::InjSet* deps){
	for(auto dep_it = deps->begin();dep_it != deps->end();dep_it++){
		if((*dep_it)->isTrashed())
			deps->erase(dep_it);
		else{
			auto& emb = (*dep_it)->getEmbedding();
			auto& cc = (*dep_it)->pattern();
			for(unsigned i = 0; i < emb.size(); i++){
				auto& mix_ag = cc.getAgent(i);
				for(auto& id_site : mix_ag){
					switch(id_site.second.state.t){
					case BaseExpression::SMALL_ID:
						if(!id_site.second.isEmptySite())
							emb[i]->getLifts(id_site.first).first->erase(*dep_it);
						break;
					default:
						break;//TODO other types of values
					}
					switch(id_site.second.link_type){
					case pattern::Pattern::FREE:case pattern::Pattern::BIND:case pattern::Pattern::BIND_TO:
						emb[i]->getLifts(id_site.first).first->erase(*dep_it);
						break;
					default:
						break;
					}
				}
			}
			injs[cc.getId()].erase(*dep_it);
		}
	}
}

string Node::toString(const pattern::Environment &env) const {
	auto& sign = env.getSignature(signId);
	string s(sign.getName()+"(");
	for(small_id i = 0; i < intfSize; i++){
		s += interface[i].toString(sign.getSite(i));
		try{
			dynamic_cast<const pattern::Signature::EmptySite&>(sign.getSite(i));
		}
		catch(std::bad_cast &e){
			s += "~";
			switch(interface[i].val.t){
			case BaseExpression::FLOAT:
				s += to_string(interface[i].val.fVal);
				break;
			case BaseExpression::INT:
				s += to_string(interface[i].val.iVal);
				break;
			case BaseExpression::SHORT_ID:
				s += dynamic_cast<const pattern::Signature::LabelSite&>(sign.getSite(i))
						.getLabel(interface[i].val.shortVal);
				break;
			case BaseExpression::SMALL_ID:
				s += dynamic_cast<const pattern::Signature::LabelSite&>(sign.getSite(i))
						.getLabel(interface[i].val.smallVal);
				break;
			default:
				s += "?";//TODO exception?
			}
		}
		s += ",";
	}
	if(intfSize)
		s.back() = ')';
	else
		s += ")";
	return s;
}

string Node::Internal::toString(const pattern::Signature::Site& sit) const {
	string s(sit.getName());
	return s;
}


} /* namespace state */
