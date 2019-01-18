/*
 * UnaryOperation.cpp
 *
 *  Created on: Jan 15, 2019
 *      Author: lukas
 */

#include "UnaryOperation.h"
#include "../state/Variable.h"
#include <math.h>       /* sqrt, exp, log, sin, cos, tan, atan, abs */


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
R UnaryOperation<R, T>::evaluate(
		const std::unordered_map<std::string, int> *aux_values) const {
	auto a = exp->evaluate(aux_values);
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
//	typedef std::unordered_map<std::string, FL_TYPE> var_map;
//	var_map vars1, vars2;
//	FL_TYPE val1 = exp1->auxFactors(vars1);
//	FL_TYPE val2 = exp2->auxFactors(vars2);
//	if (op < BaseExpression::MULT) { // + or -
//		for (var_map::iterator it = vars1.begin(); it != vars1.end(); it++) {
//			var_factors[it->first] = func(it->second, vars2[it->first]);
//			vars2.erase(it->first);
//		}
//		for (var_map::iterator it = vars2.begin(); it != vars2.end(); it++)
//			var_factors[it->first] = func(0, it->second);
//	} else if (op < BaseExpression::POW) { // * or /
//		if (vars1.size() > 0)
//			if (vars2.size() > 0)
//				throw std::invalid_argument(
//						"Only linear equations can be used for auxiliars in compartment expressions.");
//			else
//				for (var_map::iterator it = vars1.begin(); it != vars1.end();
//						it++)
//					var_factors[it->first] = func(it->second, val2);
//		else
//			for (var_map::iterator it = vars2.begin(); it != vars2.end(); it++)
//				var_factors[it->first] = func(val1, it->second);
//
//	}
//	return func(val1, val2);
//}

/*template <typename R,typename T1,typename T2>
 std::set<std::string> UnaryOperation<R,T1,T2>::getAuxiliars() const{
 auto l1 = exp1->getAuxiliars();
 auto l2 = exp2->getAuxiliars();
 l1.insert(l2.begin(),l2.end());
 return l1;
 }*/

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

/*template<bool, typename T1, typename T2>
std::string UnaryOperation<bool,T1,T2>::toString() const {
	return exp1->toString() + BoolOpChar[op] + exp2->toString();
}*/

template class UnaryOperation<int, int> ;
//template class UnaryOperation<int, bool> ;
//template class UnaryOperation<int, FL_TYPE> ;
//
//template class UnaryOperation<FL_TYPE, int> ;
//template class UnaryOperation<FL_TYPE, bool> ;
template class UnaryOperation<FL_TYPE, FL_TYPE> ;

//template class UnaryOperation<bool, FL_TYPE> ;
//template class UnaryOperation<bool, int> ;
//template class UnaryOperation<bool, bool> ;

} /* namespace expression */
