/*
 * BinaryOperation.cpp
 *
 *  Created on: Oct 29, 2018
 *      Author: naxo100
 */

#include "BinaryOperation.h"
#include "../state/Variable.h"
#include <iostream>
#include "Vars.h"
#include "UnaryOperation.h"
#include <cmath>		//std::pow
#include <algorithm>	//std::max,std::min
#include <typeinfo>		//bad_cast


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
BaseExpression::Reduction BinaryOperation<R, T1, T2>::factorize() const {
	BaseExpression::Reduction r;
	BaseExpression::Reduction r1 = exp1->factorize();
	BaseExpression::Reduction r2 = exp2->factorize();

	// create the auxiliar functions from the current operation with
	// the functions of the expressions (exp1 and exp2)
	map<string, BaseExpression*> m1 = r1.aux_functions;
	map<string, BaseExpression*> m2 = r2.aux_functions;
	std::map<string, BaseExpression*>::iterator it;
	switch(op){
		case (BaseExpression::SUM):
			if(m1.size() > 0 && m2.size() == 0){ // f(aux1)*.....*g(auxn) + constant|variable)
				if(m1.size() == 1){
					BaseExpression* ex = m1.begin()->second;
					for(auto fac : r1.factors)
						ex = BaseExpression::makeBinaryExpression<false>(fac, ex, BaseExpression::MULT);
					r.aux_functions[m1.begin()->first] = new BinaryOperation(ex, exp2, BaseExpression::SUM);
				}
				else
					throw std::invalid_argument("cant factorize expression (f(aux1)*.....*g(auxn) + constant|variable)");
			}else if(m1.size() == 0 && m2.size() > 0){ // constant|variable + f(aux1)*.....*g(auxn)
				if(m2.size() == 1){
					BaseExpression* ex = m2.begin()->second;
					for(auto fac : r2.factors)
						ex = BaseExpression::makeBinaryExpression<false>(fac, ex, BaseExpression::MULT);
					r.aux_functions[m2.begin()->first] = new BinaryOperation(exp1, ex, BaseExpression::SUM);
				}
				else
					throw std::invalid_argument("cant factorize expression (constant|variable + f(aux1)*.....*g(auxn))");
			}else if(m1.size() > 0 && m2.size() > 0){ // f(aux1)*.....*g(auxn) + f'(aux1')*.....*g'(auxn')
				if(m1.size()==1 && m2.size()==1){ // f(aux1) + f'(aux1')
					BaseExpression* ex1 = m1.begin()->second;
					BaseExpression* ex2 = m2[m1.begin()->first];
					if(m2[m1.begin()->first]){ // aux1 = aux1' => f(aux1) + f'(aux1') = g(aux1)
						for(auto fac : r1.factors)
							ex1 = BaseExpression::makeBinaryExpression<false>(fac, ex1, BaseExpression::MULT);
						for(auto fac : r2.factors)
							ex2 = BaseExpression::makeBinaryExpression<false>(fac, ex2, BaseExpression::MULT);
						r.aux_functions[m1.begin()->first] = new BinaryOperation(ex1, ex2, BaseExpression::SUM);
					}
					else
						throw std::invalid_argument("cant factorize expression (f(aux1)*.....*g(auxn) + f'(aux1')*.....*g'(auxn'))");
				}
			}else{
				break;
			}
			break;
		case (BaseExpression::MINUS):
			if(m1.size() > 0 && m2.size() == 0){ // f(aux1)*.....*g(auxn) - constant|variable)
				if(m1.size() == 1){
					BaseExpression* ex = m1.begin()->second;
					for(auto fac : r1.factors)
						ex = BaseExpression::makeBinaryExpression<false>(fac, ex, BaseExpression::MULT);
					r.aux_functions[m1.begin()->first] = new BinaryOperation(ex, exp2, BaseExpression::MINUS);
				}
				else
					throw std::invalid_argument("cant factorize expression (f(aux1)*.....*g(auxn) - constant|variable)");
			}else if(m1.size() == 0 && m2.size() > 0){ // constant|variable - f(aux1)*.....*g(auxn)
				if(m2.size() == 1){
					BaseExpression* ex = m2.begin()->second;
					for(auto fac : r2.factors)
						ex = BaseExpression::makeBinaryExpression<false>(fac, ex, BaseExpression::MULT);
					r.aux_functions[m2.begin()->first] = new BinaryOperation(exp1, ex, BaseExpression::MINUS);
				}
				else
					throw std::invalid_argument("cant factorize expression (constant|variable - f(aux1)*.....*g(auxn))");
			}else if(m1.size() > 0 && m2.size() > 0){ // f(aux1)*.....*g(auxn) - f'(aux1')*.....*g'(auxn')
				if(m1.size()==1 && m2.size()==1){ // f(aux1) - f'(aux1')
					BaseExpression* ex1 = m1.begin()->second;
					BaseExpression* ex2 = m2[m1.begin()->first];
					if(m2[m1.begin()->first]){ // aux1 = aux1' => f(aux1) + f'(aux1') = g(aux1)
						for(auto fac : r1.factors)
							ex1 = BaseExpression::makeBinaryExpression<false>(fac, ex1, BaseExpression::MULT);
						for(auto fac : r2.factors)
							ex2 = BaseExpression::makeBinaryExpression<false>(fac, ex2, BaseExpression::MULT);
						r.aux_functions[m1.begin()->first] = new BinaryOperation(ex1, ex2, BaseExpression::MINUS);
					}
					else
						throw std::invalid_argument("cant factorize expression (f(aux1)*.....*g(auxn) + f'(aux1')*.....*g'(auxn'))");
				}
			}else{
				break;
			}
			break;
		case (BaseExpression::MULT):
			// add the constants and variables to the response factors
			r.factors.insert(r.factors.end(), r1.factors.begin(), r1.factors.end());
			r.factors.insert(r.factors.end(), r2.factors.begin(), r2.factors.end());
			if(m1.size() > 0 && m2.size() > 0){ // f(aux1)*.....*g(auxn) * f'(aux1')*.....*g'(auxn')
				for(it = m1.begin(); it != m1.end(); it++){
					if(m2[it->first]) // add the auxiliars that are in m1 and m2
						r.aux_functions[it->first] = new BinaryOperation(m1[it->first], m2[it->first], BaseExpression::MULT);
					else // add the auxiliars that are in m1 but not in m2
						r.aux_functions[it->first] = it->second;
				}
				for(it = m2.begin(); it != m2.end(); it++){
					if(!m1[it->first]) // add the auxiliars that are in m2 but not in m1
						r.aux_functions[it->first] = it->second;
				}
			}else if(m1.size() > 0 && m2.size() == 0){ // f(aux1)*.....*g(auxn) * constant|variable)
				r.aux_functions[m1.begin()->first] = m1.begin()->second;
			}else if(m1.size() == 0 && m2.size() > 0){ // constant|variable) * f(aux1)*.....*g(auxn)
				r.aux_functions[m2.begin()->first] = m2.begin()->second;
			}else{
				break;
			}
			break;
		case (BaseExpression::DIV):
			// add the constants and variables to the response factors
			r.factors.insert(r.factors.end(), r1.factors.begin(), r1.factors.end());
			if(op == BaseExpression::DIV){
				for(unsigned int i = 0; i < r2.factors.size(); i++)
					r.factors.push_back(BaseExpression::makeBinaryExpression<false>(new Constant<FL_TYPE>(1), r2.factors[i], op));
			}else
				r.factors.insert(r.factors.end(), r2.factors.begin(), r2.factors.end());

			if(m1.size() > 0 && m2.size() > 0){ // f(aux1)*.....*g(auxn) / f'(aux1')*.....*g'(auxn')
				for(it = m1.begin(); it != m1.end(); it++){
					if(m2[it->first]) // add the auxiliars that are in m1 and m2, convert division to * 1/m2
						r.aux_functions[it->first] = new BinaryOperation(m1[it->first], new BinaryOperation(new Constant<FL_TYPE>(1), m2[it->first], BaseExpression::DIV), BaseExpression::MULT);
					else // add the auxiliars that are in m1 but not in m2
						r.aux_functions[it->first] = it->second;
				}
				for(it = m2.begin(); it != m2.end(); it++){
					if(!m1[it->first]) // add the auxiliars that are in m2 but not in m1 (1/m2)
						r.aux_functions[it->first] = new BinaryOperation(new Constant<FL_TYPE>(1), m2[it->first], BaseExpression::DIV);
				}
			}else if(m1.size() > 0 && m2.size() == 0){ // f(aux1)*.....*g(auxn) / constant|variable)
				r.aux_functions[m1.begin()->first] = m1.begin()->second;
			}else if(m1.size() == 0 && m2.size() > 0){ // constant|variable) / f(aux1)*.....*g(auxn)
				r.aux_functions[m2.begin()->first] = new BinaryOperation(new Constant<FL_TYPE>(1), m2.begin()->second, BaseExpression::DIV);
			}else{
				break;
			}
			break;
		case (BaseExpression::POW):
			if(m1.size()>0 && m2.size()>0) // f(aux1)*.....*g(auxn) ^ f'(aux1')*.....*g'(auxn')
				throw std::invalid_argument("cannot factorize, one or more auxiliars ^ one or more auxiliars");
			else if(m1.size()>0 && m2.size() == 0) // f(aux1)*.....*g(auxn) ^ constant|variable
				for(it = m1.begin(); it != m1.end(); it++)
					r.aux_functions[it->first] = new BinaryOperation(it->second, exp2, BaseExpression::POW);
			else if(m1.size()==0 && m2.size() > 0){ // constant|variable ^ f(aux1)*.....*g(auxn)
				if(m2.size() == 1)
					r.aux_functions[m2.begin()->first] = new BinaryOperation(exp1, m2.begin()->second, BaseExpression::POW);
				else
					throw std::invalid_argument("cannot factorize expression constant|variable ^ multiple auxiliars");
			}else
				break;
			break;
		default:
			break;
	}

	return r;

//	vector<BaseExpression*> fac_list = this->getFactorizableElements(r1.aux, r2.aux);
//
//	// put exp1 and exp2 values in vectors
//	r.constant.insert(r.constant.end(), r1.constant.begin(), r1.constant.end());
//	r.constant.insert(r.constant.end(), r2.constant.begin(), r2.constant.end());
//
//	// cannot factorize exp2 elements of a division
//	if(op == BaseExpression::DIV)
//		r.aux.insert(r.aux.end(), r1.aux.begin(), r1.aux.end());
//	else{
//		r.aux.insert(r.aux.end(), r1.aux.begin(), r1.aux.end());
//		r.aux.insert(r.aux.end(), r2.aux.begin(), r2.aux.end());
//	}
//
//
//	r.factor_vars.insert(r.factor_vars.end(), r1.factor_vars.begin(), r1.factor_vars.end());
//	r.factor_vars.insert(r.factor_vars.end(), r2.factor_vars.begin(), r2.factor_vars.end());
//
//
//	BaseExpression* ex;
//	if(fac_list.size() > 0){
//		if((op == BaseExpression::SUM) || (op == BaseExpression::MINUS)){
//			BaseExpression* ex1 = r1.factorized_expression;
//			BaseExpression* ex2 = r2.factorized_expression;
//			unsigned int i = 0;
//			// delete the auxiliars from both expressions
//			while(i < fac_list.size()){
//				ex1 = ex1->deleteElement(fac_list[i]);
//				ex2 = ex2->deleteElement(fac_list[i]);
//				i++;
//			}
//			// create the factorized expression
//			ex = new BinaryOperation<R, T1, T2>(ex1, ex2, op);
//			// multiply the expression with the auxiliars
//			for(unsigned int i=0; i< fac_list.size(); i++)
//				ex = new BinaryOperation<R, T1, T2>(fac_list[i], ex, BaseExpression::MULT);
//		}else{
//			ex = new BinaryOperation<R, T1, T2>(r1.factorized_expression, r2.factorized_expression, op);
//		}
//	}else
//		ex = new BinaryOperation<R, T1, T2>(r1.factorized_expression, r2.factorized_expression, op);
//
//	r.factorized_expression = ex;

}

template<typename R, typename T1, typename T2>
BaseExpression* BinaryOperation<R, T1, T2>::clone() const{
	return new BinaryOperation<R, T1, T2>(exp1->clone(),exp2->clone(),op);
}

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
	//bool par = op != BaseExpression::MULT;
	if(op < BaseExpression::MULT)
		return "(" + exp1->toString() + AlgOpChar[op] + exp2->toString() + ")";
	else
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
