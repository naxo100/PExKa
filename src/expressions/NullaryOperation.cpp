/*
 * NullaryOperation.cpp
 *
 *  Created on: Jan 15, 2019
 *      Author: lukas
 */

#include "NullaryOperation.h"
#include "../state/State.h"
#include "../simulation/Simulation.h"
#include <math.h>


namespace expressions {

/***********************************************/
/************ NullaryOperations ******************/
/***********************************************/


std::string n_ops[] = {"Rand(1)","TIME","EVENT","NULL_EVENT","PROD_EVENT","CPUTIME","ACTIVITY"};

template<>
FL_TYPE (*NullaryOperations<FL_TYPE>::funcs[4])(const state::State& state)= {
	[](const state::State& state) {return uniform_real_distribution<FL_TYPE>(0.0,1.0)(state.getRandomGenerator());},
	[](const state::State& state) {return state.getCounter().getTime();},
	[](const state::State& state) {return /*TODO cpu_time*/0.0;},
	[](const state::State& state) {return state.getTotalActivity();}
};

template<>
int (*NullaryOperations<int>::funcs[4])(const state::State& state)= {
	[](const state::State& state) {return int(state.getSim().getId());},
	[](const state::State& state) {return int(state.getCounter().getEvent());},
	[](const state::State& state) {return int(state.getCounter().getNullEvent());},
	[](const state::State& state) {return int(state.getCounter().getProdEvent());}
};

template<typename R>
R NullaryOperation<R>::evaluate(const VarVector &consts,
		const std::unordered_map<std::string, int> *aux_values) const {
	throw invalid_argument("Cannot cast const evaluate() on NullaryOpaertion.");
}

template<typename R>
R NullaryOperation<R>::evaluate(const state::State& state,
		const AuxMap& aux_values) const {
	return func(state);
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
BaseExpression::Reduction NullaryOperation<R>::factorize(const std::map<std::string,small_id> &aux_cc) const {
	BaseExpression::Reduction red;
	if(op == BaseExpression::RUN_ID)
		red.factor = this->clone();
	else
		throw BaseExpression::Unfactorizable("Cannot factorize: operation "+n_ops[op]+" is not factorizable.");
	return red;
}

template <typename T>
BaseExpression* NullaryOperation<T>::reduce(VarVector &vars) {
	return this;
}

template<typename R>
BaseExpression* NullaryOperation<R>::clone() const {
	return new NullaryOperation<R>(*this);
}

template<typename R>
bool NullaryOperation<R>::operator==(const BaseExpression& exp) const {
	return false;
}



template <typename R>
char NullaryOperation<R>::getVarDeps() const{
	switch(op){
	case BaseExpression::SIM_TIME:
	case BaseExpression::CPUTIME:
	case BaseExpression::ACTIVITY:
		return BaseExpression::TIME;
	case BaseExpression::NULL_EVENT:
	case BaseExpression::SIM_EVENT:
	case BaseExpression::PROD_EVENT:
		return BaseExpression::EVENT;
	case BaseExpression::RAND_1:
		return BaseExpression::RAND;
	case BaseExpression::RUN_ID:
		return BaseExpression::RUN;
	}
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
template class NullaryOperation<int> ;


} /* namespace expression */









