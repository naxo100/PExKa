/*
 * BinaryOperation.cpp
 *
 *  Created on: Oct 29, 2018
 *      Author: naxo100
 */

#include "BinaryOperation.h"
#include "../state/Variable.h"


#include <cmath>		//std::pow
#include <algorithm>	//std::max,std::min


namespace expressions {

/***********************************************/
/*********** BinaryOperations ******************/
/***********************************************/

template<typename T1, typename T2>
bool (*BinaryOperations<bool, T1, T2>::funcs[10])(T1, T2)= {
	[](T1 v1,T2 v2) {return v1 && v2;},
	[](T1 v1,T2 v2) {return v1 || v2;},
	[](T1 v1,T2 v2) {return v1 > v2;},
	[](T1 v1,T2 v2) {return v1 < v2;},
	[](T1 v1,T2 v2) {return v1 == v2;},
	[](T1 v1,T2 v2) {return v1 != v2;},
	[](T1 v1,T2 v2) {return true;},
	[](T1 v1,T2 v2) {return false;}
};
template<typename R, typename T1, typename T2>
R (*BinaryOperations<R, T1, T2>::funcs[10])(T1, T2)= {
	[](T1 v1,T2 v2) {return v1+v2;},
	[](T1 v1,T2 v2) {return v1-v2;},
	[](T1 v1,T2 v2) {return v1*v2;},
	[](T1 v1,T2 v2) {return v1/v2;},
	[](T1 v1,T2 v2) {return (R)std::pow(v1,v2);},
	[](T1 v1,T2 v2) {return (R)((int)v1%(int)v2);},
	[](T1 v1,T2 v2) {return (v1 > v2 ? v1 : v2);}, //Max
		[](T1 v1,T2 v2) {return (v1 > v2 ? v2 : v1);} //Min
	};

template<typename R, typename T1, typename T2>
R BinaryOperation<R, T1, T2>::evaluate(const VarVector &consts,
		const std::unordered_map<std::string, int> *aux_values) const {
	auto a = exp1->evaluate(consts,aux_values);
	auto b = exp2->evaluate(consts,aux_values);
	return func(a, b);
}
template<typename R, typename T1, typename T2>
R BinaryOperation<R, T1, T2>::evaluate(const state::State& state,
		const AuxMap& aux_values) const {
	auto a = exp1->evaluate(state, aux_values);
	auto b = exp2->evaluate(state, aux_values);
	return func(a, b);
}

/*template <typename R,typename T1,typename T2>
		virtual Reduction BinaryOperation<R,T1,T2>::reduce(const state::State& state,
		const AuxMap&& aux_values = AuxMap()) const {
	auto a = exp1->reduce(state,aux_values);
	auto b = exp2->reduce(state,aux_values);
	Reduction r;
	switch(op){
	case SUM:
	if(a.aux)
	if(b.aux)
	r.aux = new BinaryOperation<R,T1,T2>(a.aux,b.aux,op);
	else
	r.aux = a.aux;
	else
	if(b.aux)
	r.aux = b.aux;
	else
	r.aux = nullptr;
	r.vars = new BinaryOperation<R,T1,T2>(a.vars,b.vars,op);
	r.factor = func(a.factor, b.factor);
	break;
	case MINUS:
	break;
	case MULT:case DIV:
	if(a.aux)
	if(b.aux)
	r.aux = new BinaryOperation<R,T1,T2>(a.aux,b.aux,op);
	else
	r.aux = a.aux;
	else
	if(b.aux)
	r.aux = b.aux;
	else
	r.aux = nullptr;
	if(a.vars)
	if(b.vars)

	r.vars = new BinaryOperation<R,T1,T2>(a.vars,b.vars,op);
	r.factor = func(a.factor, b.factor);
	break;
	case POW:
	break;
	default:
	break;
	}
 }*/

template<typename R, typename T1, typename T2>
BinaryOperation<R, T1, T2>::BinaryOperation(const BaseExpression *ex1,
		const BaseExpression *ex2, const short op) :
		op(op) {
	exp1 = dynamic_cast<const AlgExpression<T1>*>(ex1);
	exp2 = dynamic_cast<const AlgExpression<T2>*>(ex2);
	func = BinaryOperations<R, T1, T2>::funcs[op];
}

template<typename R, typename T1, typename T2>
BinaryOperation<R, T1, T2>::~BinaryOperation() {
	if (!dynamic_cast<const state::Variable*>(exp1))
		delete exp1;
	if (!dynamic_cast<const state::Variable*>(exp2))
		delete exp2;
}

template<typename R, typename T1, typename T2>
FL_TYPE BinaryOperation<R, T1, T2>::auxFactors(
		std::unordered_map<std::string, FL_TYPE> &var_factors) const {
	typedef std::unordered_map<std::string, FL_TYPE> var_map;
	var_map vars1, vars2;
	FL_TYPE val1 = exp1->auxFactors(vars1);
	FL_TYPE val2 = exp2->auxFactors(vars2);
	if (op < BaseExpression::MULT) { // + or -
		for (var_map::iterator it = vars1.begin(); it != vars1.end(); it++) {
			var_factors[it->first] = func(it->second, vars2[it->first]);
			vars2.erase(it->first);
		}
		for (var_map::iterator it = vars2.begin(); it != vars2.end(); it++)
			var_factors[it->first] = func(0, it->second);
	} else if (op < BaseExpression::POW) { // * or /
		if (vars1.size() > 0)
			if (vars2.size() > 0)
				throw std::invalid_argument(
						"Only linear equations can be used for auxiliars in compartment expressions.");
			else
				for (var_map::iterator it = vars1.begin(); it != vars1.end();
						it++)
					var_factors[it->first] = func(it->second, val2);
		else
			for (var_map::iterator it = vars2.begin(); it != vars2.end(); it++)
				var_factors[it->first] = func(val1, it->second);

	}
	return func(val1, val2);
}

/*template <typename R,typename T1,typename T2>
 std::set<std::string> BinaryOperation<R,T1,T2>::getAuxiliars() const{
 auto l1 = exp1->getAuxiliars();
 auto l2 = exp2->getAuxiliars();
 l1.insert(l2.begin(),l2.end());
 return l1;
 }*/

template<typename R, typename T1, typename T2>
bool BinaryOperation<R, T1, T2>::operator==(const BaseExpression& exp) const {
	try {
		auto& binary_exp = dynamic_cast<const BinaryOperation<R, T1, T2>&>(exp);
		if (binary_exp.op == op)
			return (*binary_exp.exp1) == (*exp1)
					&& (*binary_exp.exp2) == (*exp2);
	} catch (std::bad_cast &ex) {
	}
	return false;
}

template<typename R,typename T1,typename T2>
char BinaryOperation<R,T1,T2>::getVarDeps() const{
	return exp1->getVarDeps() | exp2->getVarDeps();
}

char AlgOpChar[] = "+-*/^%Mm";
char BoolOpChar[] = "&|><=~";

template<typename R, typename T1, typename T2>
std::string BinaryOperation<R,T1,T2>::toString() const {
	return exp1->toString() + AlgOpChar[op] + exp2->toString();
}

/*template<bool, typename T1, typename T2>
std::string BinaryOperation<bool,T1,T2>::toString() const {
	return exp1->toString() + BoolOpChar[op] + exp2->toString();
}*/

template class BinaryOperation<int, int, int> ;
template class BinaryOperation<int, bool, int> ;
template class BinaryOperation<int, int, bool> ;
//template class BinaryOperation<int,int,FL_TYPE>;
//template class BinaryOperation<int,FL_TYPE,int>;
//template class BinaryOperation<int,FL_TYPE,FL_TYPE>;
//template class BinaryOperation<FL_TYPE,int,int>;
template class BinaryOperation<FL_TYPE, int, FL_TYPE> ;
template class BinaryOperation<FL_TYPE, FL_TYPE, int> ;
template class BinaryOperation<FL_TYPE, FL_TYPE, FL_TYPE> ;
template class BinaryOperation<FL_TYPE, FL_TYPE, bool> ;
template class BinaryOperation<FL_TYPE, bool, FL_TYPE> ;

template class BinaryOperation<bool, FL_TYPE, FL_TYPE> ;
template class BinaryOperation<bool, FL_TYPE, int> ;
template class BinaryOperation<bool, FL_TYPE, bool> ;
template class BinaryOperation<bool, int, FL_TYPE> ;
template class BinaryOperation<bool, int, int> ;
template class BinaryOperation<bool, int, bool> ;
template class BinaryOperation<bool, bool, FL_TYPE> ;
template class BinaryOperation<bool, bool, int> ;
template class BinaryOperation<bool, bool, bool> ;

} /* namespace expression */
