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

#define DEBUG

using namespace std;

namespace ast {

class Node {
public:
	typedef yy::location location;
	Node();
	Node(const yy::location& l);
	virtual ~Node() = 0;
	location loc;
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
	enum FLAGS {FORCE=1,AUX=2,CONST=4};
	Expression();
	Expression(const location &l);
	using Node::Node;
	virtual ~Expression()=0;

	virtual state::BaseExpression* eval(pattern::Environment& env,
			const VAR &vars,
			const char flags = 0) const;
	virtual bool isConstant();
	virtual Expression* clone() const = 0;
	virtual void show( string tabs = "" ) const;
};



//TODO class VarValue
class VarValue : public Node{
	Id var;
	const Expression *value;
public:
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
	StringExpression(const location &l,const string &s);
	StringExpression(const location &l,const Expression *e);
	StringExpression(const location &l,const Expression *e, const StringExpression* nxt);
	StringExpression(const location &l,const string &s, const StringExpression* nxt);

	~StringExpression();

protected:
	enum Type {STR,ALG} t;
	union{
		const string *str;
		const Expression  *alg;
	};
	StringExpression* next;
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
