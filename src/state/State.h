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
#include "../simulation/Rule.h"
#include "../data_structs/RandomTree.h"

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

	//data_structs::RandomTree activityTree;
	matching::InjSet* injections;
	//simulation::Counter counter;
	//time_t program_t0;

	/** \brief Negative update of injections after apply a change to an agent site.
	 * Negative update delete injections that point to sites that were changed.
	 * @param n 0 for internal value, 1 for link status, other for both.
	 * @param intf interface of the site changed.
	 */
	template <int n>
	void negativeUpdate(SiteGraph::Internal& intf);

	/** \brief Method of the actions that a rule can apply.
	 * @param act Action to apply and target agents.
	 * @param ev Embedding of nodes and other event information.
	 */
	void bind(const simulation::Rule::Action& act,EventInfo& ev);
	void free(const simulation::Rule::Action& act,EventInfo& ev);
	void modify(const simulation::Rule::Action& act,EventInfo& ev);
	void del(const simulation::Rule::Action& act, EventInfo& ev);
	//void add(const simulation::Rule::Action& a);

	/** \brief static vector with the basic rule action methods.
	 */
	static void (State::*action[4])(const simulation::Rule::Action&,EventInfo&);


public:
	/** \brief Initialize the state with the size of token vector, the vars vector and the volume.
	 * @param tok_count the size of token vector.
	 * @param _vars the variable vector.
	 * @param vol the volume of this state.
	 */
	State(size_t tok_count,const std::vector<Variable*>& _vars,const state::BaseExpression& vol);
	~State();

	/** \brief Add tokens population to the state.
	 * @param n count of tokens (could be negative).
	 * @param tok_id token id type to add.
	 */
	void addTokens(float n,short tok_id);
	/** \brief Add nodes to the SiteGraph using a fully described mixture.
	 * @param n count of copies of the mixture.
	 * @param mix a mixture without patterns to create nodes.
	 * @param env the environment of the simulation.
	 */
	void addNodes(unsigned n,const pattern::Mixture& mix,const pattern::Environment& env);

	/** \brief Apply actions described by r to the state.
	 * @param r rule to apply.
	 * @param ev embedding of nodes and other event information.
	 */
	void apply(const simulation::Rule& r,EventInfo& ev);

	void advance(double tau);

	void initializeInjections(const pattern::Environment &env);

	/** \brief Print the state for debugging purposes.
	 **/
	void print() const;
};

/** \brief Structure that stores the information related to an event.
 */
struct EventInfo {
	//map of emb LHS [cc_id][ag_id]
	SiteGraph::Node*** emb;
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
