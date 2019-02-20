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

	// get the vector of factorizable expressions
	vector<BaseExpression*> fac_list = this->getFactorizableElements(r1.aux, r2.aux);

	map<string, BaseExpression*> m1 = r1.aux_functions;
	map<string, BaseExpression*> m2 = r2.aux_functions;
	std::map<string, BaseExpression*>::iterator it;
	switch(op){
		case (BaseExpression::SUM):
			if(m1.size() > 0 && m2.size() == 0){
				for(it = m1.begin(); it != m1.end(); it++)
					r.aux_functions[it->first] = new BinaryOperation(m1[it->first], exp2, BaseExpression::SUM);
			}else if(m1.size() == 0 && m2.size() > 0){
				for(it = m2.begin(); it != m2.end(); it++)
					r.aux_functions[it->first] = new BinaryOperation(exp1, m2[it->first], BaseExpression::SUM);
			}else if(m1.size() > 0 && m2.size() > 0){
				if(m1.size()==1 && m2.size()==1){
					if(m2[m1.begin()->first])
						r.aux_functions[m1.begin()->first] = new BinaryOperation(m1.begin()->second, m2[m1.begin()->first], BaseExpression::SUM);
					else
						throw std::invalid_argument("cant factorize");
				}

			}else{
				break;
			}
			break;
		case (BaseExpression::MINUS):
			if(m1.size() > 0 && m2.size() == 0){
				for(it = m1.begin(); it != m1.end(); it++)
					r.aux_functions[it->first] = new BinaryOperation(m1[it->first], exp2, BaseExpression::MINUS);
			}else if(m1.size() == 0 && m2.size() > 0){
				for(it = m2.begin(); it != m2.end(); it++)
					r.aux_functions[it->first] = new BinaryOperation(exp1, m2[it->first], BaseExpression::MINUS);
			}else if(m1.size() > 0 && m2.size() > 0){
				if(m1.size()==1 && m2.size()==1){
					if(m2[m1.begin()->first])
						r.aux_functions[m1.begin()->first] = new BinaryOperation(m1.begin()->second, m2[m1.begin()->first], BaseExpression::MINUS);
					else
						throw std::invalid_argument("cant factorize");
				}
			}else{
				break;
			}
			break;
		case (BaseExpression::MULT):
			if(m1.size() > 0 && m2.size() > 0){
				for(it = m1.begin(); it != m1.end(); it++){
					if(m2[it->first])
						r.aux_functions[it->first] = new BinaryOperation(m1[it->first], m2[it->first], BaseExpression::MULT);
					else
						r.aux_functions[it->first] = it->second;
				}
				for(it = m2.begin(); it != m2.end(); it++){
					if(!m1[it->first])
						r.aux_functions[it->first] = m2[it->first];
				}
			}else if(m1.size() > 0 && m2.size() == 0){
				r.aux_functions[m1.begin()->first] = new BinaryOperation(m1.begin()->second, exp2, BaseExpression::MULT);
			}else if(m1.size() == 0 && m2.size() > 0){
				r.aux_functions[m2.begin()->first] = new BinaryOperation(exp1, m2.begin()->second, BaseExpression::MULT);
			}else{
				break;
			}
			break;
		case (BaseExpression::DIV):
			if(m1.size() > 0 && m2.size() > 0){
				for(it = m1.begin(); it != m1.end(); it++){
					if(m2[it->first])
						r.aux_functions[it->first] = new BinaryOperation(m1[it->first], new BinaryOperation(new Constant<FL_TYPE>(1), m2[it->first], BaseExpression::DIV), BaseExpression::MULT);
					else
						r.aux_functions[it->first] = it->second;
				}
				for(it = m2.begin(); it != m2.end(); it++){
					if(!m1[it->first])
						r.aux_functions[it->first] = new BinaryOperation(new Constant<FL_TYPE>(1), m2[it->first], BaseExpression::DIV);
				}
			}else if(m1.size() > 0 && m2.size() == 0){
				r.aux_functions[m1.begin()->first] = new BinaryOperation(m1.begin()->second, new BinaryOperation(new Constant<FL_TYPE>(1), exp2, BaseExpression::DIV), BaseExpression::MULT);
			}else if(m1.size() == 0 && m2.size() > 0){
				r.aux_functions[m2.begin()->first] = new BinaryOperation(exp1, new BinaryOperation(new Constant<FL_TYPE>(1), m2.begin()->second, BaseExpression::DIV), BaseExpression::MULT);
			}else{
				break;
			}
			break;
		case (BaseExpression::POW):
			if(m1.size()>1 && m2.size()>0)
				throw std::invalid_argument("cannot factorize, multiple auxiliars ^ one or more auxiliars");
			else if(m1.size()>0 && m2.size() == 0)
				for(it = m1.begin(); it != m1.end(); it++)
					r.aux_functions[it->first] = new BinaryOperation(it->second, exp2, BaseExpression::POW);
			else if(m1.size()==0 && m2.size() > 0)
				for(it = m2.begin(); it != m2.end(); it++)
					r.aux_functions[it->first] = new BinaryOperation(exp1, it->second, BaseExpression::POW);
			else
				throw std::invalid_argument("cannot factorize expression");
			break;
		default:
			break;
	}

	// put exp1 and exp2 values in vectors
	r.constant.insert(r.constant.end(), r1.constant.begin(), r1.constant.end());
	r.constant.insert(r.constant.end(), r2.constant.begin(), r2.constant.end());

	// cannot factorize exp2 elements of a division
	if(op == BaseExpression::DIV)
		r.aux.insert(r.aux.end(), r1.aux.begin(), r1.aux.end());
	else{
		r.aux.insert(r.aux.end(), r1.aux.begin(), r1.aux.end());
		r.aux.insert(r.aux.end(), r2.aux.begin(), r2.aux.end());
	}


	r.factor_vars.insert(r.factor_vars.end(), r1.factor_vars.begin(), r1.factor_vars.end());
	r.factor_vars.insert(r.factor_vars.end(), r2.factor_vars.begin(), r2.factor_vars.end());


	BaseExpression* ex;
	if(fac_list.size() > 0){
		if((op == BaseExpression::SUM) || (op == BaseExpression::MINUS)){
			BaseExpression* ex1 = r1.factorized_expression;
			BaseExpression* ex2 = r2.factorized_expression;
			unsigned int i = 0;
			// delete the auxiliars from both expressions
			while(i < fac_list.size()){
				ex1 = ex1->deleteElement(fac_list[i]);
				ex2 = ex2->deleteElement(fac_list[i]);
				i++;
			}
			// create the factorized expression
			ex = new BinaryOperation<R, T1, T2>(ex1, ex2, op);
			// multiply the expression with the auxiliars
			for(unsigned int i=0; i< fac_list.size(); i++)
				ex = new BinaryOperation<R, T1, T2>(fac_list[i], ex, BaseExpression::MULT);
		}else{
			ex = new BinaryOperation<R, T1, T2>(r1.factorized_expression, r2.factorized_expression, op);
		}
	}else
		ex = new BinaryOperation<R, T1, T2>(r1.factorized_expression, r2.factorized_expression, op);

	r.factorized_expression = ex;
	return r;

}

