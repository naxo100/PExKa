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

namespace ast {

using namespace state;
using namespace expressions;

/** \brief Constants in a Math Expression or Rate value
 *
 */
class Const : public Expression {
public:
	enum ConstType {FLOAT,INTEGER,BOOL,INF,INF_NEG,TMAX,EMAX};
	Const (const location &loc,const float f);
	Const (const location &loc,const int i);
	Const (const location &loc,const bool b);
	BaseExpression* eval(const pattern::Environment& env,
			const VAR &vars,pattern::DepSet* deps = nullptr,
			const char flags = 0) const override;
	bool isConstant();
	Const (const location &loc,const ConstType t);
	Const* clone() const override;
	void show( string tabs = "" ) const;
protected:
	ConstType type;
	union {int n;float f;bool b;};
};

/** \brief A boolean Operation between two boolean Expression
 *
 */
class BoolBinaryOperation: public Expression {
public:
	BoolBinaryOperation(const location &loc,const Expression *e1,const Expression *e2,
			BaseExpression::BoolOp o);
	BaseExpression* eval(const pattern::Environment& env,
			const VAR &vars,pattern::DepSet* deps = nullptr,
			const char flags = 0) const override;
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
	BaseExpression* eval(const pattern::Environment& env,
			const VAR &vars,pattern::DepSet* deps = nullptr,
			const char flags = 0) const override;
	AlgBinaryOperation* clone() const override;
	void show( string tabs = "" ) const;

protected:
	const Expression *exp1;
	const Expression *exp2;
	BaseExpression::AlgebraicOp op;
};

/** \brief An Algebraic Operation or Function with only 1 argument
 *
 */
class UnaryOperation: public Expression{
public:
	UnaryOperation(const location &loc,const Expression *e,
			const BaseExpression::Unary f);
	BaseExpression* eval(const pattern::Environment& env,
			const VAR &vars,pattern::DepSet* deps = nullptr,
			const char flags = 0) const override;
	UnaryOperation* clone() const override;
	void show( string tabs = "" ) const;
protected:
	const Expression* exp;
	BaseExpression::Unary func;
};

/** \brief A Math "Procedure" or Function without arguments
 *
 */
class NullaryOperation: public Expression {
public:
	BaseExpression::Nullary func;
	BaseExpression* eval(const pattern::Environment& env,
			const VAR &vars,pattern::DepSet* deps = nullptr,
			const char flags = 0) const override;
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
	BaseExpression* eval(const pattern::Environment& env,
			const Expression::VAR &vars,pattern::DepSet* deps = nullptr,
			const char flags = 0) const override;
	Var* clone() const override;
	void show( string tabs = "" ) const;
protected:
	Id name;
	VarType type;
};


} /* namespace ast */

#endif /* GRAMMAR_AST_ALGEBRAICEXPRESSIONS_H_ */
