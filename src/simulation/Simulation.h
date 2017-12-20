/*
 * Simulation.h
 *
 *  Created on: Apr 19, 2016
 *      Author: naxo
 */

#ifndef SIMULATION_SIMULATION_H_
#define SIMULATION_SIMULATION_H_

#include <map>
#include <set>
#include <list>
#include <vector>
#include <unordered_map>
#include "Parameters.h"
#include "Plot.h"
#include "../state/State.h"
#include "../pattern/Environment.h"
#include "../pattern/RuleSet.h"
#include "../matching/Injection.h"

namespace simulation {
using namespace std;

class Simulation {
	vector<state::State> states;//vector?
	const Parameters& params;
	pattern::Environment& env;
	pattern::RuleSet rules;
	const vector<state::Variable*>& vars;
	GlobalCounter counter;
	Plot plot;

	set<matching::Injection*> *ccInjections;//[cc_env_id].at(node_id)
	set<matching::Injection*> *mixInjections;//[mix_id].at(node_id)[cc_mix_id]


	unordered_map<unsigned int,state::State> cells;

	template <typename T>
	list<T> allocParticles(unsigned cells,T count,const list<T>* vol_ratios = nullptr);

	// deterministic
	vector<unsigned> allocAgents1(unsigned cells,unsigned ag_count,const list<float>* vol_ratios = nullptr);
	// random
	vector<unsigned> allocAgents2(unsigned cells, unsigned ag_count, const list<float>* vol_ratios = nullptr);

public:
	Simulation(pattern::Environment& env,const std::vector<state::Variable*>& vars);
	~Simulation();

	void setCells(list<unsigned int>& cells);

	void initialize();
	void run(const Parameters& params);

	template <template <typename,typename...> class Range,typename... Args>
	void addTokens(const Range<int,Args...> &cell_ids,float count,short token_id);

	template <template <typename,typename...> class Range,typename... Args>
	void addAgents(const Range<int,Args...> &cell_ids,unsigned count,const pattern::Mixture& mix);

	static vector<list<unsigned int> > allocCells(int n_cpus, const vector<double> &w_vertex,
			const map<pair<int,int>,double> &w_edges, int tol);

	void print() const;

private:
	static vector<pair<pair<int,int>,double>> sortEdgesByWeidht( const map<pair<int,int>,double> &w_edges );

	static unsigned minP( vector<list<unsigned int>> P );

	static int searchCompartment( vector<list<unsigned int>> assigned, unsigned c );

};

}

#endif /* SIMULATION_SIMULATION_H_ */
