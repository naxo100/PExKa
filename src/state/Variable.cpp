/*
 * Variable.cpp
 *
 *  Created on: Apr 29, 2016
 *      Author: naxo
 */

#include "Variable.h"

namespace state {

Variable::Variable
	(const std::string &nme,const bool is_obs):
		name(nme),isObservable(is_obs),id(-1){}

Variable::~Variable() {

}

const std::string& Variable::getName() const {return name;};

/*template <typename T>
T Variable::getValue() const {
	switch(value.t){
		case BaseExpression::FLOAT:return value.floatExp->evaluate();
		case BaseExpression::INT:return value.intExp->evaluate();
		case BaseExpression::BOOL:return value.boolExp->evaluate();
	}
	return (T)0;
}
template float Variable::getValue<float>() const;
*/


/******* class AlgebraicVar **************/
AlgebraicVar::AlgebraicVar(const std::string &nme,const bool is_obs,
		const SomeAlgExpression &exp): Variable(nme,is_obs),value(exp) {};

SomeValue AlgebraicVar::getValue(const State &state) const {
	switch(value.t){
	case BaseExpression::FLOAT:
		return SomeValue(value.floatExp->evaluate());
	case BaseExpression::INT:
		return SomeValue(value.intExp->evaluate());
	case BaseExpression::BOOL:
		return SomeValue(value.boolExp->evaluate());
	}
	return SomeValue(0);
}

/******* class KappaVar ****************/
KappaVar::KappaVar(const std::string &nme,const bool is_obs,
		const pattern::Mixture &kappa) : Variable(nme,is_obs), mixture(kappa) {}



} /* namespace state */
