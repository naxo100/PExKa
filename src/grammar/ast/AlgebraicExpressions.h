/*
 * Expression.h
 *
 *  Created on: May 2, 2016
 *      Author: naxo
 */

#ifndef GRAMMAR_AST_ALGEBRAICEXPRESSIONS_H_
#define GRAMMAR_AST_ALGEBRAICEXPRESSIONS_H_

#include "../location.hh"
#include "../../state/Variable.h"
#include "../../pattern/Environment.h"
#include "Basics.h"

namespace grammar {
namespace ast {

using namespace state;
using namespace expressions;

/** \brief Constants in a math expression or rate value. */
class Const : public Expression {
public:
	/** The type of the constant value, or a prefixed value  */
	enum ConstType {FLOAT,INTEGER,BOOL,INF,INF_NEG,TMAX,EMAX};
	Const (const location &loc,const float f);
	Const (const location &loc,const int i);
	Const (const location &loc,const bool b);

	/** \brief Evaluate the const AST to return a expressions::Constant pointer.
	 *
	 * @param env,vars,deps,flags,aux_map All params ignored.
	 * @returns an expression::Constant pointer (as a Expression). 					*/
	BaseExpression* eval(const pattern::Environment& env,const VAR &vars,
			pattern::DepSet* deps = nullptr,const char flags = 0,
			const map<string,tuple<int,small_id,small_id>>* aux_map = nullptr) const override;
	bool isConstant();
	Const (const location &loc,const ConstType t);
	Const* clone() const override;
	void show( string tabs = "" ) const;
protected:
	ConstType type;
	union {int n;float f;bool b;};
};

/** \brief A boolean Operation between two boolean Expression  */
class BoolBinaryOperation: public Expression {
public:
	BoolBinaryOperation(const location &loc,const Expression *e1,const Expression *e2,
			BaseExpression::BoolOp o);
	~BoolBinaryOperation();

	/** \brief Return the expressions::BinaryExpression from this AST.
	 *  */
	BaseExpression* eval(const pattern::Environment& env,const VAR &vars,
			pattern::DepSet* deps = nullptr,const char flags = 0,
			const map<string,tuple<int,small_id,small_id>>* aux_map = nullptr) const override;
	BoolBinaryOperation* clone() const override;
	void show( string tabs = "" ) const;
protected:
	const Expression *exp1,*exp2;
	BaseExpression::BoolOp op;
};

/** \brief An Algebraic Operation or Function with 2 arguments
 *
 */
class AlgBinaryOperation: public Expression {
public:
	AlgBinaryOperation(const location &loc,const Expression *e1,
			const Expression *e2,BaseExpression::AlgebraicOp o);
	~AlgBinaryOperation();
	BaseExpression* eval(const pattern::Environment& env,const VAR &vars,
			pattern::DepSet* deps = nullptr,const char flags = 0,
			const map<string,tuple<int,small_id,small_id>>* aux_map = nullptr) const override;
	AlgBinaryOperation* clone() const override;
	void show( string tabs = "" ) const;

protected:
	const Expression *exp1;
	const Expression *exp2;
	BaseExpression::AlgebraicOp op;
};

/** \brief An algebraic operation or function with only 1 argument.
 *
 */
class UnaryOperation: public Expression{
public:
	UnaryOperation(const location &loc,const Expression *e,
			const BaseExpression::Unary f);
	~UnaryOperation();
	BaseExpression* eval(const pattern::Environment& env,const VAR &vars,
			pattern::DepSet* deps = nullptr,const char flags = 0,
			const map<string,tuple<int,small_id,small_id>>* aux_map = nullptr) const override;
	UnaryOperation* clone() const override;
	void show( string tabs = "" ) const;
protected:
	const Expression* exp;
	BaseExpression::Unary func;
};

/** \brief A math "Procedure" or function without arguments.
 *
 */
class NullaryOperation: public Expression {
public:
	BaseExpression::Nullary func;
	BaseExpression* eval(const pattern::Environment& env,const VAR &vars,
			pattern::DepSet* deps = nullptr,const char flags = 0,
			const map<string,tuple<int,small_id,small_id>>* aux_map = nullptr) const override;
	NullaryOperation* clone() const override;
	NullaryOperation(const location &loc,const BaseExpression::Nullary f);
	void show( string tabs = "" ) const;
};

/** \brief User variables
 *
 */
class Var : public Expression {
public:
	enum VarType {VAR,TOKEN,AUX};
	Var(const location &loc,const VarType &t,const Id &label=Id());
	BaseExpression* eval(const pattern::Environment& env,const Expression::VAR &vars,
			pattern::DepSet* deps = nullptr,const char flags = 0,
			const map<string,tuple<int,small_id,small_id>>* aux_map = nullptr) const override;
	Var* clone() const override;
	void show( string tabs = "" ) const;
protected:
	Id name;
	VarType type;
};


/** \brief Functions with more than one parameter.  */
class Func : public Expression {
	BaseExpression::Funcs type;
	list<Expression*> args;
public:
	Func(const location &loc,BaseExpression::Funcs f,const list<Expression*>& args);
	~Func();
	BaseExpression* eval(const pattern::Environment& env,const Expression::VAR &vars,
			pattern::DepSet* deps = nullptr,const char flags = 0,
			const map<string,tuple<int,small_id,small_id>>* aux_map = nullptr) const override;
	Func* clone() const override;
	void show( string tabs = "" ) const;
};


} /* namespace ast */
}

#endif /* GRAMMAR_AST_ALGEBRAICEXPRESSIONS_H_ */
