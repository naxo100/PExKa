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
#include "State.h"

namespace state {

//template <> struct BaseExpression::TypeDef<BaseExpression::INT> { typedef int t;};
//template <> struct BaseExpression::TypeDef<BaseExpression::BOOL> { typedef bool t;};
template <> struct BaseExpression::EnumType<int> {static const Type t = INT;};
template <> struct BaseExpression::EnumType<bool> {static const Type t = BOOL;};


/****** BaseExpression *******/
const BaseExpression::Type BaseExpression::getType() const{
	return t;
}

bool BaseExpression::operator!=(const BaseExpression& exp) const {
	return ! ( (*this) == exp);
}

/*std::set<std::string> BaseExpression::getAuxiliars() const {
	return std::set<std::string>();
}*/

BaseExpression::~BaseExpression(){};

/*SomeValue BaseExpression::getValue(const std::unordered_map<std::string,int> *aux_values = nullptr) const{
	throw std::invalid_argument("Cannot call this expression without state.");
}
SomeValue BaseExpression::getValue(const state::State& state) const{
	throw std::invalid_argument("Cannot call this expression without auxiliars.");
}*/

template <bool isBool>
BaseExpression* BaseExpression::makeBinaryExpression(const BaseExpression *ex1,const BaseExpression *ex2,
		const int op){
	BaseExpression::Type type1 = ex1->getType();
	BaseExpression::Type type2 = ex2->getType();
	typedef typename std::conditional<isBool,bool,FL_TYPE>::type BoolOrFloat;
	typedef typename std::conditional<isBool,bool,int>::type BoolOrInt;
	BaseExpression* bin_op = nullptr;

	switch(type1){
	case BaseExpression::FLOAT:
		switch(type2){
		case BaseExpression::FLOAT:
			bin_op = new BinaryOperation<BoolOrFloat,FL_TYPE,float>(ex1,ex2,op);
			break;
		case BaseExpression::INT:
			bin_op = new BinaryOperation<BoolOrFloat,FL_TYPE,int>(ex1,ex2,op);
			break;
		case BaseExpression::BOOL:
			bin_op = new BinaryOperation<BoolOrFloat,FL_TYPE,bool>(ex1,ex2,op);
			break;
		default:
			SemanticError("Not a valid value for a binary operation",yy::location());
		}
		break;
	case BaseExpression::INT:
		switch(type2){
		case BaseExpression::FLOAT:
			bin_op = new BinaryOperation<BoolOrFloat,int,FL_TYPE>(ex1,ex2,op);
			break;
		case BaseExpression::INT:
			bin_op = new BinaryOperation<BoolOrInt,int,int>(ex1,ex2,op);
			break;
		case BaseExpression::BOOL:
			bin_op = new BinaryOperation<BoolOrInt,int,bool>(ex1,ex2,op);
			break;
		default:
			SemanticError("Not a valid value for a binary operation",yy::location());
		}
		break;
	case BaseExpression::BOOL:
		switch(type2){
		case BaseExpression::FLOAT:
			bin_op = new BinaryOperation<BoolOrFloat,bool,FL_TYPE>(ex1,ex2,op);
			break;
		case BaseExpression::INT:
			bin_op = new BinaryOperation<BoolOrInt,bool,int>(ex1,ex2,op);
			break;
		case BaseExpression::BOOL:
			bin_op = new BinaryOperation<bool,bool,bool>(ex1,ex2,op);
			break;
		default:
			SemanticError("Not a valid value for a binary operation",yy::location());
		}
		break;
	default:
		SemanticError("Not a valid value for a binary operation",yy::location());
	}

	return bin_op;
}
template BaseExpression* BaseExpression::makeBinaryExpression<true>
	(const BaseExpression *ex1,const BaseExpression *ex2,const int op);
template BaseExpression* BaseExpression::makeBinaryExpression<false>
	(const BaseExpression *ex1,const BaseExpression *ex2,const int op);



/****** SomeValue ************/
SomeValue::SomeValue(FL_TYPE f) : fVal(f),t(BaseExpression::FLOAT){}
SomeValue::SomeValue(int i) : fVal(0.0),t(BaseExpression::INT){iVal=i;}
SomeValue::SomeValue(bool b) : fVal(0.0),t(BaseExpression::BOOL){bVal=b;}
SomeValue::SomeValue(small_id i) : fVal(0.0),t(BaseExpression::SMALL_ID){smallVal=i;}
SomeValue::SomeValue(short_id i) : fVal(0.0),t(BaseExpression::SHORT_ID){shortVal=i;}

//SomeValue::SomeValue(const std::string &s) : sVal(new std::string(s)),t(BaseExpression::STR){}

template <typename T>
T SomeValue::valueAs() const {
	switch(t){
	case BaseExpression::FLOAT:
		return static_cast<T>(fVal);
	case BaseExpression::INT:
		return static_cast<T>(iVal);
	case BaseExpression::BOOL:
		return static_cast<T>(bVal);
	default:
		throw std::invalid_argument("SomeValue::valueAs(): not sense in convert id-like values.");
	/*case BaseExpression::STR:
		return std::stof(*sVal);*/
	}
	return 0;
}
template FL_TYPE SomeValue::valueAs<FL_TYPE>() const;
template int SomeValue::valueAs<int>() const;
template bool SomeValue::valueAs<bool>() const;

template <> void SomeValue::set(FL_TYPE f){fVal = f;}
template <> void SomeValue::set(int i){iVal = i;}
template <> void SomeValue::set(bool b){bVal = b;}
template <> void SomeValue::set(small_id i){smallVal = i;}
template <> void SomeValue::set(short_id i){shortVal = i;}

bool SomeValue::operator !=(const SomeValue& val) const {
	/*switch(t){
	case BaseExpression::FLOAT:
			return static_cast<T>(fVal);
		case BaseExpression::INT:
			return static_cast<T>(iVal);
		case BaseExpression::BOOL:
			return static_cast<T>(bVal);
		default:
			throw std::invalid_argument("SomeValue::valueAs(): not sense in convert id-like values.");
	}*/
	return val.fVal != fVal || val.t != t;
}
bool SomeValue::operator ==(const SomeValue& val) const {
	return val.fVal == fVal && val.t == t;
}

/*************************************/
/********** AlgExpression ************/
/*************************************/
template <typename T>
AlgExpression<T>::AlgExpression(){
	if(std::is_same<T,FL_TYPE>::value)
		t=FLOAT;
	else if(std::is_same<T,int>::value)
		t=INT;
	else if(std::is_same<T,bool>::value)
		t=BOOL;
	else
		throw std::invalid_argument("Algebraic Expression can only be float, int or bool.");
}
template <typename T>
AlgExpression<T>::~AlgExpression(){}


template <typename T>
SomeValue AlgExpression<T>::getValue(const std::unordered_map<std::string,int> *aux_values) const{
	return SomeValue(this->evaluate(aux_values));
}
template <typename T>
SomeValue AlgExpression<T>::getValue(const state::State& state,const std::unordered_map<std::string,FL_TYPE>&& aux) const{
	return SomeValue(this->evaluate(state,aux));
}

template class AlgExpression<FL_TYPE>;
template class AlgExpression<int>;
template class AlgExpression<bool>;

/******* Constant ************/
template <typename T >
Constant<T>::Constant(T v) : val(v){}

template <typename T>
T Constant<T>::evaluate(const std::unordered_map<std::string,int> *aux_values) const{
	return val;
}
template <typename T>
T Constant<T>::evaluate(const state::State& state,const AuxMap& aux_values) const{
	return val;
}
//TODO
template <typename T>
FL_TYPE Constant<T>::auxFactors(std::unordered_map<std::string,FL_TYPE> &aux_values) const{
	return (int)val;
}

template <typename T>
bool Constant<T>::operator==(const BaseExpression& exp) const{
	try{
		auto& const_exp = dynamic_cast<const Constant<T>&>(exp);
		return const_exp.val == val;
	}
	catch(bad_cast &ex){	}
	return false;
}

template class Constant<FL_TYPE>;
template class Constant<int>;
template class Constant<bool>;


/***********************************************/
/*********** BinaryOperations ******************/
/***********************************************/

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
R BinaryOperation< R, T1, T2>::evaluate(const state::State& state,const AuxMap& aux_values) const {
	auto a = exp1->evaluate(state,aux_values);
	auto b = exp2->evaluate(state,aux_values);
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
	if(!dynamic_cast<const Variable*>(exp1))
		delete exp1;
	if(!dynamic_cast<const Variable*>(exp2))
		delete exp2;
}

template <typename R,typename T1,typename T2>
FL_TYPE BinaryOperation<R,T1,T2>::auxFactors(std::unordered_map<std::string,FL_TYPE> &var_factors) const{
	typedef std::unordered_map<std::string,FL_TYPE> var_map;
	var_map vars1,vars2;
	FL_TYPE val1 = exp1->auxFactors(vars1);
	FL_TYPE val2 = exp2->auxFactors(vars2);
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
				throw std::invalid_argument("Only linear equations can be used for auxiliars in compartment expressions.");
			else
				for(var_map::iterator it = vars1.begin();it != vars1.end();it++)
					var_factors[it->first]=func(it->second,val2);
		else
			for(var_map::iterator it = vars2.begin();it != vars2.end();it++)
				var_factors[it->first]=func(val1,it->second);

	}
	return func(val1,val2);
}

