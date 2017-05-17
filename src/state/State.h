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


public:
	State(size_t tok_count,const std::vector<Variable*>& _vars,const state::BaseExpression& vol);
	~State();

	void addTokens(float n,short tok_id);
	void addNodes(unsigned n,const pattern::Mixture& mix,const pattern::Environment& env);


	void print() const;
};

} /* namespace state */

#endif /* STATE_STATE_H_ */
