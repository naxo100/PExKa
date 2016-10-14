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
	const std::string &name;
	bool isObservable;
	bool updated;

public:
	Variable(const short id, const std::string &nme,const bool is_obs);
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

	virtual T evaluate(std::unordered_map<std::string,int> *aux_values = nullptr) const override;

};

class KappaVar : public Variable, public AlgExpression<int> {
	pattern::Mixture mixture;
public:
	KappaVar(const short var_id, const std::string &nme,const bool is_obs,
			const pattern::Mixture &kappa);
	virtual int evaluate(std::unordered_map<std::string,int> *aux_values = nullptr) const override;

};



} /* namespace state */

#endif /* STATE_VARIABLE_H_ */
