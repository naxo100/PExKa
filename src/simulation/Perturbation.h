/*
 * Perturbation.h
 *
 *  Created on: May 2, 2019
 *      Author: naxo100
 */

#ifndef SRC_SIMULATION_PERTURBATION_H_
#define SRC_SIMULATION_PERTURBATION_H_

#include "../pattern/mixture/Mixture.h"


namespace state {
	class State;
}

namespace simulation {

using namespace expressions;
using namespace pattern;

class Perturbation {

	class Effect {
		virtual ~Effect() = 0;
		virtual void apply(state::State &state) const = 0;
	};

	BaseExpression* condition;
	BaseExpression* until;
	list<Effect*> effects;
public:
	Perturbation(BaseExpression* cond,BaseExpression* unt);
	virtual ~Perturbation();

	bool test(const state::State& state) const;
	void apply(state::State &state) const;

	void addEffect(Effect* eff);
};



class Intro : public Perturbation::Effect {
	unsigned n;
	const pattern::Mixture& mix;

public:
	Intro(unsigned n,const pattern::Mixture& mix);

	void apply(state::State &state) const override;
};

class Delete : public Perturbation::Effect {
	unsigned n;
	const pattern::Mixture& mix;

public:
	Delete(unsigned n,const pattern::Mixture& mix);

	void apply(state::State &state) const override;
};

class Update : public Perturbation::Effect {
	unsigned varId;
	const expressions::BaseExpression* value;

public:
	Update(unsigned var_id,const expressions::BaseExpression* val);

	void apply(state::State &state) const override;
};

class UpdateToken : public Perturbation::Effect {
	unsigned varId;
	const expressions::BaseExpression* value;

public:
	UpdateToken(unsigned var_id,const expressions::BaseExpression* val);

	void apply(state::State &state) const override;
};


//class DELETE,UPDATE,UPDATE_TOK,STOP,SNAPSHOT,PRINT,PRINTF,CFLOW,CFLOW_OFF,FLUX,FLUX_OFF



} /* namespace simulation */

#endif /* SRC_SIMULATION_PERTURBATION_H_ */
