/*
 * State.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: naxo
 */

#include "State.h"
#include "../pattern/Environment.h"

namespace state {

State::State(size_t tok_count,const std::vector<Variable*>& _vars,const state::BaseExpression& vol) :
	volume(vol),vars(_vars),tokens (new float[tok_count]){}

State::~State() {
	delete[] tokens;
}


void State::addTokens(float n,short tok_id){
	tokens[tok_id] += n;
}

void State::addNodes(unsigned n,const pattern::Mixture& mix,const pattern::Environment &env){
	vector<SiteGraph::Node*> buff_nodes;
	for(auto p_comp : mix){
		buff_nodes.reserve(p_comp->size());
		size_t i = 0;
		for(auto p_ag : *p_comp){
			auto node = new SiteGraph::Node(env.getSignature(p_ag->getId()));
			node->setCount(n);
			for(auto id_site : *p_ag){
				node->setState(id_site.first,id_site.second.state);
			}
			graph.allocate(node);
			buff_nodes[i] = node;
			i++;
		}
		for(auto bind : p_comp->getGraph())
			buff_nodes[bind.first.first]->setLink(bind.first.second,
					buff_nodes[bind.second.first],bind.second.second);
	}
}

void State::print() const {
	cout << "{state with SiteGraph.size() = " << graph.getNodeCount();
	cout << ", vars.size() = " << vars.size();
	cout << ", volume = " << volume.getValue().valueAs<float>();
	cout << " }" << endl;
}

} /* namespace ast */
