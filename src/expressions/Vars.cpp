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
#include "../state/Variable.h"
#include "../state/State.h"
#include "SomeValue.h"
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
BaseExpression* Auxiliar<R>::reduce(VarVector &vars) {
	return this;
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
VarLabel<R>::VarLabel(int id) :
		varId(id) {}
template<typename R>
R VarLabel<R>::evaluate(const VarVector &consts,
		const unordered_map<string, int> *aux_values) const {
	//throw std::invalid_argument("This should never been used");
	return consts[varId]->getValue(consts, aux_values).valueAs<R>();
}
template<typename R>
R VarLabel<R>::evaluate(const state::State& state,
		const AuxMap& aux_values) const {
	//throw std::invalid_argument("This should never been used");
	return state.getVarValue(varId,aux_values).valueAs<R>();//TODO this is not using auxs
}
template<typename R>
FL_TYPE VarLabel<R>::auxFactors(
		std::unordered_map<std::string, FL_TYPE> &factor) const {
	throw std::invalid_argument("VarLabel::auxFactor(): This should never been used");
}

template<typename R>
BaseExpression* VarLabel<R>::reduce(VarVector &vars ) {
	auto cons_var = dynamic_cast<state::ConstantVar<R>*>(vars[varId]);
	if(cons_var)
		return new Constant<R>(cons_var->evaluate(vars));
	return vars[varId];//this var should has been reduced
}

template <typename R>
BaseExpression::Reduction VarLabel<R>::factorize() const {
	throw invalid_argument("You should have reduced this expression before factorize! (var-name/id: "+name+"/"+to_string(varId));
}
template <typename R>
BaseExpression* VarLabel<R>::clone() const {
	return new VarLabel<R>(*this);
}

template<typename T>
bool VarLabel<T>::operator==(const BaseExpression& exp) const {
	try {
		auto& var_exp = dynamic_cast<const VarLabel<T>&>(exp);
		return var_exp.varId == varId;
	} catch (std::bad_cast &ex) {
	}
	return false;
}

template <typename T>
char VarLabel<T>::getVarDeps() const{
	return BaseExpression::VARDEP;
}

template <typename T>
std::string VarLabel<T>::toString() const{
	return name;
}

template class VarLabel<int> ;
template class VarLabel<FL_TYPE> ;
template class VarLabel<bool> ;

} /* namespace expressio */
