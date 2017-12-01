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

SiteGraph::SiteGraph() : fresh(0),nodeCount(0),population(0),subNodeCount(0) {
	container.reserve(1000);
}

SiteGraph::~SiteGraph() {
	for(auto node : container){
		if(node)
			delete node;
	}
}

#define MAX_CC_SIZE 10	//TODO
void SiteGraph::addComponents(unsigned n,const pattern::Mixture::Component& cc,
		const pattern::Environment& env) {
	Node* buff_nodes[MAX_CC_SIZE];
	unsigned i = 0;
	if(n < 9) {//=> n = 1
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


void SiteGraph::allocate(SubNode* node){
	if(container.size()){
		container[subNodeCount]->alloc(container.size());
		container.push_back(container[subNodeCount]);
		container[subNodeCount] = node;
	}
	else{
		container.push_back(node);
	}
	node->alloc(subNodeCount);
	subNodeCount++;
}

void SiteGraph::remove(Node* node){
	free.push_back(node->getAddress());
	container[node->getAddress()] = nullptr;
	population--;// -= node->getCount();
	//delete node;   Do not delete, save nodes for reuse
}

void SiteGraph::remove(SubNode* node){
	subNodeCount--;
	//free.push_back(node->getAddress());
	//container[node->getAddress()] = nullptr;
	//population--;// -= node->getCount();
	//delete node;   Do not delete, save nodes for reuse
}

size_t SiteGraph::getNodeCount() const{
	return nodeCount;
}
size_t SiteGraph::getPopulation() const {
	return population;
}

void SiteGraph::decPopulation(size_t pop){
	population -= pop;
}

vector<Node*>::iterator SiteGraph::begin() {
	return container.begin();
}
vector<Node*>::iterator SiteGraph::end() {
	return container.end();
}

string SiteGraph::toString(const pattern::Environment& env) const {
	string ret;
	if(container.size() < 300){
		big_id i = 0;
		for(auto node : container){
			if(node){
				cout << node->getAddress() << ": ";
				cout << node->toString(env,true) << endl;
				if(i != node->getAddress())
					cout <<  "ERROR!!!!!!" << endl;
			}
			i++;
		}

	} else
		for(big_id i = 0; i < subNodeCount; i++)
			if(container[i]){
				cout << container[i]->getAddress() << ": ";
				cout << container[i]->toString(env,true) << endl;
			}
	return ret;
}


} /* namespace ast */