/*template <typename R,typename T1,typename T2>
std::set<std::string> BinaryOperation<R,T1,T2>::getAuxiliars() const{
	auto l1 = exp1->getAuxiliars();
	auto l2 = exp2->getAuxiliars();
	l1.insert(l2.begin(),l2.end());
	return l1;
}*/

template <typename R,typename T1,typename T2>
bool BinaryOperation<R,T1,T2>::operator==(const BaseExpression& exp) const{
	try{
		auto& binary_exp = dynamic_cast<const BinaryOperation<R,T1,T2>&>(exp);
		if(binary_exp.op == op)
			return (*binary_exp.exp1) == (*exp1) && (*binary_exp.exp2) == (*exp2);
	}
	catch(bad_cast &ex){	}
	return false;
}


template class BinaryOperation<int,int,int>;
//template class BinaryOperation<int,int,FL_TYPE>;
//template class BinaryOperation<int,FL_TYPE,int>;
//template class BinaryOperation<int,FL_TYPE,FL_TYPE>;
//template class BinaryOperation<FL_TYPE,int,int>;
template class BinaryOperation<FL_TYPE,int,FL_TYPE>;
template class BinaryOperation<FL_TYPE,FL_TYPE,int>;
template class BinaryOperation<FL_TYPE,FL_TYPE,FL_TYPE>;

