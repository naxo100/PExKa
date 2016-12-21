/*
 * Simulation.h
 *
 *  Created on: Apr 19, 2016
 *      Author: naxo
 */

#ifndef SIMULATION_SIMULATION_H_
#define SIMULATION_SIMULATION_H_

#include <map>
#include <list>
#include <vector>
#include "../state/State.h"
#include "../pattern/Environment.h"
#include "../pattern/RuleSet.h"

namespace simulation {
using namespace std;

class Simulation {
	//state::State state;//vector?
	pattern::Environment env;
	pattern::RuleSet rules;
	Counter counter;

public:
	Simulation();
	~Simulation();

	/** \brief Return a way to allocate cells among cpus.
	 *
	 * @param n_cpus Number of machines or comm_world.size()
	 * @param w_vertex Weights for every cell (total reactivity).
	 * @param w_edges Weights for every channel (sum of transport reactivity).
	 */
	vector<list<int> > allocCells(int n_cpus,const float w_vertex[],const map<pair<int,int>,float> &w_edges);

};

}

#endif /* SIMULATION_SIMULATION_H_ */
