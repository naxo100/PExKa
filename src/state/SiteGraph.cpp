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
}


} /* namespace ast */
