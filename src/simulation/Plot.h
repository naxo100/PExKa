/*
 * Plot.h
 *
 *  Created on: Dec 12, 2017
 *      Author: naxo
 */

#ifndef SRC_SIMULATION_PLOT_H_
#define SRC_SIMULATION_PLOT_H_

#include <iostream>
#include <fstream>
#include "../state/State.h"
#include "Parameters.h"

namespace simulation {

using namespace std;

class Plot {
	ofstream file;
	FL_TYPE nextPoint;
	FL_TYPE dT;
public:
	Plot(const pattern::Environment& env,int run_id = 0);
	~Plot();

	void fill(const state::State& state,const pattern::Environment& env);
	void fillBefore(const state::State& state,const pattern::Environment& env);
};

} /* namespace simulation */

#endif /* SRC_SIMULATION_PLOT_H_ */
