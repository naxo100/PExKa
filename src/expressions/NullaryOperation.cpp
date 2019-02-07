/*
 * NullaryOperation.cpp
 *
 *  Created on: Jan 15, 2019
 *      Author: lukas
 */

#include "NullaryOperation.h"
#include "../state/Variable.h"
#include <math.h>       /* sqrt, exp, log, sin, cos, tan, atan, abs */


namespace expressions {

/***********************************************/
/************ NullaryOperations ******************/
/***********************************************/


template<>
bool (*NullaryOperations<bool>::funcs[2])()= {
	[]() {return true;}, // SQRT
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









