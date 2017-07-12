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
template <int n> //for both
void State::negativeUpdate(SiteGraph::Internal& intf){
	auto& lifts = intf.deps;

}
template <> //for state
void State::negativeUpdate<0>(SiteGraph::Internal& intf){
	auto& lifts = intf.deps.first;
	for(auto& lift : lifts){
		if(lift.isTrashed()){
			lifts.erase(lift);
			continue;
		}
		lift;
	}
}
template <> //for links
void State::negativeUpdate<1>(SiteGraph::Internal& intf){
	auto& lifts = intf.deps.second;

}

void State::bind(const simulation::Rule::Action& a,EventInfo& ev){
	auto n1 = (get<3>(a.trgt1) ? ev.emb : ev.fresh_emb)[get<1>(a.trgt1)];
	auto n2 = (get<3>(a.trgt2) ? ev.emb : ev.fresh_emb)[get<1>(a.trgt2)];
	auto& int_state1 = n1->getInternalState(get<2>(a.trgt1));
	auto& int_state2 = n2->getInternalState(get<2>(a.trgt2));

	if(int_state1.link.first){
		negativeUpdate<1>(int_state1.link.first->getInternalState(int_state1.link.second));
		int_state1.link.first->setLink(int_state1.link.second,nullptr,0);
		ev.side_effects.emplace(int_state1.link);
	}
	if(int_state2.link.first){
		negativeUpdate<1>(int_state2.link.first->getInternalState(int_state2.link.second));
		int_state2.link.first->setLink(int_state2.link.second,nullptr,0);
		ev.side_effects.emplace(int_state2.link);
	}
	negativeUpdate<1>(int_state1);
	int_state1.link.first = n2;int_state1.link.second = get<2>(a.trgt1);
	negativeUpdate<1>(int_state2);
	int_state1.link.first = n1;int_state1.link.second = get<2>(a.trgt2);

}

void State::free(const simulation::Rule::Action& a,EventInfo& ev){
	auto& int_state = ev.emb[get<1>(a.trgt1)]->getInternalState(get<2>(a.trgt1));
	if(int_state.link.first){
		negativeUpdate<1>(int_state.link.first->getInternalState(int_state.link.second));
		int_state.link.first->setLink(int_state.link.second,nullptr,0);
		negativeUpdate<1>(int_state);
		int_state.link.first = nullptr;
	}
	else
		ev.warns++;
}

void State::modify(const simulation::Rule::Action& a,EventInfo& ev){
	auto& int_state = ev.emb[get<1>(a.trgt1)]->getInternalState(get<2>(a.trgt1));
	if(int_state.val.smallVal == get<0>(a.trgt2))
		ev.warns++;
	else {
		int_state.val.set(get<0>(a.trgt2));
		negativeUpdate<0>(int_state);
	}

}

void State::del(const simulation::Rule::Action& a,EventInfo& ev){
	//auto node = graph.getNode(ev.emb[get<1>(a.trgt1)]);
	auto node = ev.emb[get<1>(a.trgt1)];
	//delete both inj of node and pointers to node.
	for(auto site : *node){
		negativeUpdate<2>(site);
		if(site.link.first){
			negativeUpdate<1>(site.link.first->getInternalState(site.link.second));
			site.link.first = nullptr;
			ev.side_effects.emplace(site.link);
		}
	}
}


void State::apply(const simulation::Rule& r,EventInfo& ev){
	//ADD action first
	ev.fresh_emb = new SiteGraph::Node*[r.newNodes.size()];
	int i = 0;
	for(auto& n : r.newNodes){
		auto node = new SiteGraph::Node(n);
		graph.allocate(node);
		ev.fresh_emb[i] = node;
		i++;
	}
	for(auto& act : r.script){
		action[act.t](act,ev);
	}
}



void State::print() const {
	cout << "{state with SiteGraph.size() = " << graph.getNodeCount();
	cout << ", vars.size() = " << vars.size();
	cout << ", volume = " << volume.getValue().valueAs<float>();
	cout << " }" << endl;
}

} /* namespace ast */
