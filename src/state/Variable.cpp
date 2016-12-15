/*
 * Variable.cpp
 *
 *  Created on: Apr 29, 2016
 *      Author: naxo
 */

#include "Variable.h"

namespace state {

Variable::Variable
	(const short var_id, const std::string &nme,const bool is_obs):
		id(var_id),name(nme),isObservable(is_obs),updated(false){}
Variable::~Variable(){}

/*Variable::~Variable() {

}*/

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
template <typename T>
AlgebraicVar<T>::AlgebraicVar(const short var_id, const std::string &nme,
		const bool is_obs,const AlgExpression<T> *exp):
		BaseExpression(), Variable(var_id,nme,is_obs),expression(exp) {}
template <typename T>
T AlgebraicVar<T>::evaluate(const std::unordered_map<std::string,int> *aux_values) const{
	expression->evaluate(aux_values);
}
template <typename T>
float AlgebraicVar<T>::auxFactors(std::unordered_map<std::string,float> &aux_values) const{
	expression->auxFactors(aux_values);
}

template class AlgebraicVar<float>;
template class AlgebraicVar<int>;
template class AlgebraicVar<bool>;


/*SomeValue AlgebraicVar::getValue() const {
	switch(value.getType()){
	case FLOAT:
		return SomeValue(value.floatExp->evaluate());
	case INT:
		return SomeValue(value.intExp->evaluate());
	case BOOL:
		return SomeValue(value.boolExp->evaluate());
	}
	return SomeValue(0);
}*/

/******* class ConstantVar *************/
template <typename T>
ConstantVar<T>::ConstantVar(const short var_id, const std::string &nme,const AlgExpression<T> *exp):
		BaseExpression(), Variable(var_id,nme),val(exp->evaluate(nullptr)) {}
template <typename T>
T ConstantVar<T>::evaluate(const std::unordered_map<std::string,int> *aux_values) const{
	return val;
}
template <typename T>
float ConstantVar<T>::auxFactors(std::unordered_map<std::string,float> &aux_values) const{
	return val;
}

template class ConstantVar<float>;
template class ConstantVar<int>;
template class ConstantVar<bool>;



/******* class KappaVar ****************/
KappaVar::KappaVar(const short id,const std::string &nme,const bool is_obs,
		const pattern::Mixture &kappa) :
				AlgExpression<int>(),
				Variable(id,nme,is_obs),
				mixture(kappa) {}

//TODO
float KappaVar::auxFactors(std::unordered_map<std::string,float> &factor) const {
	return 0;
}
//TODO
int KappaVar::evaluate(const std::unordered_map<std::string,int> *aux_values) const {
	return 0;
}

} /* namespace state */
