/*
 * AlgExpression.cpp
 *
 *  Created on: Jul 22, 2016
 *      Author: naxo
 */

#include "AlgExpression.h"

#include <cmath>		//std::pow
#include <algorithm>	//std::max,std::min
#include <type_traits>	//std::conditional
#include "../util/Exceptions.h" //semanticError

namespace state {

//template <> struct BaseExpression::TypeDef<BaseExpression::INT> { typedef int t;};
//template <> struct BaseExpression::TypeDef<BaseExpression::BOOL> { typedef bool t;};
template <> struct BaseExpression::EnumType<int> {static const Type t = INT;};
template <> struct BaseExpression::EnumType<bool> {static const Type t = BOOL;};


/****** BaseExpression *******/
const BaseExpression::Type BaseExpression::getType() const{
	return t;
}

std::set<std::string> BaseExpression::getAuxiliars() const {
	return std::set<std::string>();
}

BaseExpression::~BaseExpression(){};

template <bool isBool>
BaseExpression* BaseExpression::makeBinaryExpression(const BaseExpression *ex1,const BaseExpression *ex2,
		const int op){
	BaseExpression::Type type1 = ex1->getType();
	BaseExpression::Type type2 = ex2->getType();
	typedef typename std::conditional<isBool,bool,float>::type BoolOrFloat;
	typedef typename std::conditional<isBool,bool,int>::type BoolOrInt;
	BaseExpression* bin_op;

	switch(type1){
	case BaseExpression::FLOAT:
		switch(type2){
		case BaseExpression::FLOAT:
			bin_op = new BinaryOperation<BoolOrFloat,float,float>(ex1,ex2,op);
			break;
		case BaseExpression::INT:
			bin_op = new BinaryOperation<BoolOrFloat,float,int>(ex1,ex2,op);
			break;
		case BaseExpression::BOOL:
			bin_op = new BinaryOperation<BoolOrFloat,float,bool>(ex1,ex2,op);
			break;
		}
		break;
	case BaseExpression::INT:
		switch(type2){
		case BaseExpression::FLOAT:
			bin_op = new BinaryOperation<BoolOrFloat,int,float>(ex1,ex2,op);
			break;
		case BaseExpression::INT:
			bin_op = new BinaryOperation<BoolOrInt,int,int>(ex1,ex2,op);
			break;
		case BaseExpression::BOOL:
			bin_op = new BinaryOperation<BoolOrInt,int,bool>(ex1,ex2,op);
			break;
		}
		break;
	case BaseExpression::BOOL:
		switch(type2){
		case BaseExpression::FLOAT:
			bin_op = new BinaryOperation<BoolOrFloat,bool,float>(ex1,ex2,op);
			break;
		case BaseExpression::INT:
			bin_op = new BinaryOperation<BoolOrInt,bool,int>(ex1,ex2,op);
			break;
		case BaseExpression::BOOL:
			bin_op = new BinaryOperation<bool,bool,bool>(ex1,ex2,op);
			break;
		}
		break;
	}

	return bin_op;
}
template BaseExpression* BaseExpression::makeBinaryExpression<true>
	(const BaseExpression *ex1,const BaseExpression *ex2,const int op);
template BaseExpression* BaseExpression::makeBinaryExpression<false>
	(const BaseExpression *ex1,const BaseExpression *ex2,const int op);



/****** SomeValue ************/
SomeValue::SomeValue(float f) : fVal(f),t(BaseExpression::FLOAT){}
SomeValue::SomeValue(int i) : iVal(i),t(BaseExpression::INT){}
SomeValue::SomeValue(bool b) : bVal(b),t(BaseExpression::BOOL){}
SomeValue::SomeValue(const std::string &s) : sVal(new std::string(s)),t(BaseExpression::STR){}

template <typename T>
T SomeValue::valueAs() const {
	switch(t){
	case BaseExpression::FLOAT:
		return static_cast<T>(fVal);
	case BaseExpression::INT:
		return static_cast<T>(iVal);
	case BaseExpression::BOOL:
		return static_cast<T>(bVal);
	case BaseExpression::STR:
		return std::stof(*sVal);
	}
	return 0;
}
template float SomeValue::valueAs<float>() const;
template int SomeValue::valueAs<int>() const;
template bool SomeValue::valueAs<bool>() const;


/****** AlgExpression ********/
template <typename T>
AlgExpression<T>::AlgExpression(){
	if(std::is_same<T,float>::value)
		t=FLOAT;
	else if(std::is_same<T,int>::value)
		t=INT;
	else if(std::is_same<T,bool>::value)
		t=BOOL;
	else
		throw std::exception();
}
template <typename T>
AlgExpression<T>::~AlgExpression(){}

/*template <typename T>
T AlgExpression<T>::evaluate(std::unordered_map<std::string,int> *aux_values) const {
	try{
		return this->eval(aux_values);
	}catch(std::out_of_range &e){

	}catch(std::exception &e){

	}
}*/

template <typename T>
const SomeValue AlgExpression<T>::getValue(const std::unordered_map<std::string,int> *aux_values) const{
	return SomeValue(this->evaluate(aux_values));
}


/******* Constant ************/
template <typename T >
Constant<T>::Constant(T v) : val(v){}

template <typename T>
T Constant<T>::evaluate(const std::unordered_map<std::string,int> *aux_values) const{
	return val;
}
//TODO
template <typename T>
float Constant<T>::auxFactors(std::unordered_map<std::string,float> &aux_values) const{
	return (int)val;
}

template class Constant<float>;
template class Constant<int>;
template class Constant<bool>;



/*********** BinaryOperations ******************/


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
		[](T1 v1,T2 v2){return (v1 > v2 ? v1 : v2);},//Max
		[](T1 v1,T2 v2){return (v1 > v2 ? v2 : v1);}//Min
};