template<typename R, typename T1, typename T2>
vector<BaseExpression*> BinaryOperation<R, T1, T2>::getFactorizableElements(std::vector<BaseExpression*> v1, std::vector<BaseExpression*> v2) const{
	// create vectors with the different elements of v1 and v2
	vector<string> v1_str; // elements of v1 as strings
	vector<string> v2_str; // elements of v2 as strings
	vector<string> aux1_str; // different elements of v1 as strings
	vector<BaseExpression*> aux1_exp; // different elements of v1 as expressions
	vector<BaseExpression*> res; // factorizable elements of the expression
	for(unsigned int i = 0; i < v1.size(); i++){
		v1_str.push_back(v1[i]->toString());
		if(!(std::find(aux1_str.begin(), aux1_str.end(), (v1[i]->toString())) != aux1_str.end())){
			aux1_str.push_back(v1[i]->toString());
			aux1_exp.push_back(v1[i]);
		}
	}
	for(unsigned int i = 0; i < v2.size(); i++)
		v2_str.push_back(v2[i]->toString());

	// for each different expression, add to res the ones that are present in v1 and v2
	if((op == BaseExpression::SUM) || (op == BaseExpression::MINUS)){
		for (unsigned int i = 0; i < aux1_str.size(); i++) {
			int c1 = std::count(v1_str.begin(), v1_str.end(), aux1_str[i]);
			int c2 = std::count(v2_str.begin(), v2_str.end(), aux1_str[i]);
			int j = 0;
			while(j < std::min(c1, c2)){
				res.push_back(aux1_exp[i]);
				j++;
			}
		}
	}
	return res;
}

template<typename R, typename T1, typename T2>
BaseExpression* BinaryOperation<R, T1, T2>::clone() const{
	return new BinaryOperation<R, T1, T2>(*this);
}

template<typename R, typename T1, typename T2>
BaseExpression* BinaryOperation<R, T1, T2>::deleteElement(BaseExpression* exp) const{
	// tries to delete the expression in exp1p
	BaseExpression* ex1 = exp1->deleteElement(exp);
	//if the expression changes, then the element was deleted, and returns the new expression
	if(*ex1 != *exp1)
		return new BinaryOperation<R, T1, T2>(ex1, exp2, op);

	if(op != BaseExpression::DIV){
		// if the expression is not in exp1, tries to delete it in exp2
		BaseExpression* ex2 = exp2->deleteElement(exp);
		if(*ex2 != *exp2)
			return new BinaryOperation<R, T1, T2>(exp1, ex2, op);
	}
	// if the expression is not in the BinaryOperation, then returns a clone
	return this->clone();
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
	bool par = op != BaseExpression::MULT;
	if(par)
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
