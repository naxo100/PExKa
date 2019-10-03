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
Auxiliar<R>::Auxiliar(const std::string &nme,
		const tuple<int,small_id,small_id>& ccagst) :
		name(nme),cc_ag_st(ccagst) {}

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
		return aux_values.at(*this);
	} catch (std::out_of_range& ex) {
		throw std::out_of_range(
			"Cannot find value for auxiliar "+name+". Try to reorder agent-signature sites if there is dependency.");
	}
}

template <>
int Auxiliar<int>::evaluate(const state::State& state,
		const AuxMap& aux_values) const {
	throw invalid_argument("Cannot call this evaluate() on Auxiliar<int>.");
}

template<typename R>
FL_TYPE Auxiliar<R>::auxFactors(
		std::unordered_map<std::string, FL_TYPE> &var_factors) const {
	var_factors[name] = 1;
	return 0;
}

template<typename R>
BaseExpression::Reduction Auxiliar<R>::factorize(const std::map<std::string,small_id> aux_cc) const {
	BaseExpression::Reduction r;
	Auxiliar<R>* aux = new Auxiliar<R>(*this);
	r.aux_functions[aux_cc.at(aux->toString())] = aux;
	r.factor = ONE_FL_EXPR->clone();
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
		if (get<0>(aux_exp.cc_ag_st) == -1 || get<0>(cc_ag_st) == -1)
			return aux_exp.name == name;
		else
			return (aux_exp.cc_ag_st == cc_ag_st);
	} catch (std::bad_cast &ex) { }
	return false;
}

template <typename T>
void Auxiliar<T>::setAuxCoords(const std::map<std::string,std::tuple<int,small_id,small_id>>& aux_coords){
	if(get<0>(cc_ag_st) == -1)
		cc_ag_st = aux_coords.at(name);
	else{
		//TODO check if the name of the aux match with all others
	}
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
const tuple<int,small_id,small_id>& Auxiliar<T>::getCoordinates() const {
	return cc_ag_st;
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
VarLabel<R>::VarLabel(int id,const string &_name) :
		varId(id),name(_name) {}
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
BaseExpression::Reduction VarLabel<R>::factorize(const std::map<std::string,small_id> aux_cc) const {
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


template <typename T>
AuxValueMap<T>::~AuxValueMap() {}


FL_TYPE& AuxNames::operator[](const Auxiliar<FL_TYPE>& a){
	return m.unordered_map<string,FL_TYPE>::operator [](a.toString());
}
FL_TYPE& AuxNames::operator[](const string &s){
	return m.unordered_map<string,FL_TYPE>::operator [](s);
}
FL_TYPE AuxNames::at(const Auxiliar<FL_TYPE>& a) const {
	return m.unordered_map<string,FL_TYPE>::at(a.toString());
}
void AuxNames::clear() {
	m.unordered_map<string,FL_TYPE>::clear();
}
size_t AuxNames::size() const {
	return m.unordered_map<string,FL_TYPE>::size();
}



FL_TYPE& AuxCoords::operator[](const Auxiliar<FL_TYPE>& a){
	auto& coords = a.getCoordinates();
	return m.unordered_map<int,FL_TYPE>::operator [](get<1>(coords)+get<2>(coords)*sizeof(small_id));
}
FL_TYPE& AuxCoords::operator[](const two<small_id>& ag_st){
	return m.unordered_map<int,FL_TYPE>::operator [](ag_st.first+ag_st.second*sizeof(small_id));
}
FL_TYPE AuxCoords::at(const Auxiliar<FL_TYPE>& a) const {
	auto& coords = a.getCoordinates();
	return m.unordered_map<int,FL_TYPE>::at(get<1>(coords)+get<2>(coords)*sizeof(small_id));
}
void AuxCoords::clear() {
	m.unordered_map<int,FL_TYPE>::clear();
}
size_t AuxCoords::size() const {
	return m.unordered_map<int,FL_TYPE>::size();
}

template class AuxValueMap<FL_TYPE>;



} /* namespace expressio */
