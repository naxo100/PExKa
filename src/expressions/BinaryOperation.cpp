/*
 * BinaryOperation.cpp
 *
 *  Created on: Oct 29, 2018
 *      Author: naxo100
 */

#include "BinaryOperation.h"
#include "Constant.h"
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


char AlgOpChar[] = "+-*/^%Mm";
string AlgOpStr[] = {"add","subs","mult","div","pow","mod","max","min"};
char BoolOpChar[] = "&|><=~";

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
BinaryOperation<R, T1, T2>::BinaryOperation(BaseExpression *ex1,
		BaseExpression *ex2, Op_Type op) :
		op(op) {
	exp1 = dynamic_cast<AlgExpression<T1>*>(ex1);
	exp2 = dynamic_cast<AlgExpression<T2>*>(ex2);
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
BaseExpression::Reduction BinaryOperation<R, T1, T2>::factorize(const std::map<std::string,small_id> &aux_cc) const {
	using Unfactorizable = BaseExpression::Unfactorizable;
	auto VARDEP = BaseExpression::VarDep::VARDEP;
	auto MULT = BaseExpression::AlgebraicOp::MULT;
	auto DIV = BaseExpression::AlgebraicOp::DIV;
	auto make_binary = BaseExpression::makeBinaryExpression<false>;

	if(std::is_same<R,bool>::value)
		throw BaseExpression::Unfactorizable("Cannot factorize: boolean binary operations cannot be factorized.");

	auto op = BaseExpression::AlgebraicOp(this->op);//to avoid problems with op been a BoolOp

	BaseExpression::Reduction r;
	BaseExpression::Reduction r1(exp1->factorize(aux_cc));
	BaseExpression::Reduction r2(exp2->factorize(aux_cc));

	// create the auxiliar functions from the current operation with
	// the functions of the expressions (exp1 and exp2)
	auto& auxf_1 = r1.aux_functions;
	auto& auxf_2 = r2.aux_functions;
	std::map<string, BaseExpression*>::iterator it;
	switch(op){
	case BaseExpression::SUM:
	case BaseExpression::MINUS:
		if(auxf_1.size()){
			auto& cc_f_1 = *auxf_1.begin();
			if(auxf_2.size()){//sum of cc-aux funcs
				if(auxf_1.size() == 1 && auxf_2.size() == 1){//only one cc-index
					auto& cc_f_2 = *auxf_2.begin();
					if(cc_f_1.first == cc_f_2.first){//sum of same cc-aux funcs index
						if((r1.factor->getVarDeps() | r2.factor->getVarDeps()) & VARDEP)
							ADD_WARN_NOLOC("Rate factorization will depend on "+AlgOpStr[op]+"(aux,var) and this will lead to inexact results.");
						if(*r1.factor == *ONE_FL_EXPR){
							delete r1.factor;r1.factor = nullptr;
						}
						if(*r2.factor == *ONE_FL_EXPR){
							delete r2.factor;r2.factor = nullptr;
						}
						r.factor = ONE_FL_EXPR->clone();
						r.aux_functions[cc_f_1.first] = make_binary(
								r1.factor ? make_binary(r1.factor,cc_f_1.second,MULT) : cc_f_1.second,
								r2.factor ? make_binary(r2.factor,cc_f_2.second,MULT) : cc_f_2.second,
								op);
					}
					else throw Unfactorizable("Cannot factorize: add/sub of different cc-aux-functions.");
				}
				else throw Unfactorizable("Cannot factorize: add/sub of more than one cc-aux-functions.");
			}
			else{//cc-aux func plus const
				if(auxf_1.size() == 1){//only one cc-index
					if((r1.factor->getVarDeps() | r2.factor->getVarDeps()) & VARDEP)
						ADD_WARN_NOLOC("Rate factorization will depend on "+AlgOpStr[op]+"(aux,var) and this will lead to inexact results(2).");
					r.factor = ONE_FL_EXPR->clone();
					r.aux_functions[cc_f_1.first] = make_binary(make_binary(r1.factor,cc_f_1.second,MULT),
							r2.factor,op);
				}
				else throw Unfactorizable("Cannot factorize: add/sub a const to more than one cc-aux-functions.");
			}
		}
		else if(auxf_2.size()){//const plus cc-aux func
			if(auxf_2.size() == 1){//only one cc-index
				if((r1.factor->getVarDeps() | r2.factor->getVarDeps()) & VARDEP)
					ADD_WARN_NOLOC("Rate factorization will depend on "+AlgOpStr[op]+"(aux,var) and this will lead to inexact results.");
				auto& cc_f_2 = *auxf_2.begin();
				r.factor = ONE_FL_EXPR->clone();
				r.aux_functions[cc_f_2.first] = make_binary(r1.factor,
						make_binary(r2.factor,cc_f_2.second,MULT),op);
			}
			else throw Unfactorizable("Cannot factorize: add/sub a const to more than one cc-aux-functions.");
		}
		else//sum of constants (no aux)
			r.factor = make_binary(r1.factor,r2.factor,op);
		break;

	case (BaseExpression::DIV):
	case (BaseExpression::MULT):
		//setting factor, excluding factor 1x
		if(*r1.factor != *ONE_FL_EXPR)
			if(*r2.factor != *ONE_FL_EXPR)
				r.factor = make_binary(r1.factor,r2.factor,op);
			else{
				r.factor = r1.factor;
				delete r2.factor;
			}
		else{
			r.factor = op == DIV? make_binary(ONE_FL_EXPR->clone(),r2.factor,op) : r2.factor;
			delete r1.factor;
		}
		r.aux_functions = auxf_1;//aux_funcs = r1
		for(auto aux_f : auxf_2)
			if(auxf_1.count(aux_f.first))//aux_func = r1*/r2
				r.aux_functions[aux_f.first] = make_binary(
						auxf_1[aux_f.first],aux_f.second,op);
			else
				r.aux_functions[aux_f.first] = op == DIV ?//aux_func = 1*/r2
						make_binary(ONE_FL_EXPR->clone(),aux_f.second,op) :
						aux_f.second;
		break;
	case (BaseExpression::POW):
		if(auxf_2.size()){
			if(auxf_2.size() > 1)
				throw Unfactorizable("Cannot factorize: pow exponent of more than one cc_aux function.");
			auto& cc_f_2 = *auxf_2.begin();
			if(auxf_1.size() ){
				if(auxf_1.size() == 1){//only one cc-index
					auto& cc_f_1 = *auxf_1.begin();
					if(cc_f_1.first == cc_f_2.first){//pow of same cc-aux funcs index
						if((r1.factor->getVarDeps() | r2.factor->getVarDeps()) & VARDEP)
							ADD_WARN_NOLOC("Rate factorization will depend on "+AlgOpStr[op]+"(aux,var) and this will lead to inexact results.");
						r.factor = ONE_FL_EXPR->clone();
						r.aux_functions[cc_f_1.first] = make_binary(
								make_binary(r1.factor,cc_f_1.second,MULT),
								make_binary(r2.factor,cc_f_2.second,MULT),
								op);
					}
					else throw Unfactorizable("Cannot factorize: val powered to a different cc-aux-function.");
				}
				else throw Unfactorizable("Cannot factorize: more than one cc-aux functions powered to one aux-function.");
			}
			else{//no aux_f powered to 1 aux_f
				if(*r1.factor == *ONE_FL_EXPR)
					throw Unfactorizable("Cannot factorize: 1 powered to some aux function has no sense.");
				if((r1.factor->getVarDeps() | r2.factor->getVarDeps()) & VARDEP)
					ADD_WARN_NOLOC("Rate factorization will depend on "+AlgOpStr[op]+"(aux,var) and this will lead to inexact results.");
				r.factor = ONE_FL_EXPR->clone();
				r.aux_functions[cc_f_2.first] = make_binary(r1.factor,
						make_binary(r2.factor,cc_f_2.second,MULT),op);
			}
		}
		else{//some aux_f powered to factor
			if((r2.factor->getVarDeps() & VARDEP) && auxf_1.size())
				ADD_WARN_NOLOC("Rate factorization will depend on (aux^var) and this will lead to inexact results.");
			if(*r1.factor == *ONE_FL_EXPR)
				r.factor = r1.factor;
			else
				r.factor = make_binary(r1.factor,r2.factor->clone(),op);
			for(auto aux_f : auxf_1)
				r.aux_functions[aux_f.first] = make_binary(aux_f.second,r2.factor->clone(),op);
			delete r2.factor;
		}
		break;
	//default://never called
	//	delete r1.factor;
	//	delete r2.factor;
	//	break;
	}

	return r;
}


template <typename R, typename T1, typename T2>
BaseExpression* BinaryOperation<R,T1,T2>::reduce(VarVector& vars){
	auto r1 = exp1->reduce(vars);
	auto r2 = exp2->reduce(vars);
	auto cons_r1 = dynamic_cast<Constant<T1>*>(r1);
	auto cons_r2 = dynamic_cast<Constant<T2>*>(r2);
	if(cons_r1){
		if(cons_r2){
			T1 val1 = cons_r1->evaluate(vars);
			T2 val2 = cons_r2->evaluate(vars);
			if(r1 != exp1)
				delete r1;
			if(r2 != exp2)
				delete r2;
			return new Constant<R>(func(val1,val2));
		}
		//TODO delete sum of 0 and mult of 1
	}
	if(r1 != exp1)
		delete exp1;
	if(r2 != exp2)
		delete exp2;
	exp1 = dynamic_cast<AlgExpression<T1>*>(r1);
	exp2 = dynamic_cast<AlgExpression<T2>*>(r2);
	return this;
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

template <typename R,typename T1, typename T2>
void BinaryOperation<R,T1,T2>::setAuxCoords(const std::map<std::string,std::tuple<int,small_id,small_id>>& aux_coords){
	exp1->setAuxCoords(aux_coords);
	exp2->setAuxCoords(aux_coords);
}



template<typename R,typename T1,typename T2>
char BinaryOperation<R,T1,T2>::getVarDeps() const{
	return exp1->getVarDeps() | exp2->getVarDeps();
}


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
