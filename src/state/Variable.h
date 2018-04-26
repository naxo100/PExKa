/*
 * Variable.h
 *
 *  Created on: Apr 29, 2016
 *      Author: naxo
 */

#ifndef STATE_VARIABLE_H_
#define STATE_VARIABLE_H_

#include <string>

#include "AlgExpression.h"
#include "../pattern/Mixture.h"
//#include "State.h"

namespace state {

/** \brief Declared variables using '%var:' in Kappa
 *
 *
 *
 */
class Variable : public virtual BaseExpression {
	const short id;
	const std::string name;//TODO &??
	bool isObservable;
	bool updated;

public:
	Variable(const short id, const std::string &nme,const bool is_obs = false);
	virtual ~Variable() = 0;

	//template <typename T>
	//virtual SomeValue getValue(/*const State &state*/) const = 0;
	//virtual int getValue() const = 0;
	//virtual bool getValue() const = 0;
	const std::string& getName() const;

};

template <typename T>
class AlgebraicVar : public Variable, public AlgExpression<T> {
	const AlgExpression<T>* expression;
public:
	AlgebraicVar(const short var_id, const std::string &nme,const bool is_obs,
			const AlgExpression<T> *exp);

	virtual FL_TYPE auxFactors(std::unordered_map<std::string,FL_TYPE> &factor) const override;
	virtual T evaluate(const std::unordered_map<std::string,int> *aux_values = nullptr) const override;
	virtual T evaluate(const state::State& state) const override;

};

template <typename T>
class ConstantVar : public Variable, public AlgExpression<T> {
	const T val;
public:
	ConstantVar(const short var_id, const std::string &name, const AlgExpression<T> *exp);

	FL_TYPE auxFactors(std::unordered_map<std::string,FL_TYPE> &factor) const override;
	T evaluate(const std::unordered_map<std::string,int> *aux_values = nullptr) const override;
	T evaluate(const state::State& state) const override;
};

class KappaVar : public AlgExpression<int>, public Variable {
	const pattern::Mixture& mixture;
public:
	KappaVar(const short var_id, const std::string &nme,const bool is_obs,
			const pattern::Mixture &kappa);

	virtual FL_TYPE auxFactors(std::unordered_map<std::string,FL_TYPE> &factor) const override;
	virtual int evaluate(const std::unordered_map<std::string,int> *aux_values = nullptr) const override;
	virtual int evaluate(const state::State& state) const override;

	const pattern::Mixture& getMix() const;

};



} /* namespace state */

#endif /* STATE_VARIABLE_H_ */
