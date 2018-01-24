/*
 * Variable.cpp
 *
 *  Created on: Apr 29, 2016
 *      Author: naxo
 */

#include "Variable.h"
#include "State.h"

namespace state {

/************ Variable *****************/

Variable::Variable
	(const short var_id, const std::string &nme,const bool is_obs):
		id(var_id),name(nme),isObservable(is_obs),updated(false){}
Variable::~Variable(){}


const std::string& Variable::getName() const {return name;};


/******* class AlgebraicVar **************/
template <typename T>
AlgebraicVar<T>::AlgebraicVar(const short var_id, const std::string &nme,
		const bool is_obs,const AlgExpression<T> *exp):
		BaseExpression(), Variable(var_id,nme,is_obs),expression(exp) {}
template <typename T>
T AlgebraicVar<T>::evaluate(const std::unordered_map<std::string,int> *aux_values) const{
	return expression->evaluate(aux_values);
}
template <typename T>
T AlgebraicVar<T>::evaluate(const state::State& state) const{
	return expression->evaluate(state);
}
template <typename T>
FL_TYPE AlgebraicVar<T>::auxFactors(std::unordered_map<std::string,FL_TYPE> &aux_values) const{
	return expression->auxFactors(aux_values);
}

template class AlgebraicVar<FL_TYPE>;
template class AlgebraicVar<int>;
template class AlgebraicVar<bool>;


/******* class ConstantVar *************/
template <typename T>
ConstantVar<T>::ConstantVar(const short var_id, const std::string &nme,const AlgExpression<T> *exp):
		BaseExpression(), Variable(var_id,nme),val(exp->evaluate(nullptr)) {
	delete exp;
}
template <typename T>
T ConstantVar<T>::evaluate(const std::unordered_map<std::string,int> *aux_values) const{
	return val;
}
template <typename T>
T ConstantVar<T>::evaluate(const state::State& state) const{
	return val;
}
template <typename T>
FL_TYPE ConstantVar<T>::auxFactors(std::unordered_map<std::string,FL_TYPE> &aux_values) const{
	return val;
}

template class ConstantVar<FL_TYPE>;
template class ConstantVar<int>;
template class ConstantVar<bool>;



/******* class KappaVar ****************/
KappaVar::KappaVar(const short id,const std::string &nme,const bool is_obs,
		const pattern::Mixture &kappa) :
				AlgExpression<int>(),
				Variable(id,nme,is_obs),
				mixture(kappa) {}

//TODO
FL_TYPE KappaVar::auxFactors(std::unordered_map<std::string,FL_TYPE> &factor) const {
	return 0;
}

int KappaVar::evaluate(const std::unordered_map<std::string,int> *aux_values) const {
	throw std::invalid_argument("Cannot call KappaVar::evaluate() without state.");
}
int KappaVar::evaluate(const state::State& state) const {
	return state.mixInstances(mixture);
}

} /* namespace state */
