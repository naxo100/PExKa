/*
 * AlgExpression.cpp
 *
 *  Created on: Jul 22, 2016
 *      Author: naxo
 */

#include "AlgExpression.h"

#include <cmath>		//std::pow
#include <algorithm>	//std::max,std::min

namespace state {

template <> struct BaseExpression::TypeDef<BaseExpression::INT> { typedef int t;};
template <> struct BaseExpression::TypeDef<BaseExpression::BOOL> { typedef bool t;};
template <> struct BaseExpression::EnumType<int> {static const Type t = INT;};
template <> struct BaseExpression::EnumType<bool> {static const Type t = BOOL;};

const BaseExpression::Type BaseExpression::getType() const{
	return t;
}

BaseExpression::~BaseExpression(){};


template <typename T>
AlgExpression<T>::AlgExpression(){
	if(std::is_same<T,float>::value)
		t=FLOAT;
	else if(std::is_same<T,int>::value)
		t=INT;
	else if(std::is_same<T,bool>::value)
		t=BOOL;
	else
		throw;
}
template <typename T>
AlgExpression<T>::~AlgExpression(){}


template <typename T >
Constant<T>::Constant(T v) : val(v){}

template <typename T>
T Constant<T>::evaluate(/*const State &state*/) const{
	return val;
}

template class Constant<float>;
template class Constant<int>;
template class Constant<bool>;


template <bool isBool>
SomeAlgExpression SomeAlgExpression::makeBinaryOperation
		(const SomeAlgExpression &ex1,const SomeAlgExpression &ex2,const int op){
	BaseExpression::Type type1 = ex1.t;
	BaseExpression::Type type2 = ex2.t;
	BaseExpression::Type ret = isBool ? BaseExpression::BOOL : std::min(type1,type2);
	SomeAlgExpression new_expr;

#define SET_ALG_EXPRESSION(some,R,T1,T2) \
	if( !isBool ){ \
		some.R ## Exp = new BinaryOperation<R,T1,T2>(ex1.T1 ## Exp,ex2.T2 ## Exp,op);\
		some.t = BaseExpression::EnumType<R>::t;\
	} else {\
		some.boolExp = new BinaryOperation<bool,T1,T2>(ex1.T1 ## Exp,ex2.T2 ## Exp,op);\
		some.t = BaseExpression::BOOL;\
	}

	switch(type1){
	case BaseExpression::FLOAT:
		switch(type2){
		case BaseExpression::FLOAT:
			SET_ALG_EXPRESSION(new_expr,float,float,float);
			break;
		case BaseExpression::INT:
			SET_ALG_EXPRESSION(new_expr,float,float,int);
			break;
		case BaseExpression::BOOL:
			SET_ALG_EXPRESSION(new_expr,float,float,bool);
			break;
		}
		break;
	case BaseExpression::INT:
		switch(type2){
		case BaseExpression::FLOAT:
			SET_ALG_EXPRESSION(new_expr,float,int,float);
			break;
		case BaseExpression::INT:
			SET_ALG_EXPRESSION(new_expr,int,int,int);
			break;
		case BaseExpression::BOOL:
			SET_ALG_EXPRESSION(new_expr,int,int,bool);
			break;
		}
		break;
	case BaseExpression::BOOL:
		switch(type2){
		case BaseExpression::FLOAT:
			SET_ALG_EXPRESSION(new_expr,float,bool,float);
			break;
		case BaseExpression::INT:
			SET_ALG_EXPRESSION(new_expr,int,bool,int);
			break;
		case BaseExpression::BOOL:
			SET_ALG_EXPRESSION(new_expr,bool,bool,bool);
			break;
		}
		break;
	}

	return new_expr;
}
template SomeAlgExpression SomeAlgExpression::makeBinaryOperation<true>
	(const SomeAlgExpression &ex1,const SomeAlgExpression &ex2,const int op);
template SomeAlgExpression SomeAlgExpression::makeBinaryOperation<false>
	(const SomeAlgExpression &ex1,const SomeAlgExpression &ex2,const int op);


template <typename T1,typename T2>
bool (*BinaryOperations<bool,T1,T2>::funcs[10]) (T1,T2)={
		[](T1 v1,T2 v2){return v1 && v2;},
		[](T1 v1,T2 v2){return v1 || v2;},
		[](T1 v1,T2 v2){return v1 > v2;},
		[](T1 v1,T2 v2){return v1 < v2;},
		[](T1 v1,T2 v2){return v1 == v2;},
		[](T1 v1,T2 v2){return v1 != v2;},
		[](T1 v1,T2 v2){return true;},
		[](T1 v1,T2 v2){return false;}
};
template <typename R,typename T1,typename T2>
R (*BinaryOperations<R,T1,T2>::funcs[10]) (T1,T2)={
		[](T1 v1,T2 v2){return v1+v2;},
		[](T1 v1,T2 v2){return v1-v2;},
		[](T1 v1,T2 v2){return v1*v2;},
		[](T1 v1,T2 v2){return v1/v2;},
		[](T1 v1,T2 v2){return (R)std::pow(v1,v2);},
		[](T1 v1,T2 v2){return (R)((int)v1%(int)v2);},
		[](T1 v1,T2 v2){return (v1 > v2 ? v1 : v2);},
		[](T1 v1,T2 v2){return (v1 > v2 ? v2 : v1);}
};

template <typename R,typename T1,typename T2>
R BinaryOperation< R, T1, T2>::evaluate(/*const State &state*/) const {
	return func(exp1->evaluate(),exp2->evaluate());
}

template <typename R,typename T1,typename T2>
BinaryOperation< R, T1, T2>::BinaryOperation
		(const AlgExpression<T1> *ex1,const AlgExpression<T2> *ex2,int op)
{
	exp1=ex1;
	exp2=ex2;
	func = BinaryOperations<R,T1,T2>::funcs[op];
}

template <typename R,typename T1,typename T2>
BinaryOperation< R, T1, T2>::~BinaryOperation(){
	delete exp1;
	delete exp2;
}

} /* namespace ast */
