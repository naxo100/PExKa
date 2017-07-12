/*
 * State.h
 *
 *  Created on: Mar 23, 2016
 *      Author: naxo
 */

#ifndef STATE_STATE_H_
#define STATE_STATE_H_

#include <vector>
#include <ctime>
#include "Variable.h"
#include "SiteGraph.h"
#include "../pattern/Mixture.h"
#include "../simulation/Counter.h"

namespace state {
struct EventInfo;

/** \brief The state of a (set of) compartment(s).
 *
 * An object of this class store all the data to define
 * the state of a set of compartments to be run by an MPI
 * process. This class contains the SiteGraph of agents
 * and all variables, rules and perturbations declared with
 * Kappa.
 */
class State {
	SiteGraph graph;
	const state::BaseExpression& volume;
	std::vector<Variable*> vars;
	float* tokens;
	//simulation::Counter counter;
	//time_t program_t0;

	template <int n>
	void negativeUpdate(SiteGraph::Internal& intf);

	inline void bind(const simulation::Rule::Action& a,EventInfo& event);
	inline void free(const simulation::Rule::Action& a,EventInfo& e);
	inline void modify(const simulation::Rule::Action& a,EventInfo& e);
	inline void del(const simulation::Rule::Action& a, EventInfo& e);
	//void add(const simulation::Rule::Action& a);

	static void (State::*action[4])(const simulation::Rule::Action&);


public:
	State(size_t tok_count,const std::vector<Variable*>& _vars,const state::BaseExpression& vol);
	~State();

	void addTokens(float n,short tok_id);
	void addNodes(unsigned n,const pattern::Mixture& mix,const pattern::Environment& env);

	//Injection or just map???
	void apply(const simulation::Rule& r,EventInfo& e);

	void print() const;
};

struct EventInfo {
	//map of emb LHS
	SiteGraph::Node** emb;
	//map of new_nodes RHS
	SiteGraph::Node** fresh_emb;
	//node_address,site_id
	std::set<pair<SiteGraph::Node*,small_id> > side_effects;
	//perturbation_ids
	std::set<mid_id> pert_ids;
	//null events
	small_id warns;

};

} /* namespace state */

#endif /* STATE_STATE_H_ */
