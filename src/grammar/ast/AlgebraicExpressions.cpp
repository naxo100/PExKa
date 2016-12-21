/*
 * Expression.cpp
 *
 *  Created on: May 2, 2016
 *      Author: naxo
 */

#include "AlgebraicExpressions.h"
#include "../../state/Variable.h"

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

void Const::show( string tabs ) const {
	string ar_types[] = {"FLOAT","INTEGER","BOOL","INF","INF_NEG","TMAX,EMAX"};
	tabs += "   ";

	cout << endl << tabs << "Constant : (" << ar_types[type] << ", ";

	switch(type){
	case FLOAT:
		cout << f;
		break;
	case INTEGER:
		cout << n;
		break;
	case BOOL:
		cout << b;
		break;
	case INF:
		cout << "infinity";
		break;
	case INF_NEG:
		cout << "negative infinity";
		break;
	case TMAX:
		cout << "time max";
		break;
	case ConstType::EMAX:
		cout << "event max";
		break;
	}

	cout << ")";
}

BaseExpression* Const::eval(pattern::Environment& env,
		const VAR &vars,
		const char flags) const{
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
		const VAR &vars,
		const char flags) const{
	BaseExpression* ex1 = exp1->eval(env,vars,flags);
	BaseExpression* ex2 = exp2->eval(env,vars,flags);

	return BaseExpression::makeBinaryExpression<true>(ex1,ex2,op);
}
BoolBinaryOperation* BoolBinaryOperation::clone() const{
	return new BoolBinaryOperation(loc,exp1->clone(),exp2->clone(),op);
}
void BoolBinaryOperation::show( string tabs ) const {
	string ar_types[] = {"AND","OR","GREATER","SMALLER","EQUAL","DIFF"};
	tabs += "   ";

	cout << endl << tabs << "boolBinaryOperation: (";
	exp1->show( tabs );
	cout << endl << tabs+"   " << "operator: " << ar_types[op];
	exp2->show( tabs );
	cout << endl << tabs << ")";
}


/****** Class AlgBinaryOperation ****/
AlgBinaryOperation::AlgBinaryOperation(const location &l,const Expression *e1,
		const Expression *e2,BaseExpression::AlgebraicOp o):
		Expression(l),exp1(e1),exp2(e2),op(o){};

BaseExpression* AlgBinaryOperation::eval(pattern::Environment& env,
		const VAR &vars,
		const char flags) const{
	BaseExpression* ex1 = exp1->eval(env,vars,flags);
	BaseExpression* ex2 = exp2->eval(env,vars,flags);

	return BaseExpression::makeBinaryExpression<false>(ex1,ex2,op);
}
AlgBinaryOperation* AlgBinaryOperation::clone() const{
	return new AlgBinaryOperation(loc,exp1->clone(),exp2->clone(),op);
}
void AlgBinaryOperation::show( string tabs ) const {
	string ar_types[] = {"SUM","MINUS","MULT","DIV","POW","MODULO","MAX","MIN"};
	tabs += "   ";

	cout << endl << tabs << "algBinaryOperation: (";
	exp1->show( tabs );
	cout << endl << tabs+"   " << "operator: " << ar_types[op];
	exp2->show( tabs );
	cout << endl << tabs << ")";
}


/****** Class UnaryOperation ********/
UnaryOperation::UnaryOperation(const location &l,const Expression *e,
		const BaseExpression::Unary f):
		Expression(l),exp(e),func(f){};
BaseExpression* UnaryOperation::eval(pattern::Environment& env,
		const VAR &vars,
		const char flags) const{
	return new Constant<int>(0);
}
UnaryOperation* UnaryOperation::clone() const{
	return new UnaryOperation(loc,exp->clone(),func);
}
void UnaryOperation::show( string tabs ) const {
	string ar_type[] = {"SQRT","EXPONENT","LOG","SINUS","COSINUS","TAN","ATAN","ABS","COIN","RAND_N","NOT"};
	tabs += "   ";

	cout << endl << tabs << "UnaryOperation: (";
	exp->show( tabs );
	cout << endl << tabs+"   " << "operation: " << ar_type[func];
	cout << endl << tabs << ")";
}


/****** Class NullaryOperation ******/
BaseExpression* NullaryOperation::eval(pattern::Environment& env,
		const VAR &vars,
		const char flags) const{
	return new Constant<int>(0);
}
NullaryOperation::NullaryOperation(const location &l,const BaseExpression::Nullary f):
		 Expression(l), func(f){};
NullaryOperation* NullaryOperation::clone() const{
	return new NullaryOperation(*this);
}
void NullaryOperation::show( string tabs ) const {
	string ar_type[] = {"TRUE","FALSE","RAND_1"};
	tabs += "   ";

	cout << endl << tabs << "NullaryOperation: (" << ar_type[func] << ")";
}


/****** Class Var ***/
Var::Var(const location &l,const VarType &t,const Id &label):
	Expression(l),name(label),type(t){};

BaseExpression* Var::eval(pattern::Environment& env,
		const Expression::VAR &vars,
		const char flags) const {
	BaseExpression* expr;
	switch(type){
	case VAR:
		try {
			short id = env.getVarId(name.getString());
			expr = vars[id];
		}
		catch(const SemanticError &err){

		}
		break;
	case AUX:
		expr = new state::Auxiliar(name.getString());
		break;
	default:break;
	}
	return expr;
}
Var* Var::clone() const{
	return new Var(*this);
}

void Var::show( string tabs ) const {
	string ar_type[]= {"VAR","TOKEN","AUX","TIME","EVENT","NULL_EVENT","PROD_EVENT","CPUTIME","ACTIVITY"};
	tabs += "   ";

	cout << endl << tabs << "Variable: " << name.getString() << ", type: " << ar_type[type];
}

} /* namespace ast */
