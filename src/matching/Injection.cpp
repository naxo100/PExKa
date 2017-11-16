/*
 * Injection.cpp
 *
 *  Created on: Apr 11, 2017
 *      Author: naxo
 */

#include "Injection.h"
#include <queue>

namespace matching {

Injection::Injection(const pattern::Pattern& _ptrn) : ptrn(_ptrn) {}

Injection::~Injection() {}

bool Injection::operator<(const Injection& inj) const {
	return this->getEmbedding() < inj.getEmbedding();
}


const pattern::Pattern& Injection::pattern() const{
	return ptrn;
}


/********** CcInjection **********/

CcInjection::CcInjection(const pattern::Mixture::Component& _cc,Node& node,
		two<std::list<state::Node::Internal*> >& port_list,small_id root)
		: Injection(_cc),address(0) {
	//ccAgToNode = new big_id[cc.size()];
	ccAgToNode.resize(_cc.size());
	std::queue<pair<small_id,Node&> > q;
	q.emplace(0,node);
	while(q.size()){
		auto& ag = _cc.getAgent(q.front().first);
		if(ag.getId() != node.getId())
			throw False();
		for(auto& id_site : ag){
			if(node.test(id_site,q,port_list))
				q.back().first = _cc.follow(q.front().first,q.back().first).first;
			ccAgToNode[q.front().first] = &(q.front().second);
		}
		q.pop();
	}
	//TODO check_aditional_edges???
}

CcInjection::~CcInjection(){};

const vector<Node*>& CcInjection::getEmbedding() const {
	return ccAgToNode;
}

bool CcInjection::isTrashed() const {
	return false;
}

void CcInjection::codomain(Node* injs[],set<Node*> cod) const {
	int i = 0;
	for(auto node_p : ccAgToNode){
		if(cod.find(node_p) != cod.end())
			throw False();
		cod.emplace(node_p);
		injs[i] = node_p;
		i++;
	}
}

unsigned CcInjection::count() const {
	return ccAgToNode[0]->getCount();//TODO empty cc ??
}




/************ InjSet *****************/

//TODO better way to count?
unsigned InjSet::count() const {
	unsigned c = 0;
	for(auto inj : *this)
		c += inj->count();
	return c;
}

//TODO
const Injection& InjSet::chooseRandom() const{
	return **begin();
}

} /* namespace simulation */
