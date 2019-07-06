/*
 * Constant.cpp
 *
 *  Created on: Nov 12, 2018
 *      Author: naxo100
 */

#include <typeinfo>
#include "Constant.h"
#include "../state/State.h"

namespace expressions {

/******* Constant ************/
template<typename T>
Constant<T>::Constant(T v) :
		val(v) {
}

template <typename T>
Constant<T>::~Constant(){}

template<typename T>
T Constant<T>::evaluate(const VarVector &consts,
		const std::unordered_map<std::string, int> *aux_values) const {
	return val;
}
template<typename T>
T Constant<T>::evaluate(const state::State& state,
		const AuxMap& aux_values) const {
	return val;
}
//TODO
template<typename T>
FL_TYPE Constant<T>::auxFactors(
		std::unordered_map<std::string, FL_TYPE> &aux_values) const {
	return (int) val;
}

template<typename T>
BaseExpression::Reduction Constant<T>::factorize() const {
	BaseExpression::Reduction r;
	r.factors.push_back(this->clone());
	return r;
}

template <typename T>
BaseExpression* Constant<T>::reduce(VarVector& vars){
	return this;
}

template<typename T>
BaseExpression* Constant<T>::clone() const {
	return new Constant<T>(*this);
}

template<typename T>
bool Constant<T>::operator==(const BaseExpression& exp) const {
	try {
		auto& const_exp = dynamic_cast<const Constant<T>&>(exp);
		return const_exp.val == val;
	} catch (bad_cast &ex) {
	}
	return false;
}

template <typename T>
std::string Constant<T>::toString() const {
	return to_string(val);
}

//void getNeutralAuxMap(std::unordered_map<std::string, FL_TYPE>& aux_map) const{}

template class Constant<FL_TYPE> ;
template class Constant<int> ;
template class Constant<bool> ;

Constant<FL_TYPE> INF(std::numeric_limits<FL_TYPE>::infinity());
const BaseExpression* INF_EXPR = &INF;
Constant<FL_TYPE> NEG_INF(-std::numeric_limits<FL_TYPE>::infinity());
const BaseExpression* NEG_INF_EXPR = &NEG_INF;
Constant<FL_TYPE> MAX_FL(std::numeric_limits<FL_TYPE>::max());
const BaseExpression* MAX_FL_EXPR = &MAX_FL;
Constant<FL_TYPE> MIN_FL(std::numeric_limits<FL_TYPE>::min());
const BaseExpression* MIN_FL_EXPR = &MIN_FL;
Constant<FL_TYPE> MAX_INT(std::numeric_limits<int>::max());
const BaseExpression* MAX_INT_EXPR = &MAX_INT;
Constant<FL_TYPE> MIN_INT(std::numeric_limits<int>::min());
const BaseExpression* MIN_INT_EXPR = &MIN_INT;


} /* namespace expressio */
