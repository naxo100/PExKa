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
#include <unordered_map>
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

	unordered_map<int,state::State> cells;

	template <typename T>
	list<T> allocParticles(unsigned cells,T count,const list<T>* vol_ratios = nullptr);

public:
	Simulation();
	~Simulation();

	template <template <typename,typename...> class Range,typename... Args>
	void addTokens(const Range<int,Args...> &cell_ids,float count,short token_id);

	template <template <typename,typename...> class Range,typename... Args>
	void addAgents(const Range<int,Args...> &cell_ids,unsigned count,const pattern::Mixture& mix);



	/** \brief Return a way to allocate cells among cpu's.
	 *
	 * @param n_cpus Number of machines or comm_world.size()
	 * @param w_vertex Weights for every cell (total reactivity).
	 * @param w_edges Weights for every channel (sum of transport reactivity).
	 * @param tol Tolerance in the number of processors to be assigned
	 * @return a vector with the compartments indexed by ID processor
	 */
	vector<list<int> > allocCells(int n_cpus, const vector<double> &w_vertex, const map<pair<int,int>,double> &w_edges, int tol);

private:
	/** \brief Sort edges by weight from lowest to highest
	 *  @param w_edges edges with weight
	 */
	vector<pair<pair<int,int>,double>> sortEdgesByWeidht( const map<pair<int,int>,double> &w_edges );

	unsigned minP( vector<list<int>> P );

	int searchCompartment( vector<list<int>> assigned, unsigned c );

};

}

#endif /* SIMULATION_SIMULATION_H_ */