/*********** Class Auxiliar ***********/
template <typename R>
Auxiliar<R>::Auxiliar(const std::string &nme) : name(nme){}

template <typename R>
Auxiliar<R>::~Auxiliar(){}

template <typename R>
R Auxiliar<R>::evaluate(const std::unordered_map<std::string,int> *aux_values) const{
	try{
		if(aux_values)
			return aux_values->at(name);
		else
			throw std::out_of_range("No given auxiliar map for Auxiliar::evaluate().");
	}catch(std::out_of_range &e){
		//(*aux_values)[name] = 1;
		throw e;
	}catch(std::exception &e){
		throw e;//TODO
	}
	return 0;
}

template <typename R>
R Auxiliar<R>::evaluate(const state::State& state,const AuxMap& aux_values) const{
	try{
		return aux_values.at(name);
	}
	catch(out_of_range& ex){
		throw ex;//TODO throw better exception
	}
}

template <typename R>
FL_TYPE Auxiliar<R>::auxFactors(std::unordered_map<std::string,FL_TYPE> &var_factors) const{
	var_factors[name] = 1;
	return 0;
}

/*template <typename R>
std::set<std::string> Auxiliar<R>::getAuxiliars() const{
	return std::set<std::string>(&name,&name+1);
}*/

template <typename T>
bool Auxiliar<T>::operator==(const BaseExpression& exp) const{
	try{
		auto& aux_exp = dynamic_cast<const Auxiliar<T>&>(exp);
		if(aux_exp.name == name)
			return true;
	}
	catch(bad_cast &ex){	}
	return false;
}

template class Auxiliar<int>;
template class Auxiliar<FL_TYPE>;

/********************************************/
/************** class VarLabel **************/
/********************************************/
template <typename R>
VarLabel<R>::VarLabel(BaseExpression* expr) : var(expr){}
template <typename R>
R VarLabel<R>::evaluate(std::unordered_map<std::string,int> *aux_values) const {
	throw std::invalid_argument("This should never been used");
}
template <typename R>
R VarLabel<R>::evaluate(const state::State& state,const AuxMap& aux_values) const {
	throw std::invalid_argument("This should never been used");
}
template <typename R>
int VarLabel<R>::auxFactors(std::unordered_map<std::string,int> &factor) const {
	throw std::invalid_argument("This should never been used");
}

template <typename T>
bool VarLabel<T>::operator==(const BaseExpression& exp) const{
	try{
		auto& var_exp = dynamic_cast<const VarLabel<T>&>(exp);
		return var_exp.var == var;
	}
	catch(bad_cast &ex){	}
	return false;
}



/**************************/
/****** class Token *******/
/**************************/

TokenVar::TokenVar(unsigned _id) : id(_id) {}
FL_TYPE TokenVar::evaluate(const std::unordered_map<std::string,int> *aux_values) const {
	throw std::invalid_argument("Cannot call Token::evaluate() without state.");
}
FL_TYPE TokenVar::evaluate(const state::State& state,const AuxMap& aux_values) const{
	return state.getTokenValue(id);
}
FL_TYPE TokenVar::auxFactors(std::unordered_map<std::string,FL_TYPE> &factor) const {
	throw std::invalid_argument("Cannot use tokens in this expression.");
}


bool TokenVar::operator==(const BaseExpression& exp) const{
	try{
		auto& tok_exp = dynamic_cast<const TokenVar&>(exp);
		return tok_exp.id == id;
	}
	catch(bad_cast &ex){	}
	return false;
}


} /* namespace ast */
