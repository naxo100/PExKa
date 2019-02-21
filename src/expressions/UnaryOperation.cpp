/*
 * UnaryOperation.cpp
 *
 *  Created on: Jan 15, 2019
 *      Author: lukas
 */

#include "UnaryOperation.h"
#include "../state/Variable.h"
#include <math.h>       /* sqrt, exp, log, sin, cos, tan, atan, abs */

#include <iostream>
namespace expressions {

/***********************************************/
/************ UnaryOperations ******************/
/***********************************************/


template<typename R, typename T>
R (*UnaryOperations<R, T>::funcs[10])(T)= {
	[](T v) {return (R)std::sqrt(v);}, // SQRT
	[](T v) {return (R)std::exp(v);}, // EXPONENT
	[](T v) {return (R)std::log(v);}, // LOG
	[](T v) {return (R)std::sin(v);}, // SINE
	[](T v) {return (R)std::cos(v);}, // COSINE
	[](T v) {return (R)std::tan(v);}, // TAN
	[](T v) {return (R)std::atan(v);}, // ATAN
	[](T v) {return (R)std::abs(v);}, // ABS
	[](T v) {return ((rand()%10)/10.0) > v ? (R)1 : (R)0;}, // COIN
	[](T v) {return (R)std::fmod(std::rand(), v);} // RAND_N
};

template<typename R, typename T>
R UnaryOperation<R, T>::evaluate(const VarVector &consts,
		const std::unordered_map<std::string, int> *aux_values) const {
	auto a = exp->evaluate(consts,aux_values);
	return func(a);
}

template<typename R, typename T>
R UnaryOperation<R, T>::evaluate(const state::State& state,
		const AuxMap& aux_values) const {
	auto a = exp->evaluate(state, aux_values);
	return func(a);
}

template<typename R, typename T>
UnaryOperation<R, T>::UnaryOperation(const BaseExpression *ex,
		const short op) :
		op(op) {
	exp = dynamic_cast<const AlgExpression<T>*>(ex);
	func = UnaryOperations<R, T>::funcs[op];
}

template<typename R, typename T>
UnaryOperation<R, T>::~UnaryOperation() {
	if (!dynamic_cast<const state::Variable*>(exp))
		delete exp;
}

template<typename R, typename T>
FL_TYPE UnaryOperation<R, T>::auxFactors(
		std::unordered_map<std::string, FL_TYPE> &var_factors) const {return 0.0;}

template<typename R, typename T>
BaseExpression::Reduction UnaryOperation<R, T>::factorize() const {
	//throw std::invalid_argument("cannot factorize unary operations");
	BaseExpression::Reduction res;
	BaseExpression::Reduction r = exp->factorize();
	map<string, BaseExpression*> m = r.aux_functions;
	std::map<string, BaseExpression*>::iterator it;
	switch(op){
		case(BaseExpression::SQRT):
			for(it = m.begin(); it != m.end(); it++)
				res.aux_functions[it->first] = new UnaryOperation(it->second, BaseExpression::SQRT);
			break;
		default:
			throw std::invalid_argument("cant factorize Unary Operation");
			break;
	}
	return res;
}

template<typename R, typename T>
BaseExpression* UnaryOperation<R, T>::clone() const {
	return new UnaryOperation<R, T>(*this);
}

template<typename R, typename T>
bool UnaryOperation<R, T>::operator==(const BaseExpression& exp) const {
	try {
		auto& unary_exp = dynamic_cast<const UnaryOperation<R, T>&>(exp);
		if (unary_exp.op == op)
			return (*unary_exp.exp) == exp;
	} catch (std::bad_cast &ex) {
	}
	return false;
}

std::string ops[] = {"sqrt", "exp", "log", "sin", "cos", "tan", "atan", "abs", "coin", "rand_n"};

template<typename R, typename T>
std::string UnaryOperation<R,T>::toString() const {
	return ops[(int)op] + "(" + exp->toString() +")";
}



template class UnaryOperation<int, int> ;

template class UnaryOperation<FL_TYPE, FL_TYPE> ;

//template class UnaryOperation<bool, bool> ;

} /* namespace expression */
