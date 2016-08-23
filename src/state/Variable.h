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


class Variable {
	int id;
	std::string name;
	bool isObservable;
	//SomeAlgExpression value;
public:
	Variable(const std::string &nme,const bool is_obs);
	virtual ~Variable() = 0;

	//template <typename T>
	virtual SomeValue getValue(const State &state) const = 0;
	//virtual int getValue() const = 0;
	//virtual bool getValue() const = 0;
	const std::string& getName() const;

};

class AlgebraicVar : public Variable {
	SomeAlgExpression value;

public:
	AlgebraicVar(const std::string &nme,const bool is_obs,
			const SomeAlgExpression &exp);


};

class KappaVar : public Variable {
	pattern::Mixture mixture;
public:
	KappaVar(const std::string &nme,const bool is_obs,
			const pattern::Mixture &kappa);
	SomeValue getValue() const override;
};



} /* namespace state */

#endif /* STATE_VARIABLE_H_ */
