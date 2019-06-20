/*
 * Perturbation.h
 *
 *  Created on: May 2, 2019
 *      Author: naxo100
 */

#ifndef SRC_SIMULATION_PERTURBATION_H_
#define SRC_SIMULATION_PERTURBATION_H_

#include "../pattern/mixture/Mixture.h"
#include "Rule.h"


namespace state {
	class State;
}

namespace simulation {

using namespace expressions;
using namespace pattern;

class Perturbation {
public:
	class Effect {
	public:
		virtual ~Effect();
		virtual void apply(state::State &state) const = 0;
		virtual int addInfluences(int current,Rule::CandidateMap& map,const Environment &env) const;
	};
	friend class pattern::Environment;
protected:
	int id;
	BaseExpression* condition;
	BaseExpression* until;
	list<Effect*> effects;
	Rule::CandidateMap influence;
	int introCount;
	float nextStop;
	float incStep;

	mutable int applies;
	bool isCopy;

	void setId(int _id);
public:
	Perturbation(BaseExpression* cond,BaseExpression* unt,const yy::location& loc);
	Perturbation(const Perturbation& pert);
	~Perturbation();

	int getId() const;

	bool test(const state::State& state) const;
	FL_TYPE timeTest(const state::State& state) const;
	bool testAbort(const state::State &state,bool just_applied);
	void apply(state::State &state) const;

	void addEffect(Effect* eff,const Environment& env);

	float nextStopTime() const;

	string toString(const state::State& state) const;
};



class Intro : public Perturbation::Effect {
	const BaseExpression* n;
	const pattern::Mixture* mix;

public:
	Intro(const BaseExpression* n,const pattern::Mixture* mix);
	~Intro();

	void apply(state::State &state) const override;
	int addInfluences(int current,Rule::CandidateMap& map,const Environment &env) const override;
};

class Delete : public Perturbation::Effect {
	const BaseExpression* n;
	const pattern::Mixture& mix;

public:
	Delete(const BaseExpression* n,const pattern::Mixture& mix);
	~Delete();

	void apply(state::State &state) const override;
};

class Update : public Perturbation::Effect {
	state::Variable* var;

public:
	Update(const state::Variable& _var,expressions::BaseExpression* expr);
	//Update(unsigned var_id,pattern::Mixture* _var);
	~Update();

	void apply(state::State &state) const override;
};

class UpdateToken : public Perturbation::Effect {
	unsigned varId;
	const expressions::BaseExpression* value;

public:
	UpdateToken(unsigned var_id,expressions::BaseExpression* val);
	~UpdateToken();

	void apply(state::State &state) const override;
};


//class DELETE,UPDATE,UPDATE_TOK,STOP,SNAPSHOT,PRINT,PRINTF,CFLOW,CFLOW_OFF,FLUX,FLUX_OFF



} /* namespace simulation */

#endif /* SRC_SIMULATION_PERTURBATION_H_ */
