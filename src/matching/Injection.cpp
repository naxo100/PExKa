/*
 * Injection.cpp
 *
 *  Created on: Apr 11, 2017
 *      Author: naxo
 */

#include "Injection.h"
#include <queue>

namespace matching {


/*Injection::~Injection() {
	// TODO Auto-generated destructor stub
}*/

bool Injection::operator<(const Injection& inj) const {
	return this->getEmbedding() < inj.getEmbedding();
}




/********** CcInjection **********/

CcInjection::CcInjection(const pattern::Mixture::Component& cc,state::SiteGraph::Node& node,
		two<std::list<state::SiteGraph::Internal*> >& port_list)
		: Injection(),address(0),coordinate(0) {
	//ccAgToNode = new big_id[cc.size()];
	ccAgToNode.reserve(cc.size());
	std::queue<pair<small_id,state::SiteGraph::Node&> > q;
	q.emplace(0,node);
	while(q.size()){
		auto& ag = cc.getAgent(q.front().first);
		if(ag.getId() != node.getId())
			throw False();
		for(auto& id_site : ag){
			if(node.test(id_site,q,port_list))
				q.back().first = cc.follow(q.front().first,q.back().first).first;
			ccAgToNode[q.front().first] = &(q.front().second);
		}
		q.pop();
	}
	//TODO check_aditional_edges???
}

const vector<Node*>& CcInjection::getEmbedding() const {
	return ccAgToNode;
}

bool CcInjection::isTrashed() const {
	return false;
}

void CcInjection::codomain(Node* injs[],set<Node*> cod) const {
	//TODO
}


} /* namespace simulation */
