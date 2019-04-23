/*
 * Vars.cpp
 *
 *  Created on: Nov 12, 2018
 *      Author: naxo100
 */

#include "Vars.h"
#include "Constant.h"
#include <iostream>
#include <typeinfo>
namespace expressions {


/*********** Class Auxiliar ***********/
template<typename R>
Auxiliar<R>::Auxiliar(const std::string &nme) :
		name(nme) {
}

template<typename R>
Auxiliar<R>::~Auxiliar() {
}

template<typename R>
R Auxiliar<R>::evaluate(const VarVector &consts,
		const unordered_map<string, int> *aux_values) const {
	try {
		if (aux_values)
			return aux_values->at(name);
		else
			throw std::out_of_range(
					"No given auxiliar map for Auxiliar::evaluate().");
	} catch (std::out_of_range &e) {
		//(*aux_values)[name] = 1;
		throw e;
	} catch (std::exception &e) {
		throw e; //TODO
	}
	return 0;
}

template<typename R>
R Auxiliar<R>::evaluate(const state::State& state,
		const AuxMap& aux_values) const {
	try {
		return aux_values.at(name);
	} catch (std::out_of_range& ex) {
		throw std::out_of_range(
			"Cannot find value for auxiliar "+name+". Try to reorder agent-signature sites if there is dependency.");
	}
}

template<typename R>
FL_TYPE Auxiliar<R>::auxFactors(
		std::unordered_map<std::string, FL_TYPE> &var_factors) const {
	var_factors[name] = 1;
	return 0;
}

template<typename R>
BaseExpression::Reduction Auxiliar<R>::factorize() const {
	BaseExpression::Reduction r;
	Auxiliar<R>* aux = new Auxiliar<R>(*this);
	r.aux_functions[aux->toString()] = aux;
	return r;
}

template<typename R>
BaseExpression* Auxiliar<R>::clone() const {
	return new Auxiliar<R>(*this);
}

template<typename R>
AlgExpression<R>* Auxiliar<R>::reduce(const state::State& state,
		const AuxMap&& aux_values) const {
	if (aux_values.count(name))
		return new Constant<R>(aux_values.at(name));
	else
		return new Auxiliar<R>(name);
}

template<typename T>
bool Auxiliar<T>::operator==(const BaseExpression& exp) const {
	try {
		auto& aux_exp = dynamic_cast<const Auxiliar<T>&>(exp);
		if (aux_exp.name == name)
			return true;
	} catch (std::bad_cast &ex) { }
	return false;
}

template <typename T>
char Auxiliar<T>::getVarDeps() const{
	return BaseExpression::AUX;
}

template <typename T>
bool Auxiliar<T>::isAux() const {
	return true;
}


template <typename T>
std::string Auxiliar<T>::toString() const {
	return name;
}

template class Auxiliar<int> ;
template class Auxiliar<FL_TYPE> ;

/********************************************/
/************** class VarLabel **************/
/********************************************/
template<typename R>
VarLabel<R>::VarLabel(BaseExpression* expr) :
		var(expr) {
}
template<typename R>
R VarLabel<R>::evaluate(const VarVector &consts,
		unordered_map<string, int> *aux_values) const {
	throw std::invalid_argument("This should never been used");
}
template<typename R>
R VarLabel<R>::evaluate(const state::State& state,
		const AuxMap& aux_values) const {
	throw std::invalid_argument("This should never been used");
}
template<typename R>
int VarLabel<R>::auxFactors(
		std::unordered_map<std::string, int> &factor) const {
	throw std::invalid_argument("This should never been used");
}

/*template<typename R>
virtual AlgExpression<R>* VarLabel<R>::reduce(const state::State& state,
		const AuxMap&& aux_values = AuxMap()) const {
	return constant<R>(state.getVarValue(id).valueAs<R>());
}*/

template<typename T>
bool VarLabel<T>::operator==(const BaseExpression& exp) const {
	try {
		auto& var_exp = dynamic_cast<const VarLabel<T>&>(exp);
		return var_exp.var == var;
	} catch (std::bad_cast &ex) {
	}
	return false;
}

template <typename T>
char VarLabel<T>::getVarDeps() const{
	return var->getVarDeps();
}

template <typename T>
std::string VarLabel<T>::toString() const{
	return var->toString();
}

} /* namespace expressio */