template <typename R,typename T1,typename T2>
R BinaryOperation< R, T1, T2>::evaluate(const std::unordered_map<std::string,int> *aux_values) const {
	auto a = exp1->evaluate(aux_values);
	auto b = exp2->evaluate(aux_values);
	return func(a,b);
}

template <typename R,typename T1,typename T2>
BinaryOperation< R, T1, T2>::BinaryOperation
		(const BaseExpression *ex1,const BaseExpression *ex2,const short op)
		:op(op)
{
	exp1=dynamic_cast<const state::AlgExpression<T1>*>(ex1);
	exp2=dynamic_cast<const state::AlgExpression<T2>*>(ex2);
	func = BinaryOperations<R,T1,T2>::funcs[op];
}

template <typename R,typename T1,typename T2>
BinaryOperation< R, T1, T2>::~BinaryOperation(){
	delete exp1;
	delete exp2;
}

template <typename R,typename T1,typename T2>
float BinaryOperation<R,T1,T2>::auxFactors(std::unordered_map<std::string,float> &var_factors) const{
	typedef std::unordered_map<std::string,float> var_map;
	var_map vars1,vars2;
	float val1 = exp1->auxFactors(vars1);
	float val2 = exp2->auxFactors(vars2);
	if(op < BaseExpression::MULT){// + or -
		for(var_map::iterator it = vars1.begin();it != vars1.end();it++){
			var_factors[it->first]=func(it->second,vars2[it->first]);
			vars2.erase(it->first);
		}
		for(var_map::iterator it = vars2.begin();it != vars2.end();it++)
			var_factors[it->first]=func(0,it->second);
	}else if(op < BaseExpression::POW){// * or /
		if(vars1.size() > 0)
			if(vars2.size() > 0)
				throw SemanticError("Only linear equations can be used for auxiliars in compartment expressions.");
			else
				for(var_map::iterator it = vars1.begin();it != vars1.end();it++)
					var_factors[it->first]=func(it->second,val2);
		else
			for(var_map::iterator it = vars2.begin();it != vars2.end();it++)
				var_factors[it->first]=func(val1,it->second);

	}
	return func(val1,val2);
}

template <typename R,typename T1,typename T2>
std::set<std::string> BinaryOperation<R,T1,T2>::getAuxiliars() const{
	auto l1 = exp1->getAuxiliars();
	auto l2 = exp2->getAuxiliars();
	l1.insert(l2.begin(),l2.end());
	return l1;
}


/*********** Class Auxiliar ***********/
Auxiliar::Auxiliar(const std::string &nme) : name(nme){}
Auxiliar::~Auxiliar(){}
int Auxiliar::evaluate(const std::unordered_map<std::string,int> *aux_values) const{
	try{
		return aux_values->at(name);
	}catch(std::out_of_range &e){
		//(*aux_values)[name] = 1;
		throw e;
	}catch(std::exception &e){
		throw e;//TODO
	}
	return 0;
}

float Auxiliar::auxFactors(std::unordered_map<std::string,float> &var_factors) const{
	var_factors[name] = 1;
	return 0;
}

std::set<std::string> Auxiliar::getAuxiliars() const{
	return std::set<std::string>(&name,&name+1);
}

} /* namespace ast */
