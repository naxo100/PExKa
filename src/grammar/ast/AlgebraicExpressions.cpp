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
		Expression(loc),type(FLOAT),f(f){}
Const::Const (const location &loc,const int i):
		Expression(loc),type(INTEGER),n(i){}
Const::Const (const location &loc,const bool b):
		Expression(loc),type(BOOL),b(b){}
Const::Const (const location &loc,const ConstType t):
		Expression(loc),type(t){}

bool Const::isConstant(){
	return true;
}

BaseExpression* Const::eval(pattern::Environment& env,
		const unordered_map<string,state::Variable*> &vars) const{
	BaseExpression* cons;
	switch(type){
	case FLOAT:
		cons = new state::Constant<float>(f);
		break;
	case INTEGER:
		cons = new state::Constant<int>(n);
		break;
	case BOOL:
		cons = new state::Constant<bool>(b);
		break;
	case INF:
		cons = new state::Constant<float>(std::numeric_limits<float>::infinity());
		break;
	case INF_NEG:
		cons = new state::Constant<float>(-std::numeric_limits<float>::infinity());
		break;
	case TMAX:
		cons = new state::Constant<int>(-1);
		break;
	case ConstType::EMAX:
		cons = new state::Constant<int>(-1);
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

BaseExpression* BoolBinaryOperation::eval(pattern::Environment& env,
		const unordered_map<string,state::Variable*> &vars) const{
	BaseExpression* ex1 = exp1->eval(env,vars);
	BaseExpression* ex2 = exp2->eval(env,vars);

	return BaseExpression::makeBinaryExpression<true>(ex1,ex2,op);
}
BoolBinaryOperation* BoolBinaryOperation::clone() const{
	return new BoolBinaryOperation(loc,exp1->clone(),exp2->clone(),op);
}


/****** Class AlgBinaryOperation ****/
AlgBinaryOperation::AlgBinaryOperation(const location &l,const Expression *e1,
		const Expression *e2,BaseExpression::AlgebraicOp o):
		Expression(l),exp1(e1),exp2(e2),op(o){};

BaseExpression* AlgBinaryOperation::eval(pattern::Environment& env,
		const unordered_map<string,state::Variable*> &vars) const{
	BaseExpression* ex1 = exp1->eval(env,vars);
	BaseExpression* ex2 = exp2->eval(env,vars);

	return BaseExpression::makeBinaryExpression<false>(ex1,ex2,op);
}
AlgBinaryOperation* AlgBinaryOperation::clone() const{
	return new AlgBinaryOperation(loc,exp1->clone(),exp2->clone(),op);
}


/****** Class UnaryOperation ********/
UnaryOperation::UnaryOperation(const location &l,const Expression *e,
		const BaseExpression::Unary f):
		Expression(l),exp(e),func(f){};
BaseExpression* UnaryOperation::eval(pattern::Environment& env,
		const unordered_map<string,state::Variable*> &vars) const{
	return new Constant<int>(0);
}
UnaryOperation* UnaryOperation::clone() const{
	return new UnaryOperation(loc,exp->clone(),func);
}


/****** Class NullaryOperation ******/
BaseExpression* NullaryOperation::eval(pattern::Environment& env,
		const unordered_map<string,state::Variable*> &vars) const{
	return new Constant<int>(0);
}
NullaryOperation::NullaryOperation(const location &l,const BaseExpression::Nullary f):
		 Expression(l), func(f){};
NullaryOperation* NullaryOperation::clone() const{
	return new NullaryOperation(*this);
}


/****** Class Var ***/
Var::Var(const location &l,const VarType &t,const std::string &label):
	Expression(l),name(label),type(t){};

BaseExpression* Var::eval(pattern::Environment& env,
		const unordered_map<string,state::Variable*> &vars) const {
	BaseExpression* expr;
	switch(type){
	case VAR:
		try {
			//short id =
			env.getVarId(name);
			expr = new VarLabel<float>();

		}catch(const SemanticError &err){

		}
		break;
	default:break;
	}
	return expr;
}
Var* Var::clone() const{
	return new Var(*this);
}


} /* namespace ast */
