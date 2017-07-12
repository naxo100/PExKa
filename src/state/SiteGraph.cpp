/*
 * SiteGraph.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: naxo
 */

#include "SiteGraph.h"

namespace state {

SiteGraph::SiteGraph() : fresh(0),nodeCount(0) {
	container.reserve(1000);
}

SiteGraph::~SiteGraph() {
	// TODO Auto-generated destructor stub
}

void SiteGraph::allocate(Node* node){
	if(free.size() > 0){
		container[free.front()] = node;
		node->alloc(free.front());
		free.pop_front();
		nodeCount++;
	}
	else{
		node->alloc(container.size());
		container.push_back(node);
		nodeCount++;
	}
}

size_t SiteGraph::getNodeCount() const{
	return nodeCount;
}


/********* Node Class ************/
SiteGraph::Node::Node(const pattern::Signature& sign) : n(1),signId(sign.getId()),address(-1) {
	interface = new Internal[sign.getSiteCount()];
	for(int i = 0; i < sign.getSiteCount(); i++){
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


pair<matching::InjSet&,matching::InjSet&>& SiteGraph::Node::getLifts(small_id site){
	return interface[site].deps;
}


/********** Internal struct ************/

SiteGraph::Internal::Internal() : val(small_id(-1)),link(nullptr,0) {}

} /* namespace ast */
