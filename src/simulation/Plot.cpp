/*
 * Plot.cpp
 *
 *  Created on: Dec 12, 2017
 *      Author: naxo
 */

#include "Plot.h"
#include "../pattern/Environment.h"

namespace simulation {

Plot::Plot(const pattern::Environment& env) : nextPoint(0.),dT(0.) {
	auto& params = Parameters::getInstance();
	try {
		file.open(params.outputFile(),ios::out);
	}
	catch(...){
		cout << "error opening output file." << endl;
	}
	dT = params.limitTime() / params.plotPoints();
	file << "#Time";
	for(auto obs : env.getObservables())
		file << "\t" << obs->getName();
	file << endl;
}

Plot::~Plot() {
	file.close();
}


void Plot::fill(const state::State& state,const pattern::Environment& env) {
	auto t = state.getCounter().getTime();
	while(t > nextPoint){
		file << nextPoint;
		for(auto var : env.getObservables()){
			file << "\t" << var->getValue(state).valueAs<int>();
		}
		file << endl;
		nextPoint += dT;
	}
}


} /* namespace simulation */
