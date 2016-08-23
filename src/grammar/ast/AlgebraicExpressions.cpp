/*
 * Expression.cpp
 *
 *  Created on: May 2, 2016
 *      Author: naxo
 */

#include "AlgebraicExpressions.h"

#include <limits>
#include <type_traits>

namespace ast {

/****** Class Const **********/
Const::Const (const location &loc,const float f):
		Expression(loc),f(f),type(FLOAT){}
Const::Const (const location &loc,const int i):
		Expression(loc),n(i),type(INTEGER){}
Const::Const (const location &loc,const bool b):
		Expression(loc),b(b),type(BOOL){}
Const::Const (const location &loc,const ConstType t):
		Expression(loc),type(t){}

bool Const::isConstant(){
	return true;
}

SomeAlgExpression Const::eval(pattern::Environment& env) const{
	SomeAlgExpression cons;
	switch(type){
	case FLOAT:
		cons.floatExp = new Constant<float>(f);
		cons.t = BaseExpression::FLOAT;
		break;
	case INTEGER:
		cons.intExp = new state::Constant<int>(n);
		cons.t = BaseExpression::INT;
		break;
	case BOOL:
		cons.boolExp = new state::Constant<bool>(b);
		cons.t = BaseExpression::BOOL;
		break;
	case INF:
		cons.floatExp = new state::Constant<float>(std::numeric_limits<float>::infinity());
		cons.t = BaseExpression::FLOAT;
		break;
	case INF_NEG:
		cons.floatExp = new state::Constant<float>(-std::numeric_limits<float>::infinity());
		cons.t = BaseExpression::FLOAT;
		break;
	case TMAX:
		cons.intExp = new state::Constant<int>(-1);
		cons.t = BaseExpression::INT;
		break;
	case ConstType::EMAX:
		cons.intExp = new state::Constant<int>(-1);
		cons.t = BaseExpression::INT;
		break;
	}
	return cons;
}
Const* Const::clone() const{
	return new Const(*this);
}

/****** Class BoolBinaryOperation ***/
BoolBinaryOperation::BoolBinaryOperation(const location &l,const Expression *e1,
		const Expression *e2,BaseExpression::BoolOp o):
		Expression(l),exp1(e1),exp2(e2),op(o){}

SomeAlgExpression BoolBinaryOperation::eval(pattern::Environment& env) const{
	SomeAlgExpression ex1 = exp1->eval(env);
	SomeAlgExpression ex2 = exp2->eval(env);

	return SomeAlgExpression::makeBinaryOperation<true>(ex1,ex2,op);
}
BoolBinaryOperation* BoolBinaryOperation::clone() const{
	return new BoolBinaryOperation(loc,exp1->clone(),exp2->clone(),op);
}


/****** Class AlgBinaryOperation ****/
AlgBinaryOperation::AlgBinaryOperation(const location &l,const Expression *e1,
		const Expression *e2,BaseExpression::AlgebraicOp o):
		Expression(l),exp1(e1),exp2(e2),op(o){};

SomeAlgExpression AlgBinaryOperation::eval(pattern::Environment& env) const{
	SomeAlgExpression ex1 = exp1->eval(env);
	SomeAlgExpression ex2 = exp2->eval(env);

	return SomeAlgExpression::makeBinaryOperation<false>(ex1,ex2,op);
}
AlgBinaryOperation* AlgBinaryOperation::clone() const{
	return new AlgBinaryOperation(loc,exp1->clone(),exp2->clone(),op);
}


/****** Class UnaryOperation ********/
UnaryOperation::UnaryOperation(const location &l,const Expression *e,
		const BaseExpression::Unary f):
		Expression(l),exp(e),func(f){};
SomeAlgExpression UnaryOperation::eval(pattern::Environment& env) const{
	return SomeAlgExpression();
}
UnaryOperation* UnaryOperation::clone() const{
	return new UnaryOperation(loc,exp->clone(),func);
}


/****** Class NullaryOperation ******/
SomeAlgExpression NullaryOperation::eval(pattern::Environment& env) const{
	return SomeAlgExpression();
}
NullaryOperation::NullaryOperation(const location &l,const BaseExpression::Nullary f):
		 Expression(l), func(f){};
NullaryOperation* NullaryOperation::clone() const{
	return new NullaryOperation(*this);
}


/****** Class Var ***/
Var::Var(const location &l,const std::string &label,const VarType &t):
	Expression(l),id(label),type(t){};
Var::Var(const location &l,const VarType &t):
	Expression(l),type(t){};

SomeAlgExpression Var::eval(pattern::Environment& env) const {
	return SomeAlgExpression();
}
Var* Var::clone() const{
	return new Var(*this);
}


} /* namespace ast */
