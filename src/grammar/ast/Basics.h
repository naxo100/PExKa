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

using namespace std;

namespace ast {

class Node {
public:
	typedef yy::location location;
	location loc;

	Node();
	Node(const yy::location& l);
	virtual ~Node() = 0;
	virtual void show( string tabs = "" ) const;
};



//Agent Name and Similars
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


/** \brief Base Class for Math Algebraic Expression
 *
 */
class Expression : public Node {
public:
	typedef vector<state::Variable*> VAR;
	enum Info {FORCE=1,AUX_ALLOW=2,CONST=4,PATTERN=8,LHS=16,RHS=32};
	Expression();
	Expression(const location &l);
	using Node::Node;
	virtual ~Expression()=0;

	virtual expressions::BaseExpression* eval(const pattern::Environment& env,
			const VAR &vars,pattern::DepSet* deps = nullptr,
			const char flags = 0) const;
	virtual bool isConstant();
	virtual Expression* clone() const = 0;
	virtual void show( string tabs = "" ) const;
};



//TODO class VarValue
struct VarValue : public Node{
	Id var;
	const Expression *value;

	VarValue();
	VarValue(const location &l,const Id &name,const Expression *exp);
	virtual void show( string tabs = "" ) const;
};


/** \brief Strings and algebraic expressions concatenated (by a dot=".")
 *
 */
class StringExpression : public Node {
public:
	StringExpression();

	StringExpression(const location &l,const string s);
	StringExpression(const location &l,const Expression *e);

	void show(string tabs = "") const;

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


}



#endif /* GRAMMAR_AST_BASICS_H_ */
