/*
 * Basics.h
 *
 *  Created on: Dec 28, 2015
 *      Author: naxo
 */

#ifndef GRAMMAR_AST_BASICS_H_
#define GRAMMAR_AST_BASICS_H_

#include <string>
#include <list>
#include "../location.hh"
#include "../../state/Variable.h"
#include "../../pattern/Environment.h"
#include "../../pattern/Dependencies.h"

//#define DEBUG

namespace grammar {


namespace ast {


using namespace std;

/** \brief A node is an element in the AST.
 *
 * Most elements in the AST have a location, ie. the file name, line number and begin/end chars. */
class Node {
public:
	typedef yy::location location;
	location loc;///< The file name, line number and begin/end chars of the AST object.

	Node();///< \brief constructs an AST node with no location.
	Node(const yy::location& l);
	virtual ~Node() = 0;
	virtual void show( string tabs = "" ) const;
};



/** \brief Names of (kappa) entities in the AST. */
class Id : public Node {
protected:
	string id;
public:
	//Id(const string &s): Node(),id(s){};
	Id();
	Id(const Node &l,const string &s);
	Id(const location &l,const string &s);
	~Id();

	const string& getString() const;
	void show( string tabs = "" ) const;
};


/** \brief Base class for math algebraic expressions.   */
class Expression : public Node {
public:
	/** \brief Flags for giving context to the eval() method of an Expression object.*/
	enum Info {
		FORCE=1,	///< \deprecated ? Force the expression to be evaluated immediately.
		AUX_ALLOW=2,///< This expression allows the use of auxiliary variables.
		CONST=4,	///< This is a constant expression so it doesn't allows variables.
		PATTERN=8,	///< This expression is inside of an agent-pattern.
		LHS=16,		///< This expression is in the left-hand side of a rule.
		RHS=32		///< This expression is in the right-hand side of a rule.
	};
	typedef vector<state::Variable*> VAR;///< Useful for give the variable collection as a parameter.
	Expression();
	Expression(const location &l);
	using Node::Node;
	virtual ~Expression()=0;

	/** \brief Constructs a expressions::BaseExpression from the AST.
	 *
	 *  BaseExpressions are used to get numeric (or boolean) values during the simulation.
	 *  @param env The pattern::Environment of the simulation.
	 *  @param vars Collection of all %var and %const read from the kappa files, that
	 *  can be needed for this expression evaluation.
	 *  @param deps Dependencies of this expression from other entities are stored here for
	 *  future uses.
	 *  @param flags Flag-Byte with information of the context where this expression is.
	 *  Use the Expression::Info enum to check every bit meaning.
	 *  @param aux_map Mapping of the type {aux-name -\> (mix-id,ag-id,site-id)} that
	 *  contains every auxiliary var already evaluated in the context of this Expression.
	 *  @returns A pointer to a expressions::BaseExpression object.							*/
	virtual expressions::BaseExpression* eval(const pattern::Environment& env,
			const VAR &vars,pattern::DepSet* deps = nullptr,const char flags = 0,
			const map<string,tuple<int,small_id,small_id>>* aux_map = nullptr) const = 0;

	/** \brief Returns true only if the expression is a Const. */
	virtual bool isConstant();

	/** \brief Returns a copy of this expression (with no pointer dependency). */
	virtual Expression* clone() const = 0;
	virtual void show( string tabs = "" ) const;
};



/** \brief A pair (Id,Expression) to hold a var-name and a value. */
struct VarValue : public Node{
	Id var;
	const Expression *value;

	VarValue();
	VarValue(const location &l,const Id &name,const Expression *exp);
	virtual void show( string tabs = "" ) const;
};


/** \brief Strings and algebraic expressions concatenated (by a dot=".") */
class StringExpression : public Node {
public:
	StringExpression();

	StringExpression(const location &l,const string s);
	StringExpression(const location &l,const Expression *e);

	void show(string tabs = "") const;

	string evalConst(const pattern::Environment& env,const VarVector& vars) const;

	~StringExpression();

protected:
	enum Type {STR,ALG} t;
	string str;
	const Expression *alg;

	/*union{
		const string *str;
		const Expression  *alg;
	};*/
};

/** \brief Direction of a Reaction or Channel
 *
 *	deprecated.
 *//*
class Arrow : public Node {
public:
	Arrow();
	enum ArrType {RIGHT,LEFT,BI};
	Arrow(const location &l,ArrType t);
	ArrType getType();
protected:
	ArrType type;
};*/

/*
class StringOrPrint {
	bool isPrint;
	union {
		std::string str;
		PrintExpression printExpr;
	};
public:
	StringOrPrint();
};*/


}//namespace ast

}//namespace grammar



#endif /* GRAMMAR_AST_BASICS_H_ */
