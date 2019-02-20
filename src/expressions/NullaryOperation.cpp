/*
 * NullaryOperation.cpp
 *
 *  Created on: Jan 15, 2019
 *      Author: lukas
 */

#include "NullaryOperation.h"
#include "../state/Variable.h"
#include <math.h>


namespace expressions {

/***********************************************/
/************ NullaryOperations ******************/
/***********************************************/


template<>
bool (*NullaryOperations<bool>::funcs[2])()= {
	[]() {return true;},
	[]() {return false;}
};

template<>
FL_TYPE (*NullaryOperations<FL_TYPE>::funcs[1])()= {
	[]() {return (float)fmod(rand(),10)/10;}
};

template<typename R>
R NullaryOperation<R>::evaluate(const VarVector &consts,
		const std::unordered_map<std::string, int> *aux_values) const {
	return func();
}

template<typename R>
R NullaryOperation<R>::evaluate(const state::State& state,
		const AuxMap& aux_values) const {
	return func();
}

template<typename R>
NullaryOperation<R>::NullaryOperation(const short op):
		op(op) {
	func = NullaryOperations<R>::funcs[op];
}

template<typename R>
NullaryOperation<R>::~NullaryOperation() {}

template<typename R>
FL_TYPE NullaryOperation<R>::auxFactors(
		std::unordered_map<std::string, FL_TYPE> &var_factors) const {return 0.0;}

template<typename R>
BaseExpression::Reduction NullaryOperation<R>::factorize() const {
	BaseExpression::Reduction r;
	return r;
}

template<typename R>
BaseExpression* NullaryOperation<R>::clone() const {
	return new NullaryOperation<R>(*this);
}

template<typename R>
BaseExpression* NullaryOperation<R>::deleteElement(BaseExpression* exp) const {
	return this->clone();
}
template<typename R>
bool NullaryOperation<R>::operator==(const BaseExpression& exp) const {
	return false;
}

std::string n_ops[] = {"true", "false", "rand_1"};


template <typename R>
char NullaryOperation<R>::getVarDeps() const{
	return '\0';
}


template<typename R>
std::string NullaryOperation<R>::toString() const {
	return "(" + n_ops[(int)op] +")";
}

/*template<bool, typename T1, typename T2>
std::string UnaryOperation<bool,T1,T2>::toString() const {
	return exp1->toString() + BoolOpChar[op] + exp2->toString();
}*/

template class NullaryOperation<FL_TYPE> ;
//template class UnaryOperation<int, bool> ;
//template class UnaryOperation<int, FL_TYPE> ;
//
//template class UnaryOperation<FL_TYPE, int> ;
//template class UnaryOperation<FL_TYPE, bool> ;
template class NullaryOperation<bool> ;

//template class UnaryOperation<bool, FL_TYPE> ;
//template class UnaryOperation<bool, int> ;
//template class UnaryOperation<bool, bool> ;

} /* namespace expression */









