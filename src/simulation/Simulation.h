/*
 * Simulation.h
 *
 *  Created on: Apr 19, 2016
 *      Author: naxo
 */

#ifndef SIMULATION_SIMULATION_H_
#define SIMULATION_SIMULATION_H_


class Simulation {
	state::State state;//vector?
	pattern::Environment env;
	pattern::RuleSet rules;
	simulation::Counter counter;

public:
	Simulation();
	~Simulation();
};


#endif /* SIMULATION_SIMULATION_H_ */
