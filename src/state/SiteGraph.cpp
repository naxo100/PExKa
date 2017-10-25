/*
 * SiteGraph.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: naxo
 */

#include "SiteGraph.h"
#include "../pattern/Environment.h"
//#include <cstring>

namespace state {

SiteGraph::SiteGraph() : fresh(0),nodeCount(0),population(0) {
	container.reserve(1000);
}

SiteGraph::~SiteGraph() {
	// TODO Auto-generated destructor stub
}

#define MAX_CC_SIZE 10	//TODO
void SiteGraph::addComponents(unsigned n,const pattern::Mixture::Component& cc,
		const pattern::Environment& env) {
	Node* buff_nodes[MAX_CC_SIZE];
	unsigned i = 0;
	if(n < 5) {//=> n = 1
		while(n--){
			i=0;
			for(auto p_ag : cc){
				auto node = new Node(env.getSignature(p_ag->getId()));
				//node->setCount(n);
				for(auto id_site : *p_ag){
					node->setState(id_site.first,id_site.second.state);
				}
				this->allocate(node);
				buff_nodes[i] = node;
				i++;
			}
			for(auto bind : cc.getGraph())
				buff_nodes[bind.first.first]->setLink(bind.first.second,
						buff_nodes[bind.second.first],bind.second.second);
		}
	} else {
		auto multi = new MultiNode(cc.size(),n);
		for(auto p_ag : cc){
			auto node = new SubNode(env.getSignature(p_ag->getId()),*multi);
			for(auto id_site : *p_ag){
				node->setState(id_site.first,id_site.second.state);
			}
			this->allocate(node);
			buff_nodes[i] = node;
			i++;
		}
		for(auto bind : cc.getGraph())
			buff_nodes[bind.first.first]->setLink(bind.first.second,
					buff_nodes[bind.second.first],bind.second.second);
	}

}


void SiteGraph::allocate(Node* node){
	if(free.size() > 0){
		container[free.front()] = node;
		node->alloc(free.front());
		free.pop_front();
	}
	else{
		node->alloc(container.size());
		container.push_back(node);
	}
	nodeCount++;
	population += node->getCount();
}

void SiteGraph::remove(Node* node){
	free.push_back(node->getAddress());
	container[node->getAddress()] = nullptr;
	//delete node;   Do not delete, save nodes for reuse
}

size_t SiteGraph::getNodeCount() const{
	return nodeCount;
}
size_t SiteGraph::getPopulation() const {
	return population;
}

vector<Node*>::iterator SiteGraph::begin() {
	return container.begin();
}
vector<Node*>::iterator SiteGraph::end() {
	return container.end();
<<<<<<< HEAD
=======
}

/********* Node Class ************/
SiteGraph::Node::Node(const pattern::Signature& sign) : n(1),signId(sign.getId()),address(-1), intfSize(sign.getSiteCount()){
	interface = new Internal[intfSize];
	for(int i = 0; i < intfSize; i++){
		interface[i].val = sign.getSite(i).getDefaultValue();
	}
}

SiteGraph::Node::~Node() {
	delete[] interface;
}

void SiteGraph::Node::alloc(big_id addr){
	address = addr;
}

void SiteGraph::Node::setCount(pop_size q){
	n=q;
}

pop_size SiteGraph::Node::getCount() const {
	return n;
}

short_id SiteGraph::Node::getId() const{
	return signId;
}

bool SiteGraph::Node::test(const pair<small_id,pattern::Mixture::Site>& id_site,
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
void SiteGraph::Node::setState(small_id site_id,T value){
	throw std::invalid_argument("Node::setState(): not a valid state value.");
}

template <> void SiteGraph::Node::setState(small_id site_id,SomeValue value){
	interface[site_id].val = value;
}
template <> void SiteGraph::Node::setState(small_id site_id,small_id value){
	interface[site_id].val.smallVal = value;
}
template <> void SiteGraph::Node::setState(small_id site_id,float value){
	interface[site_id].val.fVal = value;
}
template <> void SiteGraph::Node::setState(small_id site_id,int value){
	interface[site_id].val.iVal = value;
}

void SiteGraph::Node::setLink(small_id site_src,Node* lnk,small_id site_trgt){
	interface[site_src].link.first = lnk;
	interface[site_src].link.second = site_trgt;
}

SiteGraph::Internal* SiteGraph::Node::begin(){
	return interface;
}
SiteGraph::Internal* SiteGraph::Node::end(){
	return interface+intfSize;
}

SiteGraph::Internal& SiteGraph::Node::getInternalState(small_id id){
	return interface[id];
}

pair<matching::InjSet*,matching::InjSet*>& SiteGraph::Node::getLifts(small_id site){
	return interface[site].deps;
}


/********** Internal struct ************/

SiteGraph::Internal::Internal() : val(small_id(-1)),link(nullptr,0),
		deps(new matching::InjSet(),new matching::InjSet()){}

SiteGraph::Internal::~Internal(){
	delete deps.first;
	delete deps.second;
}



string SiteGraph::Node::toString(const pattern::Environment &env) const {
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

string SiteGraph::Internal::toString(const pattern::Signature::Site& sit) const {
	string s(sit.getName());
	return s;
>>>>>>> refs/remotes/origin/Develop
}


} /* namespace ast */
