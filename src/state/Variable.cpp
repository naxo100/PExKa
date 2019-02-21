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


//const std::string& Variable::getName() const {return name;};
std::string Variable::toString() const {return name;};

/******* class AlgebraicVar **************/
template <typename T>
AlgebraicVar<T>::AlgebraicVar(const short var_id, const std::string &nme,
		const bool is_obs,const AlgExpression<T> *exp):
		BaseExpression(), Variable(var_id,nme,is_obs),expression(exp) {}
template <typename T>
T AlgebraicVar<T>::evaluate(const VarVector& consts,const unordered_map<string,int> *aux_values) const{
	return expression->evaluate(consts,aux_values);
}
template <typename T>
T AlgebraicVar<T>::evaluate(const state::State& state,const AuxMap& aux_values) const{
	return expression->evaluate(state,aux_values);
}
template <typename T>
FL_TYPE AlgebraicVar<T>::auxFactors(std::unordered_map<std::string,FL_TYPE> &aux_values) const{
	return expression->auxFactors(aux_values);
}
template <typename T>
BaseExpression::Reduction AlgebraicVar<T>::factorize() const {
	BaseExpression::Reduction r;
	r.factors.push_back(this->clone());
	return r;
}
template <typename T>
BaseExpression* AlgebraicVar<T>::clone() const {
	return new AlgebraicVar<T>(*this);
}

template <typename T>
bool AlgebraicVar<T>::operator==(const BaseExpression& exp) const {
	return *expression == exp;
}


template class AlgebraicVar<FL_TYPE>;
template class AlgebraicVar<int>;
template class AlgebraicVar<bool>;


/******* class ConstantVar *************/
template <typename T>
ConstantVar<T>::ConstantVar(const short var_id, const std::string &nme,const AlgExpression<T> *exp):
		Variable(var_id,nme),Constant<T>(exp->evaluate(VarVector(),nullptr)) {//TODO call with real VarVector
	delete exp;
}

template <typename T>
std::string ConstantVar<T>::toString() const {
	return "const("+Constant<T>::toString()+")";
}
/*
template <typename T>
T ConstantVar<T>::evaluate(const std::unordered_map<std::string,int> *aux_values) const{
	return val;
}
template <typename T>
T ConstantVar<T>::evaluate(const state::State& state,const AuxMap& aux_values) const{
	return val;
}
template <typename T>
FL_TYPE ConstantVar<T>::auxFactors(std::unordered_map<std::string,FL_TYPE> &aux_values) const{
	return val;
}*/

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

BaseExpression::Reduction KappaVar::factorize() const {
	BaseExpression::Reduction r;
	r.factors.push_back(this->clone());
	return r;
}
BaseExpression* KappaVar::clone() const {
	return new KappaVar(*this);
}

int KappaVar::evaluate(const VarVector& consts,const unordered_map<string,int> *aux_values) const {
	throw std::invalid_argument("Cannot call KappaVar::evaluate() without state.");
}
int KappaVar::evaluate(const state::State& state,const AuxMap& aux_values) const {
	return state.mixInstances(mixture);
}

const pattern::Mixture& KappaVar::getMix() const {
	return mixture;
}

bool KappaVar::operator==(const BaseExpression& exp) const {
	try{
		auto& kappa_exp = dynamic_cast<const KappaVar&>(exp);
		return kappa_exp.mixture == mixture;
	}
	catch(bad_cast &ex){	}
	return false;
}



/******* class RateVar ****************/
RateVar::RateVar(const short id,const std::string &nme,const bool is_obs,
		const AlgExpression<FL_TYPE> *exp ) :
				AlgExpression<FL_TYPE>(),
				Variable(id,nme,is_obs),
				expression(exp) {}

//TODO
FL_TYPE RateVar::auxFactors(std::unordered_map<std::string,FL_TYPE> &factor) const {
	return 0;
}

FL_TYPE RateVar::evaluate(const VarVector& consts,const unordered_map<string,int> *aux_values) const {
	throw std::invalid_argument("Cannot call RateVar::evaluate() without state.");
}
FL_TYPE RateVar::evaluate(const state::State& state,const AuxMap& aux_values) const {
	if(aux_values.size())
		return expression->evaluate(state,aux_values);
	else{
		FL_TYPE a = 1.0;
		for(auto dep : cc_deps)
			a *= state.injections[dep]->count();
		return a;
	}
}

bool RateVar::operator ==(const BaseExpression& exp) const {
	try{
		auto& rate_exp = dynamic_cast<const RateVar&>(exp);
		return *(rate_exp.expression) == *expression;
	}
	catch(bad_cast &ex){	}
	return false;
}


/**************************/
/****** class Token *******/
/**************************/

TokenVar::TokenVar(unsigned _id) :
		id(_id) {
}
FL_TYPE TokenVar::evaluate(const VarVector& consts,
		const unordered_map<string, int> *aux_values) const {
	throw std::invalid_argument("Cannot call Token::evaluate() without state.");
}
FL_TYPE TokenVar::evaluate(const state::State& state,
		const AuxMap& aux_values) const {
	return state.getTokenValue(id);
}
FL_TYPE TokenVar::auxFactors(
		std::unordered_map<std::string, FL_TYPE> &factor) const {
	throw std::invalid_argument("Cannot use tokens in this expression.");
}

bool TokenVar::operator==(const BaseExpression& exp) const {
	try {
		auto& tok_exp = dynamic_cast<const TokenVar&>(exp);
		return tok_exp.id == id;
	} catch (std::bad_cast &ex) {
	}
	return false;
}

} /* namespace state */
