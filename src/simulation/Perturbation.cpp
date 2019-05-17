/*
 * Perturbation.cpp
 *
 *  Created on: May 2, 2019
 *      Author: naxo100
 */

#include "Perturbation.h"
#include "../state/State.h"

namespace simulation {

Perturbation::Perturbation(BaseExpression* cond,BaseExpression* unt) : condition(cond),until(unt){}

Perturbation::~Perturbation() {
	delete condition;
	delete until;
}

bool Perturbation::test(const state::State& state) const {
	return condition->getValue(state).valueAs<bool>();
}

void Perturbation::apply(state::State& state) const {
	for(auto eff : effects)
		eff->apply(state);
}




/**************** Effects *******************/

Intro::Intro(unsigned n, const Mixture& mix ) : n(n),mix(mix) {}

void Intro::apply(state::State& state) const {
	state.addNodes(n, mix);
}

Delete::Delete(unsigned n, const Mixture& mix ) : n(n),mix(mix) {
	if(mix.compsCount() != 1)
		throw invalid_argument("Perturbations can only delete unary agent patterns.");
}

void Delete::apply(state::State& state) const {
	for(int i = 0; i < n; i++)
		state.getInjContainer(mix.getId()).chooseRandom(state.getRandomGenerator());
}

} /* namespace simulation */
