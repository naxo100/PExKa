/*
 * Constant.cpp
 *
 *  Created on: Nov 12, 2018
 *      Author: naxo100
 */

#include "Constant.h"
#include "../state/State.h"

namespace expressions {

/******* Constant ************/
template<typename T>
Constant<T>::Constant(T v) :
		val(v) {
}

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
	r.constant.push_back(val);
	r.factorized_expression = new Constant<T>(*this);
	return r;
}

template<typename T>
BaseExpression* Constant<T>::clone() const {
	return new Constant<T>(*this);
}

template<typename T>
BaseExpression* Constant<T>::deleteElement(BaseExpression* exp) const{
	return this->clone();
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



} /* namespace expressio */
